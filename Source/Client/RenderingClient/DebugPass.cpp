#include "DebugPass.h"
#include "../DefaultGPUBuffers/DefaultGPUBuffers.h"
#include "../../Engine/Common/CommonMacro.inl"
#include "../../Engine/ComponentManager/ITransformComponentManager.h"
#include "../../Engine/ComponentManager/IVisibleComponentManager.h"

#include "GIDataLoader.h"
#include "OpaquePass.h"

#include "../../Engine/Interface/IModuleManager.h"

INNO_ENGINE_API extern IModuleManager* g_pModuleManager;

using namespace DefaultGPUBuffers;

struct DebugPerObjectConstantBuffer
{
	Mat4 m;
	uint32_t materialID;
	uint32_t padding[15];
};

struct DebugMaterialConstantBuffer
{
	Vec4 color;
};

namespace DebugPass
{
	bool AddPDCMeshData(PhysicsDataComponent* PDC);
	bool AddBVHNode(BVHNode* node);

	RenderPassDataComponent* m_RPDC;
	ShaderProgramComponent* m_SPC;

	GPUBufferDataComponent* m_debugSphereMeshGBDC;
	GPUBufferDataComponent* m_debugCubeMeshGBDC;
	GPUBufferDataComponent* m_debugMaterialGBDC;

	const size_t m_maxDebugMeshes = 65536;
	const size_t m_maxDebugMaterial = 512;
	std::vector<DebugPerObjectConstantBuffer> m_debugSphereConstantBuffer;
	std::vector<DebugPerObjectConstantBuffer> m_debugCubeConstantBuffer;
	std::vector<DebugMaterialConstantBuffer> m_debugMaterialConstantBuffer;
}

bool DebugPass::Setup()
{
	m_debugSphereMeshGBDC = g_pModuleManager->getRenderingServer()->AddGPUBufferDataComponent("DebugSphereMeshGPUBuffer/");
	m_debugSphereMeshGBDC->m_ElementCount = m_maxDebugMeshes;
	m_debugSphereMeshGBDC->m_ElementSize = sizeof(DebugPerObjectConstantBuffer);
	m_debugSphereMeshGBDC->m_GPUAccessibility = Accessibility::ReadWrite;

	m_debugCubeMeshGBDC = g_pModuleManager->getRenderingServer()->AddGPUBufferDataComponent("DebugCubeMeshGPUBuffer/");
	m_debugCubeMeshGBDC->m_ElementCount = m_maxDebugMeshes;
	m_debugCubeMeshGBDC->m_ElementSize = sizeof(DebugPerObjectConstantBuffer);
	m_debugCubeMeshGBDC->m_GPUAccessibility = Accessibility::ReadWrite;

	m_debugMaterialGBDC = g_pModuleManager->getRenderingServer()->AddGPUBufferDataComponent("DebugMaterialGPUBuffer/");
	m_debugMaterialGBDC->m_ElementCount = m_maxDebugMaterial;
	m_debugMaterialGBDC->m_ElementSize = sizeof(DebugMaterialConstantBuffer);
	m_debugMaterialGBDC->m_GPUAccessibility = Accessibility::ReadWrite;

	////
	m_SPC = g_pModuleManager->getRenderingServer()->AddShaderProgramComponent("DebugPass/");

	m_SPC->m_ShaderFilePaths.m_VSPath = "debugPass.vert/";
	m_SPC->m_ShaderFilePaths.m_PSPath = "debugPass.frag/";
	m_RPDC = g_pModuleManager->getRenderingServer()->AddRenderPassDataComponent("DebugPass/");

	auto l_RenderPassDesc = g_pModuleManager->getRenderingFrontend()->getDefaultRenderPassDesc();

	l_RenderPassDesc.m_RenderTargetCount = 1;
	l_RenderPassDesc.m_UseDepthBuffer = true;

	l_RenderPassDesc.m_GraphicsPipelineDesc.m_DepthStencilDesc.m_DepthEnable = true;
	l_RenderPassDesc.m_GraphicsPipelineDesc.m_DepthStencilDesc.m_AllowDepthWrite = true;
	l_RenderPassDesc.m_GraphicsPipelineDesc.m_DepthStencilDesc.m_DepthComparisionFunction = ComparisionFunction::LessEqual;

	l_RenderPassDesc.m_GraphicsPipelineDesc.m_RasterizerDesc.m_UseCulling = false;
	l_RenderPassDesc.m_GraphicsPipelineDesc.m_RasterizerDesc.m_RasterizerFillMode = RasterizerFillMode::Wireframe;

	m_RPDC->m_RenderPassDesc = l_RenderPassDesc;

	m_RPDC->m_ResourceBinderLayoutDescs.resize(3);
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_ResourceBinderType = ResourceBinderType::Buffer;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_DescriptorSetIndex = 0;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_DescriptorIndex = 0;

	m_RPDC->m_ResourceBinderLayoutDescs[1].m_ResourceBinderType = ResourceBinderType::Buffer;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_DescriptorSetIndex = 1;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_DescriptorIndex = 0;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_BinderAccessibility = Accessibility::ReadOnly;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_ResourceAccessibility = Accessibility::ReadWrite;

	m_RPDC->m_ResourceBinderLayoutDescs[2].m_ResourceBinderType = ResourceBinderType::Buffer;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_DescriptorSetIndex = 1;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_DescriptorIndex = 1;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_BinderAccessibility = Accessibility::ReadOnly;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_ResourceAccessibility = Accessibility::ReadWrite;

	m_RPDC->m_ShaderProgram = m_SPC;

	m_debugSphereConstantBuffer.reserve(m_maxDebugMeshes);
	m_debugCubeConstantBuffer.reserve(m_maxDebugMeshes);
	m_debugMaterialConstantBuffer.reserve(m_maxDebugMaterial);

	return true;
}

bool DebugPass::Initialize()
{
	g_pModuleManager->getRenderingServer()->InitializeGPUBufferDataComponent(m_debugSphereMeshGBDC);
	g_pModuleManager->getRenderingServer()->InitializeGPUBufferDataComponent(m_debugCubeMeshGBDC);
	g_pModuleManager->getRenderingServer()->InitializeGPUBufferDataComponent(m_debugMaterialGBDC);
	g_pModuleManager->getRenderingServer()->InitializeShaderProgramComponent(m_SPC);
	g_pModuleManager->getRenderingServer()->InitializeRenderPassDataComponent(m_RPDC);

	return true;
}

bool DebugPass::AddPDCMeshData(PhysicsDataComponent* PDC)
{
	DebugPerObjectConstantBuffer l_cubeMeshData;

	l_cubeMeshData.m = InnoMath::toTranslationMatrix(PDC->m_AABBWS.m_center);
	l_cubeMeshData.m.m00 *= PDC->m_AABBWS.m_extend.x / 2.0f;
	l_cubeMeshData.m.m11 *= PDC->m_AABBWS.m_extend.y / 2.0f;
	l_cubeMeshData.m.m22 *= PDC->m_AABBWS.m_extend.z / 2.0f;
	l_cubeMeshData.materialID = 4;

	m_debugCubeConstantBuffer.emplace_back(l_cubeMeshData);

	DebugPerObjectConstantBuffer l_sphereMeshData;

	l_sphereMeshData.m = InnoMath::toTranslationMatrix(PDC->m_SphereWS.m_center);
	l_sphereMeshData.m.m00 *= 0.5f;
	l_sphereMeshData.m.m11 *= 0.5f;
	l_sphereMeshData.m.m22 *= 0.5f;
	l_sphereMeshData.materialID = 3;

	m_debugSphereConstantBuffer.emplace_back(l_sphereMeshData);

	return true;
}

bool DebugPass::AddBVHNode(BVHNode* node)
{
	if (node)
	{
		if (node->intermediatePDC)
		{
			AddPDCMeshData(node->intermediatePDC);
			if (node->leftChildNode)
			{
				AddBVHNode(node->leftChildNode);
			}
			if (node->rightChildNode)
			{
				AddBVHNode(node->rightChildNode);
			}
		}

		auto l_PDCCount = node->childrenPDCs.size();

		if (l_PDCCount)
		{
			for (size_t i = 0; i < l_PDCCount; i++)
			{
				auto l_PDC = node->childrenPDCs[i];
				AddPDCMeshData(l_PDC);
			}
		}
	}

	return true;
}

bool DebugPass::Render()
{
	auto l_renderingConfig = g_pModuleManager->getRenderingFrontend()->getRenderingConfig();

	if (l_renderingConfig.drawDebugObject)
	{
		auto l_PerFrameCBufferGBDC = GetGPUBufferDataComponent(GPUBufferUsageType::PerFrame);
		auto l_sphere = g_pModuleManager->getRenderingFrontend()->getMeshDataComponent(ProceduralMeshShape::Sphere);
		auto l_cube = g_pModuleManager->getRenderingFrontend()->getMeshDataComponent(ProceduralMeshShape::Cube);

		m_debugSphereConstantBuffer.clear();
		m_debugCubeConstantBuffer.clear();
		m_debugMaterialConstantBuffer.clear();

		for (size_t i = 0; i < 5; i++)
		{
			m_debugMaterialConstantBuffer.emplace_back();
		}

		m_debugMaterialConstantBuffer[0].color = Vec4(0.1f, 0.2f, 0.4f, 1.0f);
		m_debugMaterialConstantBuffer[1].color = Vec4(0.4f, 0.2f, 0.1f, 1.0f);
		m_debugMaterialConstantBuffer[2].color = Vec4(0.9f, 0.1f, 0.0f, 1.0f);
		m_debugMaterialConstantBuffer[3].color = Vec4(0.8f, 0.1f, 0.1f, 1.0f);
		m_debugMaterialConstantBuffer[4].color = Vec4(0.1f, 0.6f, 0.2f, 1.0f);

		static bool l_drawProbes = false;
		if (l_drawProbes)
		{
			auto l_probes = GIDataLoader::GetProbes();

			if (l_probes.size() > 0)
			{
				for (size_t i = 0; i < l_probes.size(); i++)
				{
					DebugPerObjectConstantBuffer l_meshData;

					l_meshData.m = InnoMath::toTranslationMatrix(l_probes[i].pos);
					l_meshData.m.m00 *= 0.5f;
					l_meshData.m.m11 *= 0.5f;
					l_meshData.m.m22 *= 0.5f;
					l_meshData.materialID = 0;

					m_debugSphereConstantBuffer.emplace_back(l_meshData);
				}

				auto l_brickFactor = GIDataLoader::GetBrickFactors();

				// @TODO:
				auto l_probeIndexBegin = 0;
				auto l_probeIndexEnd = 0;

				for (size_t probeIndex = l_probeIndexBegin; probeIndex < l_probeIndexEnd; probeIndex++)
				{
					m_debugSphereConstantBuffer[probeIndex].materialID = 2;

					auto l_probe = l_probes[probeIndex];

					for (size_t i = 0; i < 6; i++)
					{
						auto l_brickFactorBegin = l_probe.brickFactorRange[i * 2];
						auto l_brickFactorEnd = l_probe.brickFactorRange[i * 2 + 1];

						for (size_t j = l_brickFactorBegin; j < l_brickFactorEnd; j++)
						{
							auto l_brickIndex = l_brickFactor[j].brickIndex;
							m_debugCubeConstantBuffer[l_brickIndex].materialID = 3;
						}
					}
				}
			}
		}

		static bool l_drawBricks = false;
		if (l_drawBricks)
		{
			auto l_bricks = GIDataLoader::GetBricks();

			if (l_bricks.size() > 0)
			{
				for (size_t i = 0; i < l_bricks.size(); i++)
				{
					DebugPerObjectConstantBuffer l_meshData;

					l_meshData.m = InnoMath::toTranslationMatrix(l_bricks[i].boundBox.m_center);
					l_meshData.m.m00 *= l_bricks[i].boundBox.m_extend.x / 2.0f;
					l_meshData.m.m11 *= l_bricks[i].boundBox.m_extend.y / 2.0f;
					l_meshData.m.m22 *= l_bricks[i].boundBox.m_extend.z / 2.0f;
					l_meshData.materialID = 1;

					m_debugCubeConstantBuffer.emplace_back(l_meshData);
				}
			}
		}

		static bool l_drawBVHNodes = false;
		if (l_drawBVHNodes)
		{
			auto l_rootBVHNode = g_pModuleManager->getPhysicsSystem()->getRootBVHNode();

			AddBVHNode(l_rootBVHNode);
		}

		static bool l_drawSkeletons = true;
		if (l_drawSkeletons)
		{
			auto& l_visibleComponents = GetComponentManager(VisibleComponent)->GetAllComponents();

			for (auto i : l_visibleComponents)
			{
				if (i->m_meshUsage == MeshUsage::Skeletal && i->m_model)
				{
					auto l_transformCompoent = GetComponent(TransformComponent, i->m_Owner);
					auto l_m = l_transformCompoent->m_globalTransformMatrix.m_transformationMat;

					for (size_t j = 0; j < i->m_model->meshMaterialPairs.m_count; j++)
					{
						auto l_pair = g_pModuleManager->getAssetSystem()->getMeshMaterialPair(i->m_model->meshMaterialPairs.m_startOffset + j);
						auto l_skeleton = l_pair->mesh->m_SDC;

						for (auto k : l_skeleton->m_BoneData)
						{
							DebugPerObjectConstantBuffer l_meshData;
							auto l_bm = k.m_L2B;
							// Inverse-Joint-Matrix
							l_bm = l_bm.inverse();
							auto l_s = InnoMath::toScaleMatrix(Vec4(0.01f, 0.01f, 0.01f, 1.0f));
							l_bm = l_bm * l_s;
							l_m.m00 = 1.0f;
							l_m.m11 = 1.0f;
							l_m.m22 = 1.0f;
							l_bm = l_m * l_bm;

							l_meshData.m = l_bm;

							l_meshData.materialID = 2;

							m_debugCubeConstantBuffer.emplace_back(l_meshData);
						}
					}
				}
			}
		}

		g_pModuleManager->getRenderingServer()->UploadGPUBufferDataComponent(m_debugMaterialGBDC, m_debugMaterialConstantBuffer, 0, m_debugMaterialConstantBuffer.size());
		if (m_debugSphereConstantBuffer.size())
		{
			g_pModuleManager->getRenderingServer()->UploadGPUBufferDataComponent(m_debugSphereMeshGBDC, m_debugSphereConstantBuffer, 0, m_debugSphereConstantBuffer.size());
		}
		if (m_debugCubeConstantBuffer.size())
		{
			g_pModuleManager->getRenderingServer()->UploadGPUBufferDataComponent(m_debugCubeMeshGBDC, m_debugCubeConstantBuffer, 0, m_debugCubeConstantBuffer.size());
		}

		g_pModuleManager->getRenderingServer()->CommandListBegin(m_RPDC, 0);
		g_pModuleManager->getRenderingServer()->BindRenderPassDataComponent(m_RPDC);
		g_pModuleManager->getRenderingServer()->CleanRenderTargets(m_RPDC);

		g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Vertex, l_PerFrameCBufferGBDC->m_ResourceBinder, 0, 0, Accessibility::ReadOnly);

		g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Pixel, m_debugMaterialGBDC->m_ResourceBinder, 2, 1, Accessibility::ReadOnly);

		g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Vertex, m_debugSphereMeshGBDC->m_ResourceBinder, 1, 0, Accessibility::ReadOnly);
		if (m_debugSphereConstantBuffer.size())
		{
			g_pModuleManager->getRenderingServer()->DispatchDrawCall(m_RPDC, l_sphere, m_debugSphereConstantBuffer.size());
		}

		g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Vertex, m_debugCubeMeshGBDC->m_ResourceBinder, 1, 0, Accessibility::ReadOnly);
		if (m_debugCubeConstantBuffer.size())
		{
			g_pModuleManager->getRenderingServer()->DispatchDrawCall(m_RPDC, l_cube, m_debugCubeConstantBuffer.size());
		}

		g_pModuleManager->getRenderingServer()->CommandListEnd(m_RPDC);
	}
	else
	{
		g_pModuleManager->getRenderingServer()->CommandListBegin(m_RPDC, 0);
		g_pModuleManager->getRenderingServer()->BindRenderPassDataComponent(m_RPDC);
		g_pModuleManager->getRenderingServer()->CleanRenderTargets(m_RPDC);

		g_pModuleManager->getRenderingServer()->CommandListEnd(m_RPDC);
	}

	g_pModuleManager->getRenderingServer()->ExecuteCommandList(m_RPDC);

	g_pModuleManager->getRenderingServer()->WaitForFrame(m_RPDC);

	return true;
}

bool DebugPass::Terminate()
{
	g_pModuleManager->getRenderingServer()->DeleteRenderPassDataComponent(m_RPDC);

	return true;
}

RenderPassDataComponent* DebugPass::GetRPDC()
{
	return m_RPDC;
}

ShaderProgramComponent* DebugPass::GetSPC()
{
	return m_SPC;
}