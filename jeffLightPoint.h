#pragma once

#include "jeffLight.h"

namespace jeffNamespace
{
	class jeffLightPoint : public jeffLight
	{
	public:
		// Quadratic attenuation, linear attenuation, constant attenuation
		DirectX::XMFLOAT4 lightParams = DirectX::XMFLOAT4(1.0f, 0.6f, 0.2f, 0.0f);

		void handleKeyEvent(JEFF_KEY* key) override
		{
			if (*key == I)
			{
				lightColour.w += 0.2f;
			}
			if (*key == J)
			{
				transformPosition.x -= 0.5f;
			}
			if (*key == K)
			{
				lightColour.w -= 0.2f;
			}
			if (*key == L)
			{
				transformPosition.x += 0.5f;
			}
			if (*key == U)
			{
				lightParams.x -= 0.2f;
			}
			if (*key == O)
			{
				lightParams.x += 0.2f;
			}
			if (*key == W)
			{
				transformPosition.z += 0.5f;
			}
			if (*key == S)
			{
				transformPosition.z -= 0.5f;
			}
			if (*key == A)
			{
				transformPosition.y += 0.5f;
			}
			if (*key == D)
			{
				transformPosition.y -= 0.5f;
			}
		}

	};
}