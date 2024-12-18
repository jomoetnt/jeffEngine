#pragma once
#include "jeffManager.h"

namespace jeffNamespace
{
	class testScene : public jeffScene
	{
	public:
		jeffPhysicsObject* cube = nullptr;

		testScene() : jeffScene::jeffScene()
		{
			sceneName = "jeffTestScene";

			jeffAudio::getInstance()->loadSound(L"testSound.wav");

			jDirLight = new jeffLightDirectional("jeffLight");
			jActiveCam = new jeffCamera("jeffCam");
			rootNode.addChild(jDirLight);
			rootNode.addChild(jActiveCam);

			cube = new jeffPhysicsObject("jeffCube", DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
			addPhysicsObject(cube);

			jeffModel* jModel2 = new jeffModel("sphereModel", "models/multipingu.obj");
			jModel2->transformPosition = DirectX::XMFLOAT3(2.0f, 0.0f, 7.0f);
			addModel(new jeffModel("cubeModel", "models/texturedcube.obj"), cube);
			addModel(jModel2);

			jeffFPS* fpsCounter = new jeffFPS("fps", jGraphics::getInstance()->jRT);
			fpsCounter->transformPosition.x = 0.0f; fpsCounter->transformPosition.y = 0.0f;
			fpsCounter->transformScale.x = 150.0f; fpsCounter->transformScale.y = 100.0f;
			jeffGUI* gooey = new jeffGUI("HUD", jGraphics::getInstance()->jRT);
			gooey->guiElements.emplace_back(fpsCounter);
			jGUIs.emplace_back(gooey);

			for (int i = 0; i < 4; i++)
			{
				jPointLights[i] = new jeffLightPoint("jeffLight");
				jActiveCam->addChild(jPointLights[i]);
			}

			jeffModel* jModel3 = new jeffModel("cloudmodel", "models/cloudremake.glb");
		}

		~testScene()
		{
			delete cube;
		}


	};
}