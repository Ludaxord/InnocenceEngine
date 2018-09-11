#include "PhysicsSystem.h"

const objectStatus & PhysicsSystem::getStatus() const
{
	return m_objectStatus;
}

void PhysicsSystem::setup()
{	
}
void PhysicsSystem::setupComponents()
{
	setupCameraComponents();
	setupVisibleComponents();
	setupLightComponents();
}

void PhysicsSystem::setupCameraComponents()
{
	for (auto& i : g_pGameSystem->getCameraComponents())
	{
		setupCameraComponentProjectionMatrix(i);
		setupCameraComponentRayOfEye(i);
		setupCameraComponentFrustumVertices(i);
		generateAABB(*i);
	}
}

void PhysicsSystem::setupCameraComponentProjectionMatrix(CameraComponent* cameraComponent)
{
	cameraComponent->m_projectionMatrix.initializeToPerspectiveMatrix((cameraComponent->m_FOVX / 180.0) * PI<double>, cameraComponent->m_WHRatio, cameraComponent->m_zNear, cameraComponent->m_zFar);
}

void PhysicsSystem::setupCameraComponentRayOfEye(CameraComponent * cameraComponent)
{
	cameraComponent->m_rayOfEye.m_origin = g_pGameSystem->getTransformComponent(cameraComponent->getParentEntity())->m_transform.caclGlobalPos();
	cameraComponent->m_rayOfEye.m_direction = g_pGameSystem->getTransformComponent(cameraComponent->getParentEntity())->m_transform.getDirection(direction::BACKWARD);
}

void PhysicsSystem::setupCameraComponentFrustumVertices(CameraComponent * cameraComponent)
{
	auto l_NDC = generateNDC();
	auto l_pCamera = cameraComponent->m_projectionMatrix;
	auto l_rCamera = InnoMath::toRotationMatrix(g_pGameSystem->getTransformComponent(cameraComponent->getParentEntity())->m_transform.caclGlobalRot());
	auto l_tCamera = InnoMath::toTranslationMatrix(g_pGameSystem->getTransformComponent(cameraComponent->getParentEntity())->m_transform.caclGlobalPos());

	for (auto& l_vertexData : l_NDC)
	{
		vec4 l_mulPos;
		l_mulPos = l_vertexData.m_pos;
		// from projection space to view space
		//Column-Major memory layout
#ifdef USE_COLUMN_MAJOR_MEMORY_LAYOUT
		l_mulPos = InnoMath::mul(l_mulPos, l_pCamera.inverse());
#endif
		//Row-Major memory layout
#ifdef USE_ROW_MAJOR_MEMORY_LAYOUT
		l_mulPos = InnoMath::mul(l_pCamera.inverse(), l_mulPos);
#endif
		// perspective division
		l_mulPos = l_mulPos * (1.0 / l_mulPos.w);
		// from view space to world space
		//Column-Major memory layout
#ifdef USE_COLUMN_MAJOR_MEMORY_LAYOUT
		l_mulPos = InnoMath::mul(l_mulPos, l_rCamera);
		l_mulPos = InnoMath::mul(l_mulPos, l_tCamera);
#endif
		//Row-Major memory layout
#ifdef USE_ROW_MAJOR_MEMORY_LAYOUT
		l_mulPos = InnoMath::mul(l_rCamera, l_mulPos);
		l_mulPos = InnoMath::mul(l_tCamera, l_mulPos);
#endif
		l_vertexData.m_pos = l_mulPos;
	}

	for (auto& l_vertexData : l_NDC)
	{
		l_vertexData.m_normal = vec4(l_vertexData.m_pos.x, l_vertexData.m_pos.y, l_vertexData.m_pos.z, 0.0).normalize();
	}

	// near clip plane first
	for (size_t i = 0; i < l_NDC.size(); i++)
	{
		cameraComponent->m_frustumVertices.emplace_back(l_NDC[l_NDC.size() -  1 - i]);
	}

	cameraComponent->m_frustumIndices = { 0, 1, 3, 1, 2, 3,
		4, 5, 0, 5, 1, 0,
		7, 6, 4, 6, 5, 4,
		3, 2, 7, 2, 6 ,7,
		4, 0, 7, 0, 3, 7,
		1, 5, 2, 5, 6, 2 };
}

void PhysicsSystem::setupVisibleComponents()
{
	for (auto i : g_pGameSystem->getVisibleComponents())
	{
		if (i->m_visiblilityType != visiblilityType::INVISIBLE)
		{
			generateAABB(*i);
		}
	}
}

void PhysicsSystem::setupLightComponents()
{
	for (auto& i : g_pGameSystem->getLightComponents())
	{
		i->m_direction = vec4(0.0, 0.0, 1.0, 0.0);
		i->m_constantFactor = 1.0;
		i->m_linearFactor = 0.14;
		i->m_quadraticFactor = 0.07;
		setupLightComponentRadius(i);
		i->m_color = vec4(1.0, 1.0, 1.0, 1.0);

		if (i->m_lightType == lightType::DIRECTIONAL)
		{
			generateAABB(*i);
		}
	}
}

void PhysicsSystem::setupLightComponentRadius(LightComponent * lightComponent)
{
	double l_lightMaxIntensity = std::fmax(std::fmax(lightComponent->m_color.x, lightComponent->m_color.y), lightComponent->m_color.z);
	lightComponent->m_radius = -lightComponent->m_linearFactor + std::sqrt(lightComponent->m_linearFactor * lightComponent->m_linearFactor - 4.0 * lightComponent->m_quadraticFactor * (lightComponent->m_constantFactor - (256.0 / 5.0) * l_lightMaxIntensity)) / (2.0 * lightComponent->m_quadraticFactor);
}
std::vector<Vertex> PhysicsSystem::generateNDC()
{
	Vertex l_VertexData_1;
	l_VertexData_1.m_pos = vec4(1.0, 1.0, 1.0, 1.0);
	l_VertexData_1.m_texCoord = vec2(1.0, 1.0);

	Vertex l_VertexData_2;
	l_VertexData_2.m_pos = vec4(1.0, -1.0, 1.0, 1.0);
	l_VertexData_2.m_texCoord = vec2(1.0, 0.0);

	Vertex l_VertexData_3;
	l_VertexData_3.m_pos = vec4(-1.0, -1.0, 1.0, 1.0);
	l_VertexData_3.m_texCoord = vec2(0.0, 0.0);

	Vertex l_VertexData_4;
	l_VertexData_4.m_pos = vec4(-1.0, 1.0, 1.0, 1.0);
	l_VertexData_4.m_texCoord = vec2(0.0, 1.0);

	Vertex l_VertexData_5;
	l_VertexData_5.m_pos = vec4(1.0, 1.0, -1.0, 1.0);
	l_VertexData_5.m_texCoord = vec2(1.0, 1.0);

	Vertex l_VertexData_6;
	l_VertexData_6.m_pos = vec4(1.0, -1.0, -1.0, 1.0);
	l_VertexData_6.m_texCoord = vec2(1.0, 0.0);

	Vertex l_VertexData_7;
	l_VertexData_7.m_pos = vec4(-1.0, -1.0, -1.0, 1.0);
	l_VertexData_7.m_texCoord = vec2(0.0, 0.0);

	Vertex l_VertexData_8;
	l_VertexData_8.m_pos = vec4(-1.0, 1.0, -1.0, 1.0);
	l_VertexData_8.m_texCoord = vec2(0.0, 1.0);

	std::vector<Vertex> l_vertices = { l_VertexData_1, l_VertexData_2, l_VertexData_3, l_VertexData_4, l_VertexData_5, l_VertexData_6, l_VertexData_7, l_VertexData_8 };

	for (auto& l_vertexData : l_vertices)
	{
		l_vertexData.m_normal = vec4(l_vertexData.m_pos.x, l_vertexData.m_pos.y, l_vertexData.m_pos.z, 0.0).normalize();
	}

	return l_vertices;
}

void PhysicsSystem::generateAABB(VisibleComponent & visibleComponent)
{
	double maxX = 0;
	double maxY = 0;
	double maxZ = 0;
	double minX = 0;
	double minY = 0;
	double minZ = 0;

	std::vector<vec4> l_cornerVertices;

	for (auto& l_graphicData : visibleComponent.m_modelMap)
	{
		// get corner vertices from sub meshes
		l_cornerVertices.emplace_back(g_pAssetSystem->findMaxVertex(l_graphicData.first));
		l_cornerVertices.emplace_back(g_pAssetSystem->findMinVertex(l_graphicData.first));
	}

	std::for_each(l_cornerVertices.begin(), l_cornerVertices.end(), [&](vec4 val)
	{
		if (val.x >= maxX)
		{
			maxX = val.x;
		};
		if (val.y >= maxY)
		{
			maxY = val.y;
		};
		if (val.z >= maxZ)
		{
			maxZ = val.z;
		};
		if (val.x <= minX)
		{
			minX = val.x;
		};
		if (val.y <= minY)
		{
			minY = val.y;
		};
		if (val.z <= minZ)
		{
			minZ = val.z;
		};
	});

	visibleComponent.m_AABB = generateAABB(vec4(maxX, maxY, maxZ, 1.0), vec4(minX, minY, minZ, 1.0));
	auto l_worldTm = g_pGameSystem->getTransformComponent(visibleComponent.getParentEntity())->m_transform.caclGlobalTransformationMatrix();
	//Column-Major memory layout
#ifdef USE_COLUMN_MAJOR_MEMORY_LAYOUT
	visibleComponent.m_AABB.m_boundMax = InnoMath::mul(visibleComponent.m_AABB.m_boundMax, l_worldTm);
	visibleComponent.m_AABB.m_boundMin = InnoMath::mul(visibleComponent.m_AABB.m_boundMin,l_worldTm);
	visibleComponent.m_AABB.m_center = InnoMath::mul(visibleComponent.m_AABB.m_center, l_worldTm);
	for (auto& l_vertexData : visibleComponent.m_AABB.m_vertices)
	{
		l_vertexData.m_pos = InnoMath::mul(l_vertexData.m_pos, l_worldTm);
	}
#endif
	//Row-Major memory layout
#ifdef USE_ROW_MAJOR_MEMORY_LAYOUT
	visibleComponent.m_AABB.m_boundMax = InnoMath::mul(l_worldTm, visibleComponent.m_AABB.m_boundMax);
	visibleComponent.m_AABB.m_boundMin = InnoMath::mul(l_worldTm, visibleComponent.m_AABB.m_boundMin);
	visibleComponent.m_AABB.m_center = InnoMath::mul(l_worldTm, visibleComponent.m_AABB.m_center);
	for (auto& l_vertexData : visibleComponent.m_AABB.m_vertices)
	{
		l_vertexData.m_pos = InnoMath::mul(l_worldTm, l_vertexData.m_pos);
	}
#endif
}

void PhysicsSystem::generateAABB(LightComponent & lightComponent)
{
	lightComponent.m_AABBs.clear();

	//1.translate the big frustum to light space
	auto l_camera = g_pGameSystem->getCameraComponents()[0];
	auto l_frustumVertices = l_camera->m_frustumVertices;
	auto l_lightRotMat = g_pGameSystem->getTransformComponent(lightComponent.getParentEntity())->m_transform.getInvertGlobalRotMatrix();
	for (size_t i = 0; i < l_frustumVertices.size(); i++)
	{
		//Column-Major memory layout
#ifdef USE_COLUMN_MAJOR_MEMORY_LAYOUT
		l_frustumVertices[i].m_pos = InnoMath::mul(l_frustumVertices[i].m_pos, l_lightRotMat);
#endif
		//Row-Major memory layout
#ifdef USE_ROW_MAJOR_MEMORY_LAYOUT
		l_frustumVertices[i].m_pos = InnoMath::mul(l_lightRotMat, l_frustumVertices[i].m_pos);
#endif	
	}

	//2.calculate splited planes' corners
	std::vector<vec4> l_frustumsCornerPos;
	//2.1 first 4 corner
	for (size_t i = 0; i < 4; i++)
	{
		l_frustumsCornerPos.emplace_back(l_frustumVertices[i].m_pos);
	}

	//2.2 other 16 corner based on e^2i / e^8
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			auto scaleFactor = std::exp(((double)i + 1.0) * E<double>) / std::exp(E<double> * 4.0);
			auto l_splitedPlaneCornerPos = l_frustumVertices[j].m_pos + (l_frustumVertices[j + 4].m_pos - l_frustumVertices[j].m_pos) * scaleFactor;
			l_frustumsCornerPos.emplace_back(l_splitedPlaneCornerPos);
		}
	}

	//3.assemble splited frustums
	std::vector<Vertex> l_frustumsCornerVertices;
	auto l_NDC = generateNDC();
	for (size_t i = 0; i < 4; i++)
	{
		l_frustumsCornerVertices.insert(l_frustumsCornerVertices.end(), l_NDC.begin(), l_NDC.end());
	}
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 8; j++)
		{
			l_frustumsCornerVertices[i * 8 + j].m_pos = l_frustumsCornerPos[i * 4 + j];
		}
	}
	std::vector<std::vector<Vertex>> l_splitedFrustums;
	for (size_t i = 0; i < 4; i++)
	{
		l_splitedFrustums.emplace_back(std::vector<Vertex>(l_frustumsCornerVertices.begin() + i * 8, l_frustumsCornerVertices.begin() + 8 + i * 8));
	}

	//4.generate AABBs for the splited frustums
	for (size_t i = 0; i < 4; i++)
	{
		lightComponent.m_AABBs.emplace_back(generateAABB(l_splitedFrustums[i]));
	}
}

void PhysicsSystem::generateAABB(CameraComponent & cameraComponent)
{
	auto l_frustumCorners = cameraComponent.m_frustumVertices;
	cameraComponent.m_AABB = generateAABB(l_frustumCorners);
}

AABB PhysicsSystem::generateAABB(const std::vector<Vertex>& vertices)
{
	double maxX = vertices[0].m_pos.x;
	double maxY = vertices[0].m_pos.y;
	double maxZ = vertices[0].m_pos.z;
	double minX = vertices[0].m_pos.x;
	double minY = vertices[0].m_pos.y;
	double minZ = vertices[0].m_pos.z;

	for (auto& l_vertexData : vertices)
	{
		if (l_vertexData.m_pos.x >= maxX)
		{
			maxX = l_vertexData.m_pos.x;
		}
		if (l_vertexData.m_pos.y >= maxY)
		{
			maxY = l_vertexData.m_pos.y;
		}
		if (l_vertexData.m_pos.z >= maxZ)
		{
			maxZ = l_vertexData.m_pos.z;
		}
		if (l_vertexData.m_pos.x <= minX)
		{
			minX = l_vertexData.m_pos.x;
		}
		if (l_vertexData.m_pos.y <= minY)
		{
			minY = l_vertexData.m_pos.y;
		}
		if (l_vertexData.m_pos.z <= minZ)
		{
			minZ = l_vertexData.m_pos.z;
		}
	}

	return generateAABB(vec4(maxX, maxY, maxZ, 1.0), vec4(minX, minY, minZ, 1.0));
}

AABB PhysicsSystem::generateAABB(const vec4 & boundMax, const vec4 & boundMin)
{
	AABB l_AABB;

	l_AABB.m_boundMin = boundMin;
	l_AABB.m_boundMax = boundMax;

	l_AABB.m_center = (l_AABB.m_boundMax + l_AABB.m_boundMin) * 0.5;
	l_AABB.m_sphereRadius = std::max<double>(std::max<double>((l_AABB.m_boundMax.x - l_AABB.m_boundMin.x) / 2.0, (l_AABB.m_boundMax.y - l_AABB.m_boundMin.y) / 2.0), (l_AABB.m_boundMax.z - l_AABB.m_boundMin.z) / 2.0);

	Vertex l_VertexData_1;
	l_VertexData_1.m_pos = (vec4(boundMax.x, boundMax.y, boundMax.z, 1.0));
	l_VertexData_1.m_texCoord = vec2(1.0, 1.0);

	Vertex l_VertexData_2;
	l_VertexData_2.m_pos = (vec4(boundMax.x, boundMin.y, boundMax.z, 1.0));
	l_VertexData_2.m_texCoord = vec2(1.0, 0.0);

	Vertex l_VertexData_3;
	l_VertexData_3.m_pos = (vec4(boundMin.x, boundMin.y, boundMax.z, 1.0));
	l_VertexData_3.m_texCoord = vec2(0.0, 0.0);

	Vertex l_VertexData_4;
	l_VertexData_4.m_pos = (vec4(boundMin.x, boundMax.y, boundMax.z, 1.0));
	l_VertexData_4.m_texCoord = vec2(0.0, 1.0);

	Vertex l_VertexData_5;
	l_VertexData_5.m_pos = (vec4(boundMax.x, boundMax.y, boundMin.z, 1.0));
	l_VertexData_5.m_texCoord = vec2(1.0, 1.0);

	Vertex l_VertexData_6;
	l_VertexData_6.m_pos = (vec4(boundMax.x, boundMin.y, boundMin.z, 1.0));
	l_VertexData_6.m_texCoord = vec2(1.0, 0.0);

	Vertex l_VertexData_7;
	l_VertexData_7.m_pos = (vec4(boundMin.x, boundMin.y, boundMin.z, 1.0));
	l_VertexData_7.m_texCoord = vec2(0.0, 0.0);

	Vertex l_VertexData_8;
	l_VertexData_8.m_pos = (vec4(boundMin.x, boundMax.y, boundMin.z, 1.0));
	l_VertexData_8.m_texCoord = vec2(0.0, 1.0);


	l_AABB.m_vertices = { l_VertexData_1, l_VertexData_2, l_VertexData_3, l_VertexData_4, l_VertexData_5, l_VertexData_6, l_VertexData_7, l_VertexData_8 };

	for (auto& l_vertexData : l_AABB.m_vertices)
	{
		l_vertexData.m_normal = vec4(l_vertexData.m_pos.x, l_vertexData.m_pos.y, l_vertexData.m_pos.z, 0.0).normalize();
	}

	l_AABB.m_indices = { 0, 1, 3, 1, 2, 3,
		4, 5, 0, 5, 1, 0,
		7, 6, 4, 6, 5, 4,
		3, 2, 7, 2, 6 ,7,
		4, 0, 7, 0, 3, 7,
		1, 5, 2, 5, 6, 2 };

	return l_AABB;
}

void PhysicsSystem::initialize()
{
	setupComponents();
	m_objectStatus = objectStatus::ALIVE;
	g_pLogSystem->printLog("PhysicsSystem has been initialized.");
}

void PhysicsSystem::updateCameraComponents()
{
	if (g_pGameSystem->getCameraComponents().size() > 0)
	{
		for (auto& i : g_pGameSystem->getCameraComponents())
		{
			setupCameraComponentRayOfEye(i);
			setupCameraComponentFrustumVertices(i);
		}

		generateAABB(*g_pGameSystem->getCameraComponents()[0]);
	}
}

void PhysicsSystem::updateLightComponents()
{
	if (g_pGameSystem->getLightComponents().size() > 0)
	{
		// generate AABB for CSM
		for (auto& i : g_pGameSystem->getLightComponents())
		{
			setupLightComponentRadius(i);
			if (i->m_lightType == lightType::DIRECTIONAL)
			{
				generateAABB(*i);
			}
		}
	}
}

void PhysicsSystem::update()
{
	updateCameraComponents();
	updateLightComponents();
}

void PhysicsSystem::shutdown()
{
	m_objectStatus = objectStatus::SHUTDOWN;
	g_pLogSystem->printLog("PhysicsSystem has been shutdown.");
}
