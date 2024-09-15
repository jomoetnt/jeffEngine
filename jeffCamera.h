#pragma once

#include "jeffObject.h"

namespace jeffNamespace
{
	class jeffCamera : public jeffObject
	{
	public:
		float fovRad = DirectX::XM_PIDIV2;
		float nearDepth = 0.5f;
		float farDepth = 100.0f;
		float aspectRatio = 16.0f / 9.0f;

		DirectX::XMMATRIX projectionMatrix;

		jeffCamera(const char* camName) : jeffObject::jeffObject(camName)
		{
			setProjMat();
		}

		void handleInputEvent(JEFF_KEY key, float* coords, bool keydown) override
		{
			switch (key)
			{
			case UP: translateLocal(DirectX::XMFLOAT3(0.0f, 0.0f, 0.1f)); break;
			case DOWN: translateLocal(DirectX::XMFLOAT3(0.0f, 0.0f, -0.1f)); break;
			case LEFT: translateLocal(DirectX::XMFLOAT3(-0.1f, 0.0f, 0.0f)); break;
			case RIGHT: translateLocal(DirectX::XMFLOAT3(0.1f, 0.0f, 0.0f)); break;
			case LCTRL: transformPosition.y -= 0.1f; break;
			case SPACEBAR: transformPosition.y += 0.1f; break;
			case Z: transformRotation.y -= 0.1f; break;
			case X: transformRotation.y += 0.1f; break;
			}
		}

		void setProjMat()
		{
			projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearDepth, farDepth);
		}

	};
}