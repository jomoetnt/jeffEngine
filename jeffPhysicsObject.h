#pragma once

#include "jeffObject.h"

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

		// temp
		DirectX::XMFLOAT3 testRayStart = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		DirectX::XMFLOAT3 testRayDir = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);

		jeffPhysicsObject(DirectX::XMFLOAT3 colliderStart, DirectX::XMFLOAT3 colliderEnd)
		{
			boundingBox.start = colliderStart;
			boundingBox.end = colliderEnd;
		}

		void initObject() override
		{
			transformPosition.z += 7.0f;
		}

		void tick(float delta) override
		{
			transformRotation.x += delta;
			transformRotation.z += delta * 0.5f;
			if (isOverlappingRay(testRayDir, testRayStart)) children.front()->transformPosition.y = 2.0f;
			else children.front()->transformPosition.y = 0.0f;
		}

		void handleKeyEvent(JEFF_KEY* key) override
		{
			if (*key == W)
			{
				testRayDir.z += 0.1f;
			}
			if (*key == S)
			{
				testRayDir.z -= 0.1f;
			}
			if (*key == A)
			{
				testRayDir.y -= 0.1f;
			}
			if (*key == D)
			{
				testRayDir.y += 0.1f;
			}
			if (*key == Q)
			{
				testRayDir.x -= 0.1f;
			}
			if (*key == E)
			{
				testRayDir.x += 0.1f;
			}
			if (*key == I)
			{
				testRayStart.z += 0.1f;
			}
			if (*key == K)
			{
				testRayStart.z -= 0.1f;
			}
			if (*key == J)
			{
				testRayStart.y -= 0.1f;
			}
			if (*key == L)
			{
				testRayStart.y += 0.1f;
			}
			if (*key == U)
			{
				testRayStart.x -= 0.1f;
			}
			if (*key == O)
			{
				testRayStart.x += 0.1f;
			}
		}

		bool isOverlapping(jeffPhysicsObject& other)
		{
			DirectX::XMFLOAT3 startLocation = DirectX::XMFLOAT3(transformPosition.x + boundingBox.start.x, transformPosition.y + boundingBox.start.y, transformPosition.z + boundingBox.start.z);
			DirectX::XMFLOAT3 otherStartLocation = DirectX::XMFLOAT3(other.transformPosition.x + other.boundingBox.start.x, other.transformPosition.y + other.boundingBox.start.y, other.transformPosition.z + other.boundingBox.start.z);
			DirectX::XMFLOAT3 endLocation = DirectX::XMFLOAT3(transformPosition.x + boundingBox.end.x, transformPosition.y + boundingBox.end.y, transformPosition.z + boundingBox.end.z);
			DirectX::XMFLOAT3 otherEndLocation = DirectX::XMFLOAT3(other.transformPosition.x + other.boundingBox.end.x, other.transformPosition.y + other.boundingBox.end.y, other.transformPosition.z + other.boundingBox.end.z);

			bool xStartWithin = startLocation.x < otherStartLocation.x && startLocation.x > otherEndLocation.x;
			bool xEndWithin = endLocation.x < otherStartLocation.x && endLocation.x > otherEndLocation.x;
			bool yStartWithin = startLocation.y < otherStartLocation.y && startLocation.y > otherEndLocation.y;
			bool yEndWithin = endLocation.y < otherStartLocation.y && endLocation.y > otherEndLocation.y;
			bool zStartWithin = startLocation.z < otherStartLocation.z && startLocation.z > otherEndLocation.z;
			bool zEndWithin = endLocation.z < otherStartLocation.z && endLocation.z > otherEndLocation.z;

			return (xStartWithin || xEndWithin) && (yStartWithin || yEndWithin) && (zStartWithin || zEndWithin);
		}

		// slab method of finding intersection between AABB and ray
		// treat boundaries as infinite planes and find points of intersection
		// if the ray intersects with 3 of the orthogonal planes in a row before anything else, the ray hits the collider
		// if the ray instead intersects with both parallel planes in a pair before intersecting with 3 orthogonal planes, it does not
		bool isOverlappingRay(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 start)
		{
			float mint_x1 = (transformPosition.x + boundingBox.start.x - start.x) / dir.x;
			float mint_x2 = (transformPosition.x + boundingBox.end.x - start.x) / dir.x;
			float mint_x = fminf(mint_x1, mint_x2);

			float mint_y1 = (transformPosition.y + boundingBox.start.y - start.y) / dir.y;
			float mint_y2 = (transformPosition.y + boundingBox.end.y - start.y) / dir.y;
			float mint_y = fminf(mint_y1, mint_y2);

			float mint_z1 = (transformPosition.z + boundingBox.start.z - start.z) / dir.z;
			float mint_z2 = (transformPosition.z + boundingBox.end.z - start.z) / dir.z;
			float mint_z = fminf(mint_z1, mint_z2);

			float maxt_x1 = (transformPosition.x + boundingBox.start.x - start.x) / dir.x;
			float maxt_x2 = (transformPosition.x + boundingBox.end.x - start.x) / dir.x;
			float maxt_x = fmaxf(maxt_x1, maxt_x2);

			float maxt_y1 = (transformPosition.y + boundingBox.start.y - start.y) / dir.y;
			float maxt_y2 = (transformPosition.y + boundingBox.end.y - start.y) / dir.y;
			float maxt_y = fmaxf(maxt_y1, maxt_y2);

			float maxt_z1 = (transformPosition.z + boundingBox.start.z - start.z) / dir.z;
			float maxt_z2 = (transformPosition.z + boundingBox.end.z - start.z) / dir.z;
			float maxt_z = fmaxf(maxt_z1, maxt_z2);

			float tnear = fmaxf(fmaxf(mint_x, mint_y), mint_z);
			float tfar = fminf(fminf(maxt_x, maxt_y), maxt_z);

			return tnear <= tfar;
		}

	};
}