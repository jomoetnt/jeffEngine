#pragma once

#include "jeffLight.h"

namespace jeffNamespace
{
	class jeffLightDirectional : public jeffLight
	{
	public:
		void initObject() override
		{
			lightColour.w = 3.0f;
		}

		void handleKeyEvent(JEFF_KEY* key) override
		{
			if (*key == W)
			{
				transformRotation.z += 0.5f;
			}
			if (*key == A)
			{
				transformRotation.z -= 0.5f;
			}
			if (*key == S)
			{
				transformRotation.y -= 0.5f;
			}
			if (*key == D)
			{
				transformRotation.y += 0.5f;
			}
		}


	};
}
