#pragma once

#include "jeffObject.h"
#include "jeffModel.h"

namespace jeffNamespace
{
	class jeffPhysicsObject : public jeffObject
	{
	public:
		struct
		{
			DirectX::XMFLOAT3 start;
			DirectX::XMFLOAT3 end;
		} boundingBox;

		DirectX::XMFLOAT3 velocity = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 acceleration = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		jeffModel* debugShape = nullptr;

		jeffPhysicsObject(DirectX::XMFLOAT3 colliderStart, DirectX::XMFLOAT3 colliderEnd);

		void initObject() override;
		void tick(float delta) override;

		bool isOverlapping(jeffPhysicsObject& other);
		bool isOverlappingRay(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 start);

	};
}