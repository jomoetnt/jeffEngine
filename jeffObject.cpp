#include "jeffObject.h"

using namespace jeffNamespace;

void jeffObject::initObject()
{
	for (auto& child : children)
	{
		child->initObject();
	}
}

void jeffObject::handleEvent(JEFF_EVENT_TYPE eventType, void* jParams)
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

void jeffObject::tick(float delta)
{
	time += delta;
	for (auto& child : children)
	{
		child->tick(delta);
	}
}

DirectX::XMMATRIX jeffObject::getTransformMat() const
{
	DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&transformRotation);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transformPosition);
	DirectX::XMVECTOR scale = DirectX::XMLoadFloat3(&transformScale);
	DirectX::XMMATRIX childTransform = DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationRollPitchYawFromVector(rot) * DirectX::XMMatrixTranslationFromVector(pos);
	if (parent == nullptr)
		return childTransform;

	return childTransform * parent->getTransformMat();
}

void jeffObject::addChild(jeffObject* child)
{
	child->parent = this;
	children.emplace_back(child);
}