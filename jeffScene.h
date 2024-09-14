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

		jeffObject rootNode = jeffObject::jeffObject("root");
		jeffCamera* jActiveCam = nullptr;
		std::vector<jeffModel*> jModels;
		// can make vectors later
		std::array<jeffLightPoint*, 4> jPointLights;
		jeffLightDirectional* jDirLight;

		bool debugShapes = true;

		float mouseX, mouseY;

		jeffScene();
		~jeffScene();
		void doPhysicsTick(float delta);
		void handleInputEvent(JEFF_KEY eventKey, float* location, bool keydown);
		void draw();
		void addModel(jeffModel* model, jeffObject* parent);
		void addModel(jeffModel* model);
		void addPhysicsObject(jeffPhysicsObject* obj, jeffObject* parent);
		void addPhysicsObject(jeffPhysicsObject* obj);

	};
}