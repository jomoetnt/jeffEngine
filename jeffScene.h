#pragma once

#include "jeffModel.h"

namespace jeffNamespace
{
	class jeffScene
	{
	public:
		graphicsStruct jGrafStruct;

		jeffObject rootNode;
		jeffCamera* jActiveCam = nullptr;
		std::array<jeffLightPoint*, 4> jPointLights;
		jeffLightDirectional* jDirLight;
		std::vector<jeffModel*> jModels;

		jeffScene(graphicsStruct jefStruct)
		{
			jGrafStruct = jefStruct;

			initObjects();
		}

		~jeffScene()
		{
			delete jActiveCam;
			delete jDirLight;
			for (jeffModel* mod : jModels)
				delete mod;
			for (jeffLightPoint* light : jPointLights)
				delete light;
		}

		void initObjects()
		{
			jeffModel* jModel = new jeffModel("cube.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", jGrafStruct);
			jModel->transformPosition.x += 2.0f;

			jeffModel* jModel2 = new jeffModel("sphere.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", jGrafStruct);
			jModels.emplace_back(jModel); jModels.emplace_back(jModel2);
			rootNode.addChild((jeffObject*)jModel); rootNode.addChild((jeffObject*)jModel2);

			for (int i = 0; i < 4; i++)
			{
				jPointLights[i] = new jeffLightPoint();
				rootNode.addChild((jeffObject*)jPointLights[i]);
			}

			jDirLight = new jeffLightDirectional();
			jActiveCam = new jeffCamera();
			rootNode.addChild((jeffObject*)jDirLight);
			rootNode.addChild((jeffObject*)jActiveCam);

			rootNode.initObject();
		}

		void handleKeyEvent(JEFF_KEY &eventKey)
		{
			rootNode.handleEvent(JEFF_KEY_EVENT, &eventKey);
		}

		void doPhysicsTick(float delta)
		{
			rootNode.tick(delta);
		}

		void draw()
		{
			for (jeffModel* mod : jModels)
				mod->draw(jPointLights, jDirLight, jActiveCam);
		}

	};
}