#pragma once

#include "jeffLight.h"

namespace jeffNamespace
{
	class jeffLightPoint : public jeffLight
	{
	public:
		// Quadratic attenuation, linear attenuation, constant attenuation
		DirectX::XMFLOAT4 lightParams = DirectX::XMFLOAT4(0.4f, 0.6f, 0.2f, 0.0f);

		void handleKeyEvent(JEFF_KEY* key) override
		{
			if (*key == I)
			{
				lightColour.w += 0.2f;
			}
			if (*key == J)
			{
				lightColour.w -= 0.2f;
			}
			if (*key == K)
			{
				lightParams.x -= 0.2f;
			}
			if (*key == L)
			{
				lightParams.x += 0.2f;
			}
			if (*key == U)
			{
				lightParams.z -= 0.2f;
			}
			if (*key == O)
			{
				lightParams.z += 0.2f;
			}
		}

	};
}