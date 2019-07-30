#pragma once
#include "RenderPassDataComponent.h"
#include "../RenderingBackend/GLRenderingBackend/GLHeaders.h"

class GLPipelineStateObject : public IPipelineStateObject
{
public:
	std::deque<std::function<void()>> m_Activate;
	std::deque<std::function<void()>> m_Deactivate;
	GLenum m_GLPrimitiveTopology = 0;
};

class GLResourceBinder : public IResourceBinder
{
public:
	std::vector<void*> m_Resources;
};

class GLCommandList : public ICommandList
{
};

class GLCommandQueue : public ICommandQueue
{
};

class GLSemaphore : public ISemaphore
{
};

class GLFence : public IFence
{
};

class GLRenderPassDataComponent : public RenderPassDataComponent
{
public:
	GLuint m_FBO = 0;
	GLuint m_RBO = 0;

	GLenum m_renderBufferAttachmentType = 0;
	GLenum m_renderBufferInternalFormat = 0;
};