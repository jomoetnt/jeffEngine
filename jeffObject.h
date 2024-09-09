#pragma once

#include <vector>
#include <array>
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
		DirectX::XMFLOAT3 transformPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

		float time = 0.0f;

		virtual void initObject() 
		{
			for (auto& child : children)
			{
				child->initObject();
			}
		}

		virtual void handleEvent(JEFF_EVENT_TYPE eventType, void* jParams)
		{
			switch (eventType)
			{
			case JEFF_KEY_EVENT:
				handleKeyEvent((JEFF_KEY*)jParams);
				break;
			case JEFF_MOUSE_EVENT:
				handleMouseEvent((float*)jParams);
				break;
			}

			for (auto& child : children)
			{
				child->handleEvent(eventType, jParams);
			}
		}

		virtual void handleKeyEvent(JEFF_KEY* key) {}
		virtual void handleMouseEvent(float* coords) {}

		virtual void tick(float delta)
		{
			time += delta;
			for (auto& child : children)
			{
				child->tick(delta);
			}
		}

		DirectX::XMMATRIX getTransformMat() const
		{
			DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&transformRotation);
			DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transformPosition);
			DirectX::XMVECTOR scale = DirectX::XMLoadFloat3(&transformScale);
			DirectX::XMMATRIX childTransform = DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationRollPitchYawFromVector(rot) * DirectX::XMMatrixTranslationFromVector(pos);
			if (parent == nullptr)
				return childTransform;

			return childTransform * parent->getTransformMat();
		}

		void addChild(jeffObject* child)
		{
			child->parent = this;
			children.emplace_back(child);
		}


	};
}
