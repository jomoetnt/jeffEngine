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

		jeffManager() {}
		jeffManager(HWND hWnd);

		~jeffManager()
		{
			delete jefGraf;
			delete jScene;
		}

		void handleKeyEvent(char keycode);

		int doFrame();
		void doPhysicsTick(float delta);

		void playSound(LPCWSTR filename);

	private:
		jGraphics* jefGraf = nullptr;
		jeffAudio* jefSound = nullptr;
		jeffInput jefInput;

		jeffScene* jScene = nullptr;

		float refreshTimer = 0.0f;
		int quit = 0;

		RECT screenSize{};
		int width = 0; int height = 0;

	};
}