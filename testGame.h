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
			scene = new testScene();
			jeffManager::getInstance()->addScene(scene);
		}
	};
}