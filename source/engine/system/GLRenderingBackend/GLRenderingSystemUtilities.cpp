#include "GLRenderingSystemUtilities.h"

#include "../ICoreSystem.h"

extern ICoreSystem* g_pCoreSystem;

INNO_PRIVATE_SCOPE GLRenderingSystemNS
{
	void addRenderTargetTextures(GLRenderPassComponent* GLRPC, TextureDataDesc RTDesc, unsigned int colorAttachmentIndex);
	void setDrawBuffers(unsigned int RTNum);

	bool initializeGLMeshDataComponent(GLMeshDataComponent * rhs, const std::vector<Vertex>& vertices, const std::vector<Index>& indices);
	bool initializeGLTextureDataComponent(GLTextureDataComponent * rhs, TextureDataDesc textureDataDesc, const std::vector<void*>& textureData);

	GLTextureDataDesc getGLTextureDataDesc(const TextureDataDesc& textureDataDesc);

	GLenum getTextureSamplerType(TextureSamplerType rhs);
	GLenum getTextureWrapMethod(TextureWrapMethod rhs);
	GLenum getTextureFilterParam(TextureFilterMethod rhs);
	GLenum getTextureInternalFormat(TextureColorComponentsFormat rhs);
	GLenum getTexturePixelDataFormat(TexturePixelDataFormat rhs);
	GLenum getTexturePixelDataType(TexturePixelDataType rhs);

	std::unordered_map<EntityID, GLMeshDataComponent*> m_initializedGLMDC;
	std::unordered_map<EntityID, GLTextureDataComponent*> m_initializedGLTDC;

	std::unordered_map<EntityID, GLMeshDataComponent*> m_meshMap;
	std::unordered_map<EntityID, GLTextureDataComponent*> m_textureMap;

	void* m_GLMeshDataComponentPool;
	void* m_GLTextureDataComponentPool;
	void* m_GLRenderPassComponentPool;
	void* m_GLShaderProgramComponentPool;

	const std::string m_shaderRelativePath = std::string{ "..//res//shaders//" };
}

bool GLRenderingSystemNS::initializeComponentPool()
{
	m_GLMeshDataComponentPool = g_pCoreSystem->getMemorySystem()->allocateMemoryPool(sizeof(GLMeshDataComponent), 32768);
	m_GLTextureDataComponentPool = g_pCoreSystem->getMemorySystem()->allocateMemoryPool(sizeof(GLTextureDataComponent), 32768);
	m_GLRenderPassComponentPool = g_pCoreSystem->getMemorySystem()->allocateMemoryPool(sizeof(GLRenderPassComponent), 128);
	m_GLShaderProgramComponentPool = g_pCoreSystem->getMemorySystem()->allocateMemoryPool(sizeof(GLShaderProgramComponent), 256);

	return true;
}

GLRenderPassComponent* GLRenderingSystemNS::addGLRenderPassComponent(unsigned int RTNum, GLFrameBufferDesc glFrameBufferDesc, TextureDataDesc RTDesc)
{
	auto l_rawPtr = g_pCoreSystem->getMemorySystem()->spawnObject(m_GLRenderPassComponentPool, sizeof(GLRenderPassComponent));
	auto l_GLRPC = new(l_rawPtr)GLRenderPassComponent();

	// generate and bind framebuffer
	l_GLRPC->m_GLFrameBufferDesc = glFrameBufferDesc;

	glGenFramebuffers(1, &l_GLRPC->m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, l_GLRPC->m_FBO);

	// generate and bind renderbuffer
	glGenRenderbuffers(1, &l_GLRPC->m_RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, l_GLRPC->m_RBO);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, l_GLRPC->m_GLFrameBufferDesc.renderBufferAttachmentType, GL_RENDERBUFFER, l_GLRPC->m_RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, l_GLRPC->m_GLFrameBufferDesc.renderBufferInternalFormat, l_GLRPC->m_GLFrameBufferDesc.sizeX, l_GLRPC->m_GLFrameBufferDesc.sizeY);

	// generate and bind texture
	l_GLRPC->m_TDCs.reserve(RTNum);

	for (unsigned int i = 0; i < RTNum; i++)
	{
		auto l_TDC = g_pCoreSystem->getAssetSystem()->addTextureDataComponent();

		l_TDC->m_textureDataDesc = RTDesc;

		if (RTDesc.textureSamplerType == TextureSamplerType::CUBEMAP)
		{
			l_TDC->m_textureData = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
		}
		else
		{
			l_TDC->m_textureData = { nullptr };
		}

		l_GLRPC->m_TDCs.emplace_back(l_TDC);
	}

	l_GLRPC->m_GLTDCs.reserve(RTNum);

	for (unsigned int i = 0; i < RTNum; i++)
	{
		auto l_TDC = l_GLRPC->m_TDCs[i];
		auto l_GLTDC = generateGLTextureDataComponent(l_TDC);

		if (RTDesc.textureSamplerType == TextureSamplerType::SAMPLER_2D)
		{
			if (RTDesc.texturePixelDataFormat == TexturePixelDataFormat::DEPTH_COMPONENT)
			{
				attach2DDepthRT(
					l_GLTDC,
					l_GLRPC
				);
			}
			else
			{
				attach2DColorRT(
					l_GLTDC,
					l_GLRPC,
					i
				);
			}
		}
		else if (RTDesc.textureSamplerType == TextureSamplerType::SAMPLER_3D)
		{
			if (RTDesc.texturePixelDataFormat == TexturePixelDataFormat::DEPTH_COMPONENT)
			{
				g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_WARNING, "GLRenderingSystem: 3D depth only render target is not supported now");
			}
			else
			{
				attach3DColorRT(
					l_GLTDC,
					l_GLRPC,
					i,
					0
				);
			}
		}

		l_GLRPC->m_GLTDCs.emplace_back(l_GLTDC);
	}

	if (glFrameBufferDesc.drawColorBuffers)
	{
		std::vector<unsigned int> l_colorAttachments;
		for (unsigned int i = 0; i < RTNum; ++i)
		{
			l_colorAttachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
		}
		glDrawBuffers((GLsizei)l_colorAttachments.size(), &l_colorAttachments[0]);
	}
	else
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	// finally check if framebuffer is complete
	auto l_result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (l_result != GL_FRAMEBUFFER_COMPLETE)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: Framebuffer is not completed: " + std::to_string(l_result));
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return l_GLRPC;
}

void GLRenderingSystemNS::addRenderTargetTextures(GLRenderPassComponent* GLRPC, TextureDataDesc RTDesc, unsigned int colorAttachmentIndex)
{
	auto l_TDC = g_pCoreSystem->getAssetSystem()->addTextureDataComponent();

	l_TDC->m_textureDataDesc = RTDesc;
	l_TDC->m_textureData = { nullptr };

	GLRPC->m_TDCs.emplace_back(l_TDC);

	auto l_GLTDC = generateGLTextureDataComponent(l_TDC);

	attach2DColorRT(
		l_GLTDC,
		GLRPC,
		colorAttachmentIndex
	);

	GLRPC->m_GLTDCs.emplace_back(l_GLTDC);
}

void GLRenderingSystemNS::setDrawBuffers(unsigned int RTNum)
{
	std::vector<unsigned int> l_colorAttachments;
	for (unsigned int i = 0; i < RTNum; ++i)
	{
		l_colorAttachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);
	}
	glDrawBuffers((GLsizei)l_colorAttachments.size(), &l_colorAttachments[0]);
}

bool GLRenderingSystemNS::resizeGLRenderPassComponent(GLRenderPassComponent * GLRPC, GLFrameBufferDesc glFrameBufferDesc)
{
	GLRPC->m_GLFrameBufferDesc = glFrameBufferDesc;

	glBindFramebuffer(GL_FRAMEBUFFER, GLRPC->m_FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, GLRPC->m_RBO);

	for (unsigned int i = 0; i < GLRPC->m_GLTDCs.size(); i++)
	{
		GLRPC->m_TDCs[i]->m_textureDataDesc.textureWidth = glFrameBufferDesc.sizeX;
		GLRPC->m_TDCs[i]->m_textureDataDesc.textureHeight = glFrameBufferDesc.sizeY;
		GLRPC->m_TDCs[i]->m_objectStatus = ObjectStatus::STANDBY;

		glDeleteTextures(1, &GLRPC->m_GLTDCs[i]->m_TAO);

		initializeGLTextureDataComponent(GLRPC->m_GLTDCs[i], GLRPC->m_TDCs[i]->m_textureDataDesc, GLRPC->m_TDCs[i]->m_textureData);

		if (glFrameBufferDesc.drawColorBuffers)
		{
			attach2DColorRT(
				GLRPC->m_GLTDCs[i],
				GLRPC,
				i
			);
		}
		else
		{
			// @TODO: it's not a binary classfication problem
			attach2DDepthRT(
				GLRPC->m_GLTDCs[i],
				GLRPC
			);
		}
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

GLShaderProgramComponent * GLRenderingSystemNS::addGLShaderProgramComponent(const EntityID& rhs)
{
	auto l_rawPtr = g_pCoreSystem->getMemorySystem()->spawnObject(m_GLShaderProgramComponentPool, sizeof(GLShaderProgramComponent));
	auto l_GLSPC = new(l_rawPtr)GLShaderProgramComponent();
	l_GLSPC->m_parentEntity = rhs;
	return l_GLSPC;
}

GLMeshDataComponent * GLRenderingSystemNS::addGLMeshDataComponent(const EntityID& rhs)
{
	auto l_rawPtr = g_pCoreSystem->getMemorySystem()->spawnObject(m_GLMeshDataComponentPool, sizeof(GLMeshDataComponent));
	auto l_GLMDC = new(l_rawPtr)GLMeshDataComponent();
	l_GLMDC->m_parentEntity = rhs;
	auto l_meshMap = &m_meshMap;
	l_meshMap->emplace(std::pair<EntityID, GLMeshDataComponent*>(rhs, l_GLMDC));
	return l_GLMDC;
}

GLTextureDataComponent * GLRenderingSystemNS::addGLTextureDataComponent(const EntityID& rhs)
{
	auto l_rawPtr = g_pCoreSystem->getMemorySystem()->spawnObject(m_GLTextureDataComponentPool, sizeof(GLTextureDataComponent));
	auto l_GLTDC = new(l_rawPtr)GLTextureDataComponent();
	l_GLTDC->m_parentEntity = rhs;
	auto l_textureMap = &m_textureMap;
	l_textureMap->emplace(std::pair<EntityID, GLTextureDataComponent*>(rhs, l_GLTDC));
	return l_GLTDC;
}

GLMeshDataComponent * GLRenderingSystemNS::getGLMeshDataComponent(const EntityID& rhs)
{
	auto result = m_meshMap.find(rhs);
	if (result != m_meshMap.end())
	{
		return result->second;
	}
	else
	{
		return nullptr;
	}
}

GLTextureDataComponent * GLRenderingSystemNS::getGLTextureDataComponent(const EntityID& rhs)
{
	auto result = m_textureMap.find(rhs);
	if (result != m_textureMap.end())
	{
		return result->second;
	}
	else
	{
		return nullptr;
	}
}

GLMeshDataComponent* GLRenderingSystemNS::generateGLMeshDataComponent(MeshDataComponent* rhs)
{
	if (rhs->m_objectStatus == ObjectStatus::ALIVE)
	{
		return getGLMeshDataComponent(rhs->m_parentEntity);
	}
	else
	{
		auto l_ptr = addGLMeshDataComponent(rhs->m_parentEntity);

		initializeGLMeshDataComponent(l_ptr, rhs->m_vertices, rhs->m_indices);

		rhs->m_objectStatus = ObjectStatus::ALIVE;

		return l_ptr;
	}
}

GLTextureDataComponent* GLRenderingSystemNS::generateGLTextureDataComponent(TextureDataComponent * rhs)
{
	if (rhs->m_objectStatus == ObjectStatus::ALIVE)
	{
		return getGLTextureDataComponent(rhs->m_parentEntity);
	}
	else
	{
		if (rhs->m_textureDataDesc.textureUsageType == TextureUsageType::INVISIBLE)
		{
			g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_WARNING, "GLRenderingSystem: try to generate GLTextureDataComponent for TextureUsageType::INVISIBLE type!");
			return nullptr;
		}
		else
		{
			if (rhs->m_textureData.size() > 0)
			{
				auto l_ptr = addGLTextureDataComponent(rhs->m_parentEntity);

				initializeGLTextureDataComponent(l_ptr, rhs->m_textureDataDesc, rhs->m_textureData);

				rhs->m_objectStatus = ObjectStatus::ALIVE;

				if (rhs->m_textureDataDesc.textureUsageType != TextureUsageType::RENDER_TARGET)
				{
					g_pCoreSystem->getAssetSystem()->releaseRawDataForTextureDataComponent(rhs->m_parentEntity);
				}

				return l_ptr;
			}
			else
			{
				g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_WARNING, "GLRenderingSystem: try to generate GLTextureDataComponent without raw data!");
				return nullptr;
			}
		}
	}
}

bool GLRenderingSystemNS::initializeGLShaderProgramComponent(GLShaderProgramComponent* rhs, const ShaderFilePaths& ShaderFilePaths)
{
	rhs->m_program = glCreateProgram();

	std::function<void(GLuint& shaderProgram, GLuint& shaderID, GLuint ShaderType, const std::string & shaderFilePath)> f_addShader =
		[&](GLuint& shaderProgram, GLuint& shaderID, GLuint shaderType, const std::string & shaderFilePath) {
		shaderID = glCreateShader(shaderType);

		if (shaderID == 0)
		{
			g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: Shader creation failed! memory location invaild when adding shader!");
			return;
		}

		auto l_shaderCodeContent = g_pCoreSystem->getFileSystem()->loadTextFile(m_shaderRelativePath + shaderFilePath);

		const char* l_sourcePointer = l_shaderCodeContent.c_str();

		if (l_sourcePointer == nullptr || l_shaderCodeContent.empty())
		{
			g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " loading failed!");
			return;
		}

		// compile shader
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_VERBOSE, "GLRenderingSystem: innoShader: " + shaderFilePath + " is compiling ...");

		glShaderSource(shaderID, 1, &l_sourcePointer, NULL);
		glCompileShader(shaderID);

		GLint l_validationResult = GL_FALSE;
		GLint l_infoLogLength = 0;
		GLint l_shaderFileLength = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &l_validationResult);

		if (!l_validationResult)
		{
			g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " compile failed!");
			glGetShaderiv(shaderID, GL_SHADER_SOURCE_LENGTH, &l_shaderFileLength);
			g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " file length is: " + std::to_string(l_shaderFileLength));
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &l_infoLogLength);

			if (l_infoLogLength > 0)
			{
				std::vector<char> l_shaderErrorMessage(l_infoLogLength + 1);
				glGetShaderInfoLog(shaderID, l_infoLogLength, NULL, &l_shaderErrorMessage[0]);
				g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " compile error: " + &l_shaderErrorMessage[0] + "\n -- --------------------------------------------------- -- ");
			}
			else
			{
				g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " compile error: no info log provided!");
			}

			return;
		}

		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_SUCCESS, "GLRenderingSystem: innoShader: " + shaderFilePath + " has been compiled.");

		// Link shader to program
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_VERBOSE, "GLRenderingSystem: innoShader: " + shaderFilePath + " is linking ...");

		glAttachShader(shaderProgram, shaderID);
		glLinkProgram(shaderProgram);
		glValidateProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &l_validationResult);
		if (!l_validationResult)
		{
			g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " link failed!");
			glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &l_infoLogLength);

			if (l_infoLogLength > 0) {
				std::vector<char> l_shaderErrorMessage(l_infoLogLength + 1);
				glGetProgramInfoLog(shaderProgram, l_infoLogLength, NULL, &l_shaderErrorMessage[0]);
				g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " link error: " + &l_shaderErrorMessage[0] + "\n -- --------------------------------------------------- -- ");
			}
			else
			{
				g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: " + shaderFilePath + " link error: no info log provided!");
			}

			return;
		}

		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_SUCCESS, "GLRenderingSystem: innoShader: " + shaderFilePath + " has been linked.");
	};

	if (!ShaderFilePaths.m_VSPath.empty())
	{
		f_addShader(rhs->m_program, rhs->m_VSID, GL_VERTEX_SHADER, ShaderFilePaths.m_VSPath);
	}
	if (!ShaderFilePaths.m_GSPath.empty())
	{
		f_addShader(rhs->m_program, rhs->m_GSID, GL_GEOMETRY_SHADER, ShaderFilePaths.m_GSPath);
	}
	if (!ShaderFilePaths.m_FSPath.empty())
	{
		f_addShader(rhs->m_program, rhs->m_FSID, GL_FRAGMENT_SHADER, ShaderFilePaths.m_FSPath);
	}
	if (!ShaderFilePaths.m_CSPath.empty())
	{
		f_addShader(rhs->m_program, rhs->m_CSID, GL_COMPUTE_SHADER, ShaderFilePaths.m_CSPath);
	}

	rhs->m_objectStatus = ObjectStatus::ALIVE;
	return rhs;
}

bool GLRenderingSystemNS::initializeGLMeshDataComponent(GLMeshDataComponent * rhs, const std::vector<Vertex>& vertices, const std::vector<Index>& indices)
{
	std::vector<float> l_verticesBuffer;
	auto l_containerSize = vertices.size() * 8;
	l_verticesBuffer.reserve(l_containerSize);

	std::for_each(vertices.begin(), vertices.end(), [&](Vertex val)
	{
		l_verticesBuffer.emplace_back((float)val.m_pos.x);
		l_verticesBuffer.emplace_back((float)val.m_pos.y);
		l_verticesBuffer.emplace_back((float)val.m_pos.z);
		l_verticesBuffer.emplace_back((float)val.m_texCoord.x);
		l_verticesBuffer.emplace_back((float)val.m_texCoord.y);
		l_verticesBuffer.emplace_back((float)val.m_normal.x);
		l_verticesBuffer.emplace_back((float)val.m_normal.y);
		l_verticesBuffer.emplace_back((float)val.m_normal.z);
	});

	glGenVertexArrays(1, &rhs->m_VAO);
	glGenBuffers(1, &rhs->m_VBO);
	glGenBuffers(1, &rhs->m_IBO);

	glBindVertexArray(rhs->m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, rhs->m_VBO);
	glBufferData(GL_ARRAY_BUFFER, l_verticesBuffer.size() * sizeof(float), &l_verticesBuffer[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rhs->m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// position attribute, 1st attribution with 3 * sizeof(float) bits of data
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	// texture attribute, 2nd attribution with 2 * sizeof(float) bits of data
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	// normal coord attribute, 3rd attribution with 3 * sizeof(float) bits of data
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

	rhs->m_objectStatus = ObjectStatus::ALIVE;

	m_initializedGLMDC.emplace(rhs->m_parentEntity, rhs);

	g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_VERBOSE, "GLRenderingSystem: VAO " + std::to_string(rhs->m_VAO) + " is initialized.");

	return true;
}

bool GLRenderingSystemNS::initializeGLTextureDataComponent(GLTextureDataComponent * rhs, TextureDataDesc textureDataDesc, const std::vector<void*>& textureData)
{
	rhs->m_GLTextureDataDesc = getGLTextureDataDesc(textureDataDesc);

	//generate and bind texture object
	glGenTextures(1, &rhs->m_TAO);

	glBindTexture(rhs->m_GLTextureDataDesc.textureSamplerType, rhs->m_TAO);

	glTexParameteri(rhs->m_GLTextureDataDesc.textureSamplerType, GL_TEXTURE_WRAP_R, rhs->m_GLTextureDataDesc.textureWrapMethod);
	glTexParameteri(rhs->m_GLTextureDataDesc.textureSamplerType, GL_TEXTURE_WRAP_S, rhs->m_GLTextureDataDesc.textureWrapMethod);
	glTexParameteri(rhs->m_GLTextureDataDesc.textureSamplerType, GL_TEXTURE_WRAP_T, rhs->m_GLTextureDataDesc.textureWrapMethod);

	glTexParameterfv(rhs->m_GLTextureDataDesc.textureSamplerType, GL_TEXTURE_BORDER_COLOR, rhs->m_GLTextureDataDesc.boardColor);

	glTexParameteri(rhs->m_GLTextureDataDesc.textureSamplerType, GL_TEXTURE_MIN_FILTER, rhs->m_GLTextureDataDesc.minFilterParam);
	glTexParameteri(rhs->m_GLTextureDataDesc.textureSamplerType, GL_TEXTURE_MAG_FILTER, rhs->m_GLTextureDataDesc.magFilterParam);

	if (rhs->m_GLTextureDataDesc.textureSamplerType == GL_TEXTURE_1D)
	{
		glTexImage1D(GL_TEXTURE_1D, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[0]);
	}
	else if (rhs->m_GLTextureDataDesc.textureSamplerType == GL_TEXTURE_2D)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[0]);
	}
	else if (rhs->m_GLTextureDataDesc.textureSamplerType == GL_TEXTURE_3D)
	{
		glTexImage3D(GL_TEXTURE_3D, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, textureDataDesc.textureDepth, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[0]);
	}
	else if (rhs->m_GLTextureDataDesc.textureSamplerType == GL_TEXTURE_CUBE_MAP)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[0]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[1]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[2]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[3]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[4]);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, rhs->m_GLTextureDataDesc.internalFormat, textureDataDesc.textureWidth, textureDataDesc.textureHeight, 0, rhs->m_GLTextureDataDesc.pixelDataFormat, rhs->m_GLTextureDataDesc.pixelDataType, textureData[5]);
	}

	// should generate mipmap or not
	if (rhs->m_GLTextureDataDesc.minFilterParam == GL_LINEAR_MIPMAP_LINEAR)
	{
		glGenerateMipmap(rhs->m_GLTextureDataDesc.textureSamplerType);
	}

	rhs->m_objectStatus = ObjectStatus::ALIVE;

	m_initializedGLTDC.emplace(rhs->m_parentEntity, rhs);

	g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_VERBOSE, "GLRenderingSystem: TAO " + std::to_string(rhs->m_TAO) + " is initialized.");

	return true;
}

GLenum GLRenderingSystemNS::getTextureSamplerType(TextureSamplerType rhs)
{
	switch (rhs)
	{
	case TextureSamplerType::SAMPLER_1D:
		return GL_TEXTURE_1D;
	case TextureSamplerType::SAMPLER_2D:
		return GL_TEXTURE_2D;
	case TextureSamplerType::SAMPLER_3D:
		return GL_TEXTURE_3D;
	case TextureSamplerType::CUBEMAP:
		return GL_TEXTURE_CUBE_MAP;
	default:
		return 0;
	}
}

GLenum GLRenderingSystemNS::getTextureWrapMethod(TextureWrapMethod rhs)
{
	GLenum result;

	switch (rhs)
	{
	case TextureWrapMethod::CLAMP_TO_EDGE: result = GL_CLAMP_TO_EDGE; break;
	case TextureWrapMethod::REPEAT: result = GL_REPEAT; break;
	case TextureWrapMethod::CLAMP_TO_BORDER: result = GL_CLAMP_TO_BORDER; break;
	}

	return result;
}

GLenum GLRenderingSystemNS::getTextureFilterParam(TextureFilterMethod rhs)
{
	GLenum result;

	switch (rhs)
	{
	case TextureFilterMethod::NEAREST: result = GL_NEAREST; break;
	case TextureFilterMethod::LINEAR: result = GL_LINEAR; break;
	case TextureFilterMethod::LINEAR_MIPMAP_LINEAR: result = GL_LINEAR_MIPMAP_LINEAR; break;
	}

	return result;
}

GLenum GLRenderingSystemNS::getTextureInternalFormat(TextureColorComponentsFormat rhs)
{
	GLenum result;

	switch (rhs)
	{
	case TextureColorComponentsFormat::RED: result = GL_RED; break;
	case TextureColorComponentsFormat::RG: result = GL_RG; break;
	case TextureColorComponentsFormat::RGB: result = GL_RGB; break;
	case TextureColorComponentsFormat::RGBA: result = GL_RGBA; break;

	case TextureColorComponentsFormat::R8: result = GL_R8; break;
	case TextureColorComponentsFormat::RG8: result = GL_RG8; break;
	case TextureColorComponentsFormat::RGB8: result = GL_RGB8; break;
	case TextureColorComponentsFormat::RGBA8: result = GL_RGBA8; break;

	case TextureColorComponentsFormat::R8I: result = GL_R8I; break;
	case TextureColorComponentsFormat::RG8I: result = GL_RG8I; break;
	case TextureColorComponentsFormat::RGB8I: result = GL_RGB8I; break;
	case TextureColorComponentsFormat::RGBA8I: result = GL_RGBA8I; break;

	case TextureColorComponentsFormat::R8UI: result = GL_R8UI; break;
	case TextureColorComponentsFormat::RG8UI: result = GL_RG8UI; break;
	case TextureColorComponentsFormat::RGB8UI: result = GL_RGB8UI; break;
	case TextureColorComponentsFormat::RGBA8UI: result = GL_RGBA8UI; break;

	case TextureColorComponentsFormat::R16: result = GL_R16; break;
	case TextureColorComponentsFormat::RG16: result = GL_RG16; break;
	case TextureColorComponentsFormat::RGB16: result = GL_RGB16; break;
	case TextureColorComponentsFormat::RGBA16: result = GL_RGBA16; break;

	case TextureColorComponentsFormat::R16I: result = GL_R16I; break;
	case TextureColorComponentsFormat::RG16I: result = GL_RG16I; break;
	case TextureColorComponentsFormat::RGB16I: result = GL_RGB16I; break;
	case TextureColorComponentsFormat::RGBA16I: result = GL_RGBA16I; break;

	case TextureColorComponentsFormat::R16UI: result = GL_R16UI; break;
	case TextureColorComponentsFormat::RG16UI: result = GL_RG16UI; break;
	case TextureColorComponentsFormat::RGB16UI: result = GL_RGB16UI; break;
	case TextureColorComponentsFormat::RGBA16UI: result = GL_RGBA16UI; break;

	case TextureColorComponentsFormat::R16F: result = GL_R16F; break;
	case TextureColorComponentsFormat::RG16F: result = GL_RG16F; break;
	case TextureColorComponentsFormat::RGB16F: result = GL_RGB16F; break;
	case TextureColorComponentsFormat::RGBA16F: result = GL_RGBA16F; break;

	case TextureColorComponentsFormat::R32I: result = GL_R32I; break;
	case TextureColorComponentsFormat::RG32I: result = GL_RG32I; break;
	case TextureColorComponentsFormat::RGB32I: result = GL_RGB32I; break;
	case TextureColorComponentsFormat::RGBA32I: result = GL_RGBA32I; break;

	case TextureColorComponentsFormat::R32UI: result = GL_R32UI; break;
	case TextureColorComponentsFormat::RG32UI: result = GL_RG32UI; break;
	case TextureColorComponentsFormat::RGB32UI: result = GL_RGB32UI; break;
	case TextureColorComponentsFormat::RGBA32UI: result = GL_RGBA32UI; break;

	case TextureColorComponentsFormat::R32F: result = GL_R32F; break;
	case TextureColorComponentsFormat::RG32F: result = GL_RG32F; break;
	case TextureColorComponentsFormat::RGB32F: result = GL_RGB32F; break;
	case TextureColorComponentsFormat::RGBA32F: result = GL_RGBA32F; break;

	case TextureColorComponentsFormat::SRGB: result = GL_SRGB; break;
	case TextureColorComponentsFormat::SRGBA: result = GL_SRGB_ALPHA; break;
	case TextureColorComponentsFormat::SRGB8: result = GL_SRGB8; break;
	case TextureColorComponentsFormat::SRGBA8: result = GL_SRGB8_ALPHA8; break;

	case TextureColorComponentsFormat::DEPTH_COMPONENT: result = GL_DEPTH_COMPONENT; break;
	}

	return result;
}

GLenum GLRenderingSystemNS::getTexturePixelDataFormat(TexturePixelDataFormat rhs)
{
	GLenum result;

	switch (rhs)
	{
	case TexturePixelDataFormat::RED:result = GL_RED; break;
	case TexturePixelDataFormat::RG:result = GL_RG; break;
	case TexturePixelDataFormat::RGB:result = GL_RGB; break;
	case TexturePixelDataFormat::RGBA:result = GL_RGBA; break;
	case TexturePixelDataFormat::RED_INT:result = GL_RED_INTEGER; break;
	case TexturePixelDataFormat::RG_INT:result = GL_RG_INTEGER; break;
	case TexturePixelDataFormat::RGB_INT:result = GL_RGB_INTEGER; break;
	case TexturePixelDataFormat::RGBA_INT:result = GL_RGBA_INTEGER; break;
	case TexturePixelDataFormat::DEPTH_COMPONENT:result = GL_DEPTH_COMPONENT; break;
	default:
		break;
	}

	return result;
}

GLenum GLRenderingSystemNS::getTexturePixelDataType(TexturePixelDataType rhs)
{
	GLenum result;

	switch (rhs)
	{
	case TexturePixelDataType::UNSIGNED_BYTE:result = GL_UNSIGNED_BYTE; break;
	case TexturePixelDataType::BYTE:result = GL_BYTE; break;
	case TexturePixelDataType::UNSIGNED_SHORT:result = GL_UNSIGNED_SHORT; break;
	case TexturePixelDataType::SHORT:result = GL_SHORT; break;
	case TexturePixelDataType::UNSIGNED_INT:result = GL_UNSIGNED_INT; break;
	case TexturePixelDataType::INT:result = GL_INT; break;
	case TexturePixelDataType::FLOAT:result = GL_FLOAT; break;
	case TexturePixelDataType::DOUBLE:result = GL_FLOAT; break;
	}

	return result;
}

GLTextureDataDesc GLRenderingSystemNS::getGLTextureDataDesc(const TextureDataDesc& textureDataDesc)
{
	GLTextureDataDesc l_result;

	l_result.textureSamplerType = getTextureSamplerType(textureDataDesc.textureSamplerType);

	l_result.textureWrapMethod = getTextureWrapMethod(textureDataDesc.textureWrapMethod);

	l_result.minFilterParam = getTextureFilterParam(textureDataDesc.textureMinFilterMethod);
	l_result.magFilterParam = getTextureFilterParam(textureDataDesc.textureMagFilterMethod);

	// set texture formats
	if (textureDataDesc.textureUsageType == TextureUsageType::ALBEDO)
	{
		if (textureDataDesc.texturePixelDataFormat == TexturePixelDataFormat::RGB)
		{
			l_result.internalFormat = GL_SRGB;
		}
		else if (textureDataDesc.texturePixelDataFormat == TexturePixelDataFormat::RGBA)
		{
			l_result.internalFormat = GL_SRGB_ALPHA;
		}
	}
	else
	{
		l_result.internalFormat = getTextureInternalFormat(textureDataDesc.textureColorComponentsFormat);
	}

	l_result.pixelDataFormat = getTexturePixelDataFormat(textureDataDesc.texturePixelDataFormat);

	l_result.pixelDataType = getTexturePixelDataType(textureDataDesc.texturePixelDataType);

	l_result.boardColor[0] = textureDataDesc.borderColor.x;
	l_result.boardColor[1] = textureDataDesc.borderColor.y;
	l_result.boardColor[2] = textureDataDesc.borderColor.z;
	l_result.boardColor[3] = textureDataDesc.borderColor.w;

	return l_result;
}

bool GLRenderingSystemNS::deleteShaderProgram(GLShaderProgramComponent* rhs)
{
	if (rhs->m_VSID)
	{
		glDetachShader(rhs->m_program, rhs->m_VSID);
		glDeleteShader(rhs->m_VSID);
	}
	if (rhs->m_GSID)
	{
		glDetachShader(rhs->m_program, rhs->m_GSID);
		glDeleteShader(rhs->m_GSID);
	}
	if (rhs->m_FSID)
	{
		glDetachShader(rhs->m_program, rhs->m_FSID);
		glDeleteShader(rhs->m_FSID);
	}

	glDeleteProgram(rhs->m_program);

	g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_DEV_SUCCESS, "GLRenderingSystem: innoShader: shader deleted.");

	return true;
}

GLuint GLRenderingSystemNS::getUniformLocation(GLuint shaderProgram, const std::string & uniformName)
{
	glUseProgram(shaderProgram);
	int uniformLocation = glGetUniformLocation(shaderProgram, uniformName.c_str());
	if (uniformLocation == 0xFFFFFFFF)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: Uniform lost: " + uniformName);
		return -1;
	}
	return uniformLocation;
}

GLuint GLRenderingSystemNS::getUniformBlockIndex(GLuint shaderProgram, const std::string & uniformBlockName)
{
	glUseProgram(shaderProgram);
	auto uniformBlockIndex = glGetUniformBlockIndex(shaderProgram, uniformBlockName.c_str());
	if (uniformBlockIndex == 0xFFFFFFFF)
	{
		g_pCoreSystem->getLogSystem()->printLog(LogType::INNO_ERROR, "GLRenderingSystem: innoShader: Uniform Block lost: " + uniformBlockName);
		return -1;
	}
	return uniformBlockIndex;
}

GLuint GLRenderingSystemNS::generateUBO(GLuint UBOSize)
{
	GLuint l_ubo;
	glGenBuffers(1, &l_ubo);
	glBindBuffer(GL_UNIFORM_BUFFER, l_ubo);
	glBufferData(GL_UNIFORM_BUFFER, UBOSize, NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	return l_ubo;
}

void GLRenderingSystemNS::bindUniformBlock(GLuint UBO, GLuint UBOSize, GLuint program, const std::string & uniformBlockName, GLuint uniformBlockBindingPoint)
{
	auto uniformBlockIndex = getUniformBlockIndex(program, uniformBlockName.c_str());
	glUniformBlockBinding(program, uniformBlockIndex, uniformBlockBindingPoint);
	glBindBufferRange(GL_UNIFORM_BUFFER, uniformBlockIndex, UBO, 0, UBOSize);
}

void GLRenderingSystemNS::updateTextureUniformLocations(GLuint program, const std::vector<std::string>& UniformNames)
{
	for (size_t i = 0; i < UniformNames.size(); i++)
	{
		updateUniform(getUniformLocation(program, UniformNames[i]), (int)i);
	}
}

void GLRenderingSystemNS::updateUBOImpl(const GLint & UBO, size_t size, const void * UBOValue)
{
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, size, UBOValue);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, bool uniformValue)
{
	glUniform1i(uniformLocation, (int)uniformValue);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, int uniformValue)
{
	glUniform1i(uniformLocation, uniformValue);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, unsigned int uniformValue)
{
	glUniform1ui(uniformLocation, uniformValue);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, float uniformValue)
{
	glUniform1f(uniformLocation, (GLfloat)uniformValue);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, float x, float y)
{
	glUniform2f(uniformLocation, (GLfloat)x, (GLfloat)y);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, float x, float y, float z)
{
	glUniform3f(uniformLocation, (GLfloat)x, (GLfloat)y, (GLfloat)z);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, float x, float y, float z, float w)
{
	glUniform4f(uniformLocation, (GLfloat)x, (GLfloat)y, (GLfloat)z, (GLfloat)w);
}

void GLRenderingSystemNS::updateUniform(const GLint uniformLocation, const mat4 & mat)
{
#ifdef USE_COLUMN_MAJOR_MEMORY_LAYOUT
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, &mat.m00);
#endif
#ifdef USE_ROW_MAJOR_MEMORY_LAYOUT
	glUniformMatrix4fv(uniformLocation, 1, GL_TRUE, &mat.m00);
#endif
}

void GLRenderingSystemNS::attach2DDepthRT(GLTextureDataComponent * GLTDC, GLRenderPassComponent * GLRPC)
{
	glBindFramebuffer(GL_FRAMEBUFFER, GLRPC->m_FBO);
	glBindTexture(GLTDC->m_GLTextureDataDesc.textureSamplerType, GLTDC->m_TAO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GLTDC->m_TAO, 0);
}

void GLRenderingSystemNS::attachCubemapDepthRT(GLTextureDataComponent * GLTDC, GLRenderPassComponent * GLRPC, unsigned int textureIndex, unsigned int mipLevel)
{
	glBindFramebuffer(GL_FRAMEBUFFER, GLRPC->m_FBO);
	glBindTexture(GLTDC->m_GLTextureDataDesc.textureSamplerType, GLTDC->m_TAO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X + textureIndex, GLTDC->m_TAO, mipLevel);
}

void GLRenderingSystemNS::attach2DColorRT(GLTextureDataComponent * GLTDC, GLRenderPassComponent * GLRPC, unsigned int colorAttachmentIndex)
{
	glBindFramebuffer(GL_FRAMEBUFFER, GLRPC->m_FBO);
	glBindTexture(GLTDC->m_GLTextureDataDesc.textureSamplerType, GLTDC->m_TAO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_2D, GLTDC->m_TAO, 0);
}

void GLRenderingSystemNS::attach3DColorRT(GLTextureDataComponent * GLTDC, GLRenderPassComponent * GLRPC, unsigned int colorAttachmentIndex, unsigned int layer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, GLRPC->m_FBO);
	glBindTexture(GLTDC->m_GLTextureDataDesc.textureSamplerType, GLTDC->m_TAO);
	glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_3D, GLTDC->m_TAO, 0, layer);
}

void GLRenderingSystemNS::attachCubemapColorRT(GLTextureDataComponent * GLTDC, GLRenderPassComponent * GLRPC, unsigned int colorAttachmentIndex, unsigned int textureIndex, unsigned int mipLevel)
{
	glBindFramebuffer(GL_FRAMEBUFFER, GLRPC->m_FBO);
	glBindTexture(GLTDC->m_GLTextureDataDesc.textureSamplerType, GLTDC->m_TAO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + colorAttachmentIndex, GL_TEXTURE_CUBE_MAP_POSITIVE_X + textureIndex, GLTDC->m_TAO, mipLevel);
}

void GLRenderingSystemNS::activateShaderProgram(GLShaderProgramComponent * GLShaderProgramComponent)
{
	glUseProgram(GLShaderProgramComponent->m_program);
}

void GLRenderingSystemNS::drawMesh(const EntityID& rhs)
{
	auto l_MDC = g_pCoreSystem->getAssetSystem()->getMeshDataComponent(rhs);
	if (l_MDC)
	{
		drawMesh(l_MDC);
	}
}

void GLRenderingSystemNS::drawMesh(MeshDataComponent* MDC)
{
	auto l_GLMDC = getGLMeshDataComponent(MDC->m_parentEntity);
	if (l_GLMDC)
	{
		drawMesh(MDC->m_indicesSize, MDC->m_meshPrimitiveTopology, l_GLMDC);
	}
}

void GLRenderingSystemNS::drawMesh(size_t indicesSize, MeshPrimitiveTopology MeshPrimitiveTopology, GLMeshDataComponent* GLMDC)
{
	if (GLMDC->m_VAO)
	{
		glBindVertexArray(GLMDC->m_VAO);
		switch (MeshPrimitiveTopology)
		{
		case MeshPrimitiveTopology::POINT: glDrawElements(GL_POINTS, (GLsizei)indicesSize, GL_UNSIGNED_INT, 0); break;
		case MeshPrimitiveTopology::LINE: glDrawElements(GL_LINE, (GLsizei)indicesSize, GL_UNSIGNED_INT, 0); break;
		case MeshPrimitiveTopology::TRIANGLE: glDrawElements(GL_TRIANGLES, (GLsizei)indicesSize, GL_UNSIGNED_INT, 0); break;
		case MeshPrimitiveTopology::TRIANGLE_STRIP: glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)indicesSize, GL_UNSIGNED_INT, 0); break;
		default:
			break;
		}
	}
}

void GLRenderingSystemNS::activateTexture(TextureDataComponent * TDC, int activateIndex)
{
	auto l_GLTDC = getGLTextureDataComponent(TDC->m_parentEntity);
	activateTexture(l_GLTDC, activateIndex);
}

void GLRenderingSystemNS::activateTexture(GLTextureDataComponent * GLTDC, int activateIndex)
{
	glActiveTexture(GL_TEXTURE0 + activateIndex);
	glBindTexture(GLTDC->m_GLTextureDataDesc.textureSamplerType, GLTDC->m_TAO);
}

void GLRenderingSystemNS::activateRenderPass(GLRenderPassComponent* val)
{
	cleanFBC(val);

	glRenderbufferStorage(GL_RENDERBUFFER, val->m_GLFrameBufferDesc.renderBufferInternalFormat, val->m_GLFrameBufferDesc.sizeX, val->m_GLFrameBufferDesc.sizeY);
	glViewport(0, 0, val->m_GLFrameBufferDesc.sizeX, val->m_GLFrameBufferDesc.sizeY);
};

void GLRenderingSystemNS::cleanFBC(GLRenderPassComponent* val)
{
	glBindFramebuffer(GL_FRAMEBUFFER, val->m_FBO);
	glBindRenderbuffer(GL_RENDERBUFFER, val->m_RBO);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_STENCIL_BUFFER_BIT);
};

void GLRenderingSystemNS::copyDepthBuffer(GLRenderPassComponent* src, GLRenderPassComponent* dest)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->m_FBO);
	glBlitFramebuffer(0, 0, src->m_GLFrameBufferDesc.sizeX, src->m_GLFrameBufferDesc.sizeY, 0, 0, dest->m_GLFrameBufferDesc.sizeX, dest->m_GLFrameBufferDesc.sizeY, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
};

void GLRenderingSystemNS::copyStencilBuffer(GLRenderPassComponent* src, GLRenderPassComponent* dest)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->m_FBO);
	glBlitFramebuffer(0, 0, src->m_GLFrameBufferDesc.sizeX, src->m_GLFrameBufferDesc.sizeY, 0, 0, dest->m_GLFrameBufferDesc.sizeX, dest->m_GLFrameBufferDesc.sizeY, GL_STENCIL_BUFFER_BIT, GL_NEAREST);
};

void GLRenderingSystemNS::copyColorBuffer(GLRenderPassComponent* src, unsigned int srcIndex, GLRenderPassComponent* dest, unsigned int destIndex)
{
	std::vector<GLenum> drawBuffers = { GL_COLOR_ATTACHMENT0 + destIndex };
	glBindFramebuffer(GL_READ_FRAMEBUFFER, src->m_FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest->m_FBO);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + srcIndex);
	glDrawBuffers(1, &drawBuffers[0]);
	glBlitFramebuffer(0, 0, src->m_GLFrameBufferDesc.sizeX, src->m_GLFrameBufferDesc.sizeY, 0, 0, dest->m_GLFrameBufferDesc.sizeX, dest->m_GLFrameBufferDesc.sizeY, GL_COLOR_BUFFER_BIT, GL_NEAREST);
};