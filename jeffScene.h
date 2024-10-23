#pragma once

#include "jeffModel.h"
#include "jeffPhysicsObject.h"
#include "jeffAudio.h"
#include "jeffGraphics.h"
#include "jeffGUI.h"
#include "jeffText.h"

namespace jeffNamespace
{
	class jeffScene
	{
	public:
		std::string sceneName;

		jeffObject rootNode = jeffObject::jeffObject("root");
		jeffCamera* jActiveCam = nullptr;
		std::vector<jeffModel*> jModels;
		std::vector<jeffGUI*> jGUIs;
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
		void draw2D()
		{
			jGraphics::getInstance()->jRT->BeginDraw();

			for (auto& gui : jGUIs)
				gui->draw(jGraphics::getInstance()->delta);

			HRESULT hr = jGraphics::getInstance()->jRT->EndDraw();
			if (FAILED(hr)) throw std::runtime_error("error ending 2d draw");
		}
		void addModel(jeffModel* model, jeffObject* parent);
		void addModel(jeffModel* model);
		void addPhysicsObject(jeffPhysicsObject* obj, jeffObject* parent);
		void addPhysicsObject(jeffPhysicsObject* obj);

	};
}