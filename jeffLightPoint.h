#pragma once

#include "jeffLight.h"

namespace jeffNamespace
{
	class jeffLightPoint : public jeffLight
	{
	public:
		// Quadratic attenuation, linear attenuation, constant attenuation
		DirectX::XMFLOAT4 lightParams = DirectX::XMFLOAT4(1.1f, 0.6f, 0.2f, 0.0f);

		float* mutableParams[4];
		int curParam = 0;

		jeffLightPoint(const char* lightName) : jeffLight::jeffLight(lightName) {}
		// temporary test
		void initObject() override
		{
			lightColour.w = 2.2f;
			transformPosition.z = -2.0f;
			
			mutableParams[0] = &lightParams.x; mutableParams[1] = &lightParams.y; mutableParams[2] = &lightParams.z;
			mutableParams[3] = &lightColour.w;
		}
		//temporary test
		void handleInputEvent(JEFF_KEY key, float* coords, bool keydown) override
		{
			switch (key)
			{
			case I:
				*mutableParams[curParam] += 0.1f;
				break;
			case K:
				*mutableParams[curParam] -= 0.1f;
				break;
			case L:
				curParam++;
				if (curParam >= 4)
					curParam = 0;
				break;
			case J:
				curParam--;
				if (curParam <= 0)
					curParam = 3;
				break;
			}
		}

	};
}