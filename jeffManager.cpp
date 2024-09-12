#include "jeffManager.h"

using namespace jeffNamespace;

void jeffManager::makeInstance()
{
	instance = new jeffManager();
}

int jeffManager::doFrame()
{
	jGraphics::getInstance()->beginFrame();
	if (jActiveScene != nullptr)
		jActiveScene->draw();

	// make 2d nodes to replace
	jGraphics::getInstance()->draw2D();

	jGraphics::getInstance()->endFrame();
	return quit;
}

void jeffManager::doPhysicsTick(float delta)
{
	if (jActiveScene == nullptr) return;
	jActiveScene->doPhysicsTick(delta);
}

void jeffManager::addScene(jeffScene* newScene)
{
	newScene->initObjects();
	jScenes.emplace_back(newScene);
}

void jeffManager::changeScene(std::string scnName)
{
	for (auto& scn : jScenes)
	{
		if (scn->sceneName.compare(scnName) == 0)
			jActiveScene = scn;
	}
}

void jeffManager::handleKeyEvent(char keycode, bool keydown)
{
	JEFF_KEY eventKey = jeffInput::getInstance()->handleKeyEvent(keycode);
	if (eventKey == UNKNOWN) return;

	if (jActiveScene == nullptr) return;
	float xy[2] = {jActiveScene->mouseX, jActiveScene->mouseY};
	jActiveScene->handleInputEvent(eventKey, xy, keydown);
}

void jeffManager::handleMouseEvent(float x, float y)
{
	if (jActiveScene == nullptr) return;
	jActiveScene->mouseX = x; jActiveScene->mouseY = y;
}