#pragma once

#include <vector>
#include <DirectXMath.h>

class jeffObject
{
public:
	virtual void initObject() = 0;
	virtual void tick(float delta) = 0;

	jeffObject* parent = nullptr;
	std::vector<jeffObject*> children;
	DirectX::XMMATRIX transformMat = DirectX::XMMatrixIdentity();

	void translate(DirectX::XMFLOAT3 offset)
	{
		transformMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(offset.x, offset.y, offset.z), transformMat);
	}

	void translate(float offsetX, float offsetY, float offsetZ)
	{
		transformMat = DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(offsetX, offsetY, offsetZ), transformMat);
	}

};