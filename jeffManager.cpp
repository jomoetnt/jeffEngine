#include "jeffManager.h"

using namespace jeffNamespace;

void jeffManager::makeInstance()
{
	instance = new jeffManager();

	instance->jScene = new jeffScene(jGraphics::getInstance()->jDev, jGraphics::getInstance()->jContext);

	instance->jefSound = new jeffAudio();
	instance->jefSound->loadSound(L"testSound.wav");
}

int jeffManager::doFrame()
{
	jGraphics::getInstance()->beginFrame();
	jScene->draw();

	jGraphics::getInstance()->draw2D();

	jGraphics::getInstance()->endFrame();
	return quit;
}

void jeffManager::doPhysicsTick(float delta)
{
	if (jScene == nullptr) return;
	jScene->doPhysicsTick(delta);
}

void jeffManager::playSound(LPCWSTR filename)
{
	jefSound->playSound(filename);
}

void jeffManager::handleKeyEvent(char keycode)
{
	JEFF_KEY eventKey = jefInput.handleKeyEvent(keycode);
	if (eventKey == UNKNOWN) return;

	jScene->handleKeyEvent(eventKey);
}