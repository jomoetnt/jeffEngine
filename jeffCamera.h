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

		jeffCamera()
		{
			setProjMat();
		}

		void handleInputEvent(JEFF_KEY key, float* coords, bool keydown) override
		{
			switch (key)
			{
			case UP: transformPosition.z += 0.1f; break;
			case DOWN: transformPosition.z -= 0.1f; break;
			case LEFT: transformPosition.x -= 0.1f; break;
			case RIGHT: transformPosition.x += 0.1f; break;
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