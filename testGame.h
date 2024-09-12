#pragma once
#include "testScene.h"

namespace jeffNamespace
{
	class testGame
	{
	public:
		testScene* scene = nullptr;

		testGame()
		{
			startGame();
		}

		~testGame()
		{
			delete scene;
		}

		void startGame()
		{
			scene = new testScene(jGraphics::getInstance()->jDev, jGraphics::getInstance()->jContext);
			jeffManager::getInstance()->addScene(scene);
			jeffManager::getInstance()->changeScene("jeffTestScene");
		}
	};
}