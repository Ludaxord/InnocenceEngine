#include "PreTAAPass.h"
#include "../DefaultGPUBuffers/DefaultGPUBuffers.h"

#include "LightPass.h"
#include "SkyPass.h"

#include "../../Engine/Interface/IModuleManager.h"

INNO_ENGINE_API extern IModuleManager* g_pModuleManager;

using namespace DefaultGPUBuffers;

namespace PreTAAPass
{
	RenderPassDataComponent* m_RPDC;
	ShaderProgramComponent* m_SPC;
	TextureDataComponent* m_TDC;
}

bool PreTAAPass::Setup()
{
	m_SPC = g_pModuleManager->getRenderingServer()->AddShaderProgramComponent("PreTAAPass/");

	m_SPC->m_ShaderFilePaths.m_CSPath = "preTAAPass.comp/";

	m_RPDC = g_pModuleManager->getRenderingServer()->AddRenderPassDataComponent("PreTAAPass/");

	auto l_RenderPassDesc = g_pModuleManager->getRenderingFrontend()->getDefaultRenderPassDesc();

	l_RenderPassDesc.m_RenderTargetCount = 0;
	l_RenderPassDesc.m_RenderPassUsage = RenderPassUsage::Compute;

	m_RPDC->m_RenderPassDesc = l_RenderPassDesc;

	m_RPDC->m_ResourceBinderLayoutDescs.resize(3);
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_ResourceBinderType = ResourceBinderType::Image;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_DescriptorSetIndex = 1;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_DescriptorIndex = 0;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_BinderAccessibility = Accessibility::ReadOnly;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_ResourceAccessibility = Accessibility::ReadWrite;
	m_RPDC->m_ResourceBinderLayoutDescs[0].m_IndirectBinding = true;

	m_RPDC->m_ResourceBinderLayoutDescs[1].m_ResourceBinderType = ResourceBinderType::Image;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_DescriptorSetIndex = 1;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_DescriptorIndex = 1;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_BinderAccessibility = Accessibility::ReadOnly;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_ResourceAccessibility = Accessibility::ReadWrite;
	m_RPDC->m_ResourceBinderLayoutDescs[1].m_IndirectBinding = true;

	m_RPDC->m_ResourceBinderLayoutDescs[2].m_ResourceBinderType = ResourceBinderType::Image;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_DescriptorSetIndex = 2;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_DescriptorIndex = 0;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_BinderAccessibility = Accessibility::ReadWrite;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_ResourceAccessibility = Accessibility::ReadWrite;
	m_RPDC->m_ResourceBinderLayoutDescs[2].m_IndirectBinding = true;

	m_RPDC->m_ShaderProgram = m_SPC;

	m_TDC = g_pModuleManager->getRenderingServer()->AddTextureDataComponent("PreTAAPass/");
	m_TDC->m_TextureDesc = l_RenderPassDesc.m_RenderTargetDesc;

	return true;
}

bool PreTAAPass::Initialize()
{
	g_pModuleManager->getRenderingServer()->InitializeShaderProgramComponent(m_SPC);
	g_pModuleManager->getRenderingServer()->InitializeRenderPassDataComponent(m_RPDC);
	g_pModuleManager->getRenderingServer()->InitializeTextureDataComponent(m_TDC);

	return true;
}

bool PreTAAPass::PrepareCommandList()
{
	auto l_viewportSize = g_pModuleManager->getRenderingFrontend()->getScreenResolution();

	g_pModuleManager->getRenderingServer()->CommandListBegin(m_RPDC, 0);
	g_pModuleManager->getRenderingServer()->BindRenderPassDataComponent(m_RPDC);
	g_pModuleManager->getRenderingServer()->CleanRenderTargets(m_RPDC);

	g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Compute, LightPass::GetResult(0), 0, 0);
	g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Compute, SkyPass::GetResult(), 1, 1);
	g_pModuleManager->getRenderingServer()->ActivateResourceBinder(m_RPDC, ShaderStage::Compute, m_TDC->m_ResourceBinder, 2, 0, Accessibility::ReadWrite);

	g_pModuleManager->getRenderingServer()->DispatchCompute(m_RPDC, uint32_t(l_viewportSize.x / 8.0f), uint32_t(l_viewportSize.y / 8.0f), 1);

	g_pModuleManager->getRenderingServer()->DeactivateResourceBinder(m_RPDC, ShaderStage::Compute, LightPass::GetResult(0), 0, 0);
	g_pModuleManager->getRenderingServer()->DeactivateResourceBinder(m_RPDC, ShaderStage::Compute, SkyPass::GetResult(), 1, 1);
	g_pModuleManager->getRenderingServer()->DeactivateResourceBinder(m_RPDC, ShaderStage::Compute, m_TDC->m_ResourceBinder, 2, 0, Accessibility::ReadWrite);

	g_pModuleManager->getRenderingServer()->CommandListEnd(m_RPDC);

	return true;
}

bool PreTAAPass::Terminate()
{
	g_pModuleManager->getRenderingServer()->DeleteRenderPassDataComponent(m_RPDC);

	return true;
}

RenderPassDataComponent* PreTAAPass::GetRPDC()
{
	return m_RPDC;
}

ShaderProgramComponent* PreTAAPass::GetSPC()
{
	return m_SPC;
}

IResourceBinder* PreTAAPass::GetResult()
{
	return m_TDC->m_ResourceBinder;
}