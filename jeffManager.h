#pragma once

#include "jeffGraphics.h"
#include "jeffInput.h"
#include "jeffAudio.h"
#include "jeffScene.h"
#include <vector>
#include <chrono>

namespace jeffNamespace
{
	class jeffManager;

	static float delta;

	class jeffManager
	{
	public:
		void handleKeyEvent(char keycode);
		void handleMouseEvent(float x, float y);

		int doFrame();
		void doPhysicsTick(float delta);

		static void makeInstance();
		static jeffManager* getInstance() { return instance; }
		static void destroyInstance() { delete instance; }

	private:
		jeffManager() = default;

		~jeffManager()
		{
			delete jScene;
		}

	    static inline jeffManager* instance;

		jeffScene* jScene = nullptr;

		float refreshTimer = 0.0f;
		int quit = 0;

	};
}