#pragma once
#include "../Common/InnoObject.h"
#include "../Common/InnoMathHelper.h"

struct KeyData
{
	Mat4 m;
};

class AnimationDataComponent : public InnoComponent
{
public:
	static uint32_t GetTypeID() { return 10; };
	static char* GetTypeName() { return "AnimationDataComponent"; };

	float m_Duration = 0.0f;
	uint32_t m_NumChannels = 0;
	uint32_t m_NumTicks = 0;
	// for each tick first, then for each channel
	std::vector<KeyData> m_KeyData;
};
