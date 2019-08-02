#pragma once
#include "../../Engine/RenderingServer/IRenderingServer.h"

namespace FinalBlendPass
{
	bool Setup();
	bool Initialize();
	bool PrepareCommandList(RenderPassDataComponent* inputRPDC);

	ShaderProgramComponent* getSPC();
};
