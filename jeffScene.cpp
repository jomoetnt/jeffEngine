#include "jeffScene.h"

using namespace jeffNamespace;

jeffScene::jeffScene(ID3D11Device* dev, ID3D11DeviceContext* context) : jDev(dev), jContext(context)
{
	initObjects();
}

jeffScene::~jeffScene()
{
	delete jActiveCam;
	delete jDirLight;
	for (jeffModel* mod : jModels) delete mod;
	for (jeffLightPoint* light : jPointLights) delete light;
}

void jeffScene::initObjects()
{
	rootNode.nodeName = "root";
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