#pragma once

#include "jeffObject.h"

namespace jeffNamespace
{
	class jeffLight : public jeffObject
	{
	public:
		enum JEFF_LIGHT_TYPE
		{
			DIRECTIONAL_LIGHT, POINT_LIGHT
		};

		// RGB + intensity
		DirectX::XMFLOAT4 lightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		// For point light: quadratic attenuation, linear attenuation, constant attenuation
		DirectX::XMFLOAT4 lightParams = DirectX::XMFLOAT4(0.4f, 0.6f, 0.2f, 0.0f);

		static DirectX::XMFLOAT4 threeToFour(DirectX::XMFLOAT3 in)
		{
			return DirectX::XMFLOAT4(in.x, in.y, in.z, 0.0f);
		}

		static DirectX::XMFLOAT4 threeToFour(DirectX::XMFLOAT3 in, float fourth)
		{
			return DirectX::XMFLOAT4(in.x, in.y, in.z, fourth);
		}

		void initObject() override
		{
			transformPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		}

		void handleKeyEvent(JEFF_KEY* key) override
		{
			if (*key == W)
			{
				transformPosition.y += 0.5f;
			}
			if (*key == A)
			{
				transformPosition.x -= 0.5f;
			}
			if (*key == S)
			{
				transformPosition.y -= 0.5f;
			}
			if (*key == D)
			{
				transformPosition.x += 0.5f;
			}
			if (*key == I)
			{
				lightParams.x += 0.2f;
			}
			if (*key == J)
			{
				lightParams.y -= 0.2f;
			}
			if (*key == K)
			{
				lightParams.x -= 0.2f;
			}
			if (*key == L)
			{
				lightParams.y += 0.2f;
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