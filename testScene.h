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

			for (int i = 0; i < 4; i++)
			{
				jPointLights[i] = new jeffLightPoint("jeffLight");
				jActiveCam->addChild(jPointLights[i]);
			}

			jeffJSON::JSONObject testJSON;
			jeffJSON::readJSON("testJSON.json", &testJSON);
			std::string reString = testJSON.dictionary["testProperty7"].jeffString;
		}

		~testScene()
		{
			delete cube;
		}


	};
}