#pragma once

#include "jeffModel.h"
#include "jeffPhysicsObject.h"
#include "jeffAudio.h"
#include "jeffGraphics.h"

namespace jeffNamespace
{
	class jeffScene
	{
	public:
		std::string sceneName;

		jeffObject rootNode;
		jeffCamera* jActiveCam = nullptr;
		std::vector<jeffModel*> jModels;
		// can make vectors later
		std::array<jeffLightPoint*, 4> jPointLights;
		jeffLightDirectional* jDirLight;

		bool debugShapes = true;

		float mouseX, mouseY;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;

		jeffScene(ID3D11Device* dev, ID3D11DeviceContext* context);
		~jeffScene();

		virtual void initObjects();
		void doPhysicsTick(float delta);

		void handleInputEvent(JEFF_KEY eventKey, float* location, bool keydown);

		void draw();

	};
}