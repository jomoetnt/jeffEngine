#include "jeffManager.h"

using namespace jeffNamespace;


jeffManager::jeffManager(HWND hWnd)
{
	GetClientRect(hWnd, &screenSize);
	width = screenSize.right - screenSize.left;
	height = screenSize.bottom - screenSize.top;
	jefGraf = new jGraphics(hWnd, width, height);

	jeffModel::graphicsStruct modelStruct(jefGraf->jDev, jefGraf->jContext, jefGraf->jLayout, jefGraf->jRast, width, height);
	jeffModel* jModel = new jeffModel("cube.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", modelStruct);
	jeffModel* jModel2 = new jeffModel("sphere.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", modelStruct);
	jModel->transformPosition.x += 2.0f;
	jModels.emplace_back(jModel);
	jModels.emplace_back(jModel2);

	for (int i = 0; i < 4; i++)
		jPLights[i] = jeffLightPoint();

	jDLight.initObject();

	jActiveCam = new jeffCamera();
	jActiveCam->setProjMat();


	jefSound = new jeffAudio();
	jefSound->loadSound(L"testSound.wav");
}

int jeffManager::doFrame()
{
	jefGraf->beginFrame();
	for (auto &model : jModels)
	{
		model->draw(jPLights, jDLight, jActiveCam);
		jefGraf->jLayout = model->jLayout;
		jefGraf->jRast = model->jRast;
	}
	jefGraf->draw2D();

	jefGraf->endFrame();
	return quit;
}

void jeffManager::doPhysicsTick(float delta)
{
	for (auto& model : jModels)
	{
		model->tick(delta);
	}
}

void jeffManager::playSound(LPCWSTR filename)
{
	jefSound->playSound(filename);
}

void jeffManager::handleKeyEvent(char keycode)
{
	JEFF_KEY eventKey = jefInput.handleKeyEvent(keycode);
	if (eventKey == UNKNOWN) return;
	//jefSound->playSound(L"testSound.wav");
	jActiveCam->handleEvent(JEFF_KEY_EVENT, &eventKey);

	jDLight.handleEvent(JEFF_KEY_EVENT, &eventKey);

	for (auto& thing : jModels)
	{
		thing->handleEvent(JEFF_KEY_EVENT, &eventKey);
	}
	for (auto& thing : jPLights)
	{
		thing.handleEvent(JEFF_KEY_EVENT, &eventKey);
	}
}