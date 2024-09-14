#include "jeffScene.h"

using namespace jeffNamespace;

jeffScene::jeffScene()
{
}

jeffScene::~jeffScene()
{
	delete jActiveCam;
	delete jDirLight;
	for (jeffModel* mod : jModels) delete mod;
	for (jeffLightPoint* light : jPointLights) delete light;
}

void jeffScene::handleInputEvent(JEFF_KEY eventKey, float* location, bool keydown)
{
	jeffInputEvent jeffEvent{};
	jeffEvent.key = eventKey;
	jeffEvent.coords = location;
	jeffEvent.keydown = keydown;
	rootNode.handleEvent(JEFF_INPUT_EVENT, &jeffEvent);
}

void jeffScene::doPhysicsTick(float delta)
{
	rootNode.tick(delta);
}

void jeffScene::draw()
{
	for (jeffModel* mod : jModels) mod->draw(jPointLights, jDirLight, jActiveCam);
}

void jeffScene::addModel(jeffModel* model, jeffObject* parent)
{
	jModels.emplace_back(model);
	parent->addChild(model);
}

void jeffScene::addModel(jeffModel* model)
{
	jModels.emplace_back(model);
	rootNode.addChild(model);
}

void jeffScene::addPhysicsObject(jeffPhysicsObject* obj, jeffObject* parent)
{
	parent->addChild(obj);
	parent->addChild(obj->debugShape);
	jModels.emplace_back(obj->debugShape);
}

void jeffScene::addPhysicsObject(jeffPhysicsObject* obj)
{
	rootNode.addChild(obj);
	rootNode.addChild(obj->debugShape);
	jModels.emplace_back(obj->debugShape);
}