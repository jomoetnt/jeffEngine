#pragma once

#include "jeffModel.h"

namespace jeffNamespace
{
	class jeffScene
	{
	public:
		jeffObject rootNode;
		jeffCamera* jActiveCam = nullptr;
		std::array<jeffLightPoint*, 4> jPointLights;
		jeffLightDirectional* jDirLight;
		std::vector<jeffModel*> jModels;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;

		jeffScene(ID3D11Device* dev, ID3D11DeviceContext* context) : jDev(dev), jContext(context)
		{
			initObjects();
		}

		~jeffScene()
		{
			delete jActiveCam;
			delete jDirLight;
			for (jeffModel* mod : jModels) delete mod;
			for (jeffLightPoint* light : jPointLights) delete light;
		}

		void initObjects()
		{
			jeffModel* jModel = new jeffModel("cube.obj", jDev, jContext);
			jModel->transformPosition.x += 2.0f;

			jeffModel* jModel2 = new jeffModel("sphere.obj", jDev, jContext);
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
			for (jeffModel* mod : jModels) mod->draw(jPointLights, jDirLight, jActiveCam);
		}

	};
}