#include "jeffManager.h"

using namespace jeffNamespace;


jeffManager::jeffManager(HWND hWnd)
{
	GetClientRect(hWnd, &screenSize);
	width = screenSize.right - screenSize.left;
	height = screenSize.bottom - screenSize.top;
	jefGraf = new jGraphics(hWnd, width, height);

	graphicsStruct modelStruct(jefGraf->jDev, jefGraf->jContext, width, height);
	jScene = new jeffScene(modelStruct);

	jefSound = new jeffAudio();
	jefSound->loadSound(L"testSound.wav");
}

int jeffManager::doFrame()
{
	jefGraf->beginFrame();
	jScene->draw();

	jefGraf->draw2D();

	jefGraf->endFrame();
	return quit;
}

void jeffManager::doPhysicsTick(float delta)
{
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