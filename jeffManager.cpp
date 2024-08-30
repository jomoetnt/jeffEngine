#include "jeffManager.h"

using namespace jeffNamespace;

namespace jeffNamespace
{
	jeffModel* jModel;
}

jeffManager::jeffManager(HWND hWnd)
{
	GetClientRect(hWnd, &screenSize);
	width = screenSize.right - screenSize.left;
	height = screenSize.bottom - screenSize.top;
	jefGraf = new jGraphics(hWnd, width, height);

	jefSound = new jeffAudio();
	jefSound->loadSound(L"testSound.wav");
	jefSound->playSound(L"testSound.wav");

	jeffModel::graphicsStruct modelStruct(jefGraf->jDev, jefGraf->jContext, jefGraf->jLayout, jefGraf->jRast, width, height);
	jModel = new jeffModel("cube.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", modelStruct);
	jObjects.emplace_back(jModel);
	

	//std::vector<JEFF_DATATYPE> testParams; testParams.emplace_back(jeffNamespace::JEFF_BOOL);
	//jeffFunc callbackTest = moveCube;
	//jeffFuncStruct testFunction(moveCube, testParams);
	//functionLookup["moveCube"] = testFunction;

	//jefInput.callbackDictionary[jeffInput::W].emplace_back("moveCube(true)");
	//jefInput.callbackDictionary[jeffInput::S].emplace_back("moveCube(false)");



}

int jeffManager::doFrame()
{
	std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
	jefGraf->draw3D(jModel);
	jefGraf->draw2D();
	std::chrono::time_point<std::chrono::steady_clock> endTime = std::chrono::steady_clock::now();

	int elapsedTime = (int)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
	float elapsedSeconds = elapsedTime / 1000000000.0f;
	jefGraf->time += elapsedSeconds;
	jefGraf->delta += elapsedSeconds;
	delta = elapsedSeconds;

	refreshTimer += elapsedSeconds;
	if (refreshTimer >= 0.1f)
	{
		refreshTimer = 0.0f;
		int frameRate = (int)(1.0f / elapsedSeconds);
		jefGraf->frameRate = frameRate;
	}

	jModel->tick(delta);

	jefGraf->endFrame();

	return quit;
}

void jeffManager::playSound(LPCWSTR filename)
{
	jefSound->playSound(filename);
}

void jeffManager::handleKeyEvent(char keycode)
{
	JEFF_KEY eventKey = jefInput.handleKeyEvent(keycode);
	if (eventKey == UNKNOWN) return;

	for (auto &thing : jObjects)
	{
		thing->handleEvent(JEFF_KEY_EVENT, &eventKey);
	}
}