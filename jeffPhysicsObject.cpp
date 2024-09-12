#include "jeffPhysicsObject.h"

using namespace jeffNamespace;

jeffPhysicsObject::jeffPhysicsObject(DirectX::XMFLOAT3 colliderStart, DirectX::XMFLOAT3 colliderEnd)
{
	boundingBox.start = colliderStart;
	boundingBox.end = colliderEnd;
}

void jeffPhysicsObject::initObject()
{
	// temporary test
	transformPosition.z += 7.0f;
}

void jeffPhysicsObject::tick(float delta)
{
	debugShape->transformPosition = transformPosition;
	// temporary test
	transformRotation.x += delta;
	transformRotation.z += delta * 0.5f;
}

void jeffPhysicsObject::handleInputEvent(JEFF_KEY key, float* coords, bool keydown)
{
	// temporary test
	if (!keydown) return;
	if (key == LEFTCLICK)
	{
		jeffObject* cam = parent->find("jeffCam");
		DirectX::XMFLOAT3 origin = cam->transformPosition;

		float halfWidth = jGraphics::getInstance()->screenWidth * 0.5f; float halfHeight = jGraphics::getInstance()->screenHeight * 0.5f;
		float difX = (coords[0] - halfWidth) / (halfWidth); float difY = (halfHeight - coords[1]) / halfHeight;
		float aspect = (float)jGraphics::getInstance()->screenWidth / jGraphics::getInstance()->screenHeight;
		difX *= aspect;

		DirectX::XMFLOAT4 dir = DirectX::XMFLOAT4(difX, difY, 1.0f, 0.0f);
		DirectX::XMVECTOR dirTransformed = DirectX::XMLoadFloat4(&dir);
		dirTransformed = DirectX::XMVector4Transform(dirTransformed, cam->getTransformMat());
		DirectX::XMStoreFloat4(&dir, dirTransformed);
		if (isOverlappingRay(DirectX::XMFLOAT3(dir.x, dir.y, dir.z), origin))
		{
			if (transformScale.x == 1.0f) transformScale.x = 2.0f;
			else transformScale.x = 1.0f;
			jeffAudio::getInstance()->playSound(L"testSound.wav");
		}
	}
}

bool jeffPhysicsObject::isOverlapping(jeffPhysicsObject& other)
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
bool jeffPhysicsObject::isOverlappingRay(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 start)
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