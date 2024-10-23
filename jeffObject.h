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
		JEFF_INPUT_EVENT,
	};

	struct jeffInputEvent
	{
		JEFF_KEY key = UNKNOWN;
		float* coords = nullptr;
		bool keydown = true;
	};

	class jeffObject
	{
	public:
		std::string nodeName;
		bool dimension3 = true;

		jeffObject* parent = nullptr;
		std::vector<jeffObject*> children;
		DirectX::XMFLOAT3 transformPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformScale = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

		float time = 0.0f;
		jeffObject(const char* name);

		// Object node methods
		virtual void initObject();
		virtual void tick(float delta);
		void addChild(jeffObject* child);
		jeffObject* find(std::string name);
		DirectX::XMMATRIX getTransformMat() const;
		void translateLocal(DirectX::XMFLOAT3 offset);
		// Events
		virtual void handleEvent(JEFF_EVENT_TYPE eventType, void* jParams);
		virtual void handleInputEvent(JEFF_KEY key, float* coords, bool keydown) {}

	};
}
