#include "jeffManager.h"

using namespace jeffNamespace;

void jeffManager::makeInstance()
{
	instance = new jeffManager();

	instance->jScene = new jeffScene(jGraphics::getInstance()->jDev, jGraphics::getInstance()->jContext);

	jeffAudio::getInstance()->loadSound(L"testSound.wav");
}

int jeffManager::doFrame()
{
	jGraphics::getInstance()->beginFrame();
	jScene->draw();

	jGraphics::getInstance()->draw2D(jScene->cube->testRayDir, jScene->cube->testRayStart);

	jGraphics::getInstance()->endFrame();
	return quit;
}

void jeffManager::doPhysicsTick(float delta)
{
	if (jScene == nullptr) return;
	jScene->doPhysicsTick(delta);
}

void jeffManager::handleKeyEvent(char keycode)
{
	JEFF_KEY eventKey = jeffInput::getInstance()->handleKeyEvent(keycode);
	if (eventKey == UNKNOWN) return;

	jScene->handleKeyEvent(eventKey);
}

void jeffManager::handleMouseEvent(float x, float y)
{
	jScene->handleMouseEvent(x, y);
}