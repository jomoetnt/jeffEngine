#pragma once

#include <vector>
#include <DirectXMath.h>

namespace jeffNamespace
{
	class jeffObject
	{
	public:
		virtual void initObject() = 0;
		virtual void tick(float delta) = 0;

		jeffObject* parent = nullptr;
		std::vector<jeffObject*> children;
		DirectX::XMMATRIX transformMat = DirectX::XMMatrixIdentity();
		DirectX::XMFLOAT3 transformPosition = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 transformRotation = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

	};
}
