#pragma once

#include <vector>
#include <map>
#include <string>
#include <DirectXMath.h>
#include "jeffInput.h"

namespace jeffNamespace
{
	enum JEFF_EVENT_TYPE
	{
		JEFF_KEY_EVENT, JEFF_MOUSE_EVENT
	};

	class jeffObject
	{
	public:
		jeffObject* parent = nullptr;
		std::vector<jeffObject*> children;
		DirectX::XMMATRIX transformMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 transformPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

		float time = 0.0f;

		virtual void initObject() = 0;

		virtual void handleEvent(JEFF_EVENT_TYPE eventType, void* jParams)
		{
			switch (eventType)
			{
			case JEFF_KEY_EVENT: 
				handleKeyEvent((JEFF_KEY*)jParams); 
				break;
			}
		}

		virtual void handleKeyEvent(JEFF_KEY* key) = 0;

		virtual void tick(float delta)
		{
			time += delta;
		}

		virtual DirectX::XMMATRIX getTransformMat()
		{
			DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&transformRotation);
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transformPosition);
			DirectX::XMVECTOR scale = DirectX::XMLoadFloat3(&transformScale);
			return DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationRollPitchYawFromVector(rot) * DirectX::XMMatrixTranslationFromVector(pos);
		}

	};
}
