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

		int doFrame();
		void doPhysicsTick(float delta);

		void playSound(LPCWSTR filename);

		static void makeInstance();
		static jeffManager* getInstance() { return instance; }
		static void destroyInstance() { delete instance; }

	private:
		jeffManager() = default;

		~jeffManager()
		{
			delete jScene;
			delete jefSound;
		}

	    static inline jeffManager* instance;

		jeffScene* jScene = nullptr;
		jeffAudio* jefSound = nullptr;
		jeffInput jefInput;

		float refreshTimer = 0.0f;
		int quit = 0;

	};
}