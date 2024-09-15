#include "jeffObject.h"

using namespace jeffNamespace;

jeffObject::jeffObject(const char* name) : nodeName(name)
{

}

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
	case JEFF_INPUT_EVENT:
		jeffInputEvent* jeffEvent = (jeffInputEvent*)jParams;
		handleInputEvent(jeffEvent->key, jeffEvent->coords, jeffEvent->keydown);
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

void jeffObject::addChild(jeffObject* child)
{
	child->parent = this;
	child->initObject();
	children.emplace_back(child);
}

jeffObject* jeffObject::find(std::string name)
{
	if (name.compare(nodeName) == 0) return this;
	for (auto& child : children)
	{
		if (child->nodeName.compare(name) == 0) return child;
		else child->find(name);
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

void jeffObject::translateLocal(DirectX::XMFLOAT3 offset)
{
	DirectX::XMVECTOR offVec = DirectX::XMLoadFloat3(&offset);
	offVec = DirectX::XMVector3Transform(offVec, DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&transformRotation)));
	DirectX::XMStoreFloat3(&offset, offVec);
	transformPosition.x += offset.x; transformPosition.y += offset.y; transformPosition.z += offset.z;
}