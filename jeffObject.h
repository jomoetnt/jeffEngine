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

		virtual void initObject();

		virtual void handleEvent(JEFF_EVENT_TYPE eventType, void* jParams);
		virtual void handleKeyEvent(JEFF_KEY* key) {}
		virtual void handleMouseEvent(float* coords) {}

		virtual void tick(float delta);

		void addChild(jeffObject* child);

		DirectX::XMMATRIX getTransformMat() const;


	};
}
