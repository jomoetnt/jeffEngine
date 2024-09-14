#pragma once

#include "jeffLight.h"

namespace jeffNamespace
{
	class jeffLightPoint : public jeffLight
	{
	public:
		// Quadratic attenuation, linear attenuation, constant attenuation
		DirectX::XMFLOAT4 lightParams = DirectX::XMFLOAT4(1.0f, 0.6f, 0.2f, 0.0f);

		jeffLightPoint(const char* lightName) : jeffLight::jeffLight(lightName) {}
		// temporary test
		void initObject() override
		{
			lightColour.w = 1.2f;
			transformPosition.z = -2.0f;
		}

	};
}