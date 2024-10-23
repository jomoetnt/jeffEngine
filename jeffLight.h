#pragma once

#include "jeffObject.h"

namespace jeffNamespace
{
	class jeffLight : public jeffObject
	{
	public:
		// RGB + intensity
		DirectX::XMFLOAT4 lightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);

		jeffLight(const char* lightName) : jeffObject::jeffObject(lightName)
		{
			dimension3 = true;
		}

		static DirectX::XMFLOAT4 threeToFour(DirectX::XMFLOAT3 in)
		{
			return DirectX::XMFLOAT4(in.x, in.y, in.z, 0.0f);
		}

		static DirectX::XMFLOAT4 threeToFour(DirectX::XMFLOAT3 in, float fourth)
		{
			return DirectX::XMFLOAT4(in.x, in.y, in.z, fourth);
		}

		static DirectX::XMFLOAT4 jeffClamp(DirectX::XMFLOAT4 in)
		{
			DirectX::XMVECTOR vec = DirectX::XMLoadFloat4(&in);

			vec = DirectX::XMVectorMax(DirectX::XMVectorZero(), vec);
			DirectX::XMStoreFloat4(&in, vec);

			return in;
		}


	};
}