#pragma once
#include "jeffManager.h"

namespace jeffNamespace
{
	class testScene : public jeffScene
	{
	public:
		jeffPhysicsObject* cube = nullptr;

		testScene(ID3D11Device* dev, ID3D11DeviceContext* cont) : jeffScene(dev, cont)
		{
			
		}

		~testScene()
		{
			delete cube;
		}

		void initObjects() override
		{
			sceneName = "jeffTestScene";

			jeffScene::initObjects();

			jeffAudio::getInstance()->loadSound(L"testSound.wav");

			for (int i = 0; i < 4; i++)
			{
				jPointLights[i] = new jeffLightPoint();
				jPointLights[i]->nodeName = "jeffLight";
				rootNode.addChild((jeffObject*)jPointLights[i]);
			}

			jDirLight = new jeffLightDirectional();
			jDirLight->nodeName = "jeffLight";
			jActiveCam = new jeffCamera();
			jActiveCam->nodeName = "jeffCam";
			rootNode.addChild((jeffObject*)jDirLight);
			rootNode.addChild((jeffObject*)jActiveCam);

			cube = new jeffPhysicsObject(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
			cube->nodeName = "jeffCube";
			rootNode.addChild(cube);

			jeffModel* jModel = new jeffModel("models/cube.obj", jDev, jContext);
			jModel->nodeName = "cubeModel";
			jeffModel* jModel2 = new jeffModel("models/sphere.obj", jDev, jContext);
			jModel2->nodeName = "sphereModel";
			jModel2->transformPosition.z += 7.0f;
			jModel2->transformPosition.x += 2.0f;

			cube->debugShape = new jeffModel("models/cube.obj", jDev, jContext);
			cube->debugShape->nodeName = "cubeModel";
			cube->debugShape->wireframe = true;

			jModels.emplace_back(jModel); jModels.emplace_back(jModel2); jModels.emplace_back(cube->debugShape);

			cube->addChild(jModel); rootNode.addChild(jModel2); rootNode.addChild(cube->debugShape);
		}


	};
}