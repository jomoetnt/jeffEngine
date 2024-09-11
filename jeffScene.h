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
		jeffObject rootNode;
		jeffCamera* jActiveCam = nullptr;
		std::array<jeffLightPoint*, 4> jPointLights;
		jeffLightDirectional* jDirLight;
		std::vector<jeffModel*> jModels;
		jeffPhysicsObject* cube = nullptr;

		bool debugShapes = true;

		float mouseX, mouseY;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;

		jeffScene(ID3D11Device* dev, ID3D11DeviceContext* context);
		~jeffScene();

		void initObjects();
		void doPhysicsTick(float delta);

		void handleKeyEvent(JEFF_KEY& eventKey);
		void handleMouseEvent(float x, float y);

		void draw();

	};
}