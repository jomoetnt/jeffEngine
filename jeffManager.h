#pragma once

#include "jeffGraphics.h"
#include "jeffInput.h"
#include "jeffAudio.h"
#include <vector>
#include <chrono>

namespace jeffNamespace
{
	class jeffManager;

	//extern jeffModel* jModel;

	static float delta;

	class jeffManager
	{
	public:

		jeffManager() {}
		jeffManager(HWND hWnd);

		~jeffManager()
		{
			delete jefGraf;
			for (auto& obj : jModels)
			{
				delete obj;
			}
		}

		void handleKeyEvent(char keycode);

		int doFrame();
		void doPhysicsTick(float delta);

		void playSound(LPCWSTR filename);

	private:
		std::vector<jeffModel*> jModels;

		jGraphics* jefGraf = nullptr;
		jeffAudio* jefSound = nullptr;
		jeffInput jefInput;

		float refreshTimer = 0.0f;
		int quit = 0;

		RECT screenSize{};
		int width = 0; int height = 0;

	};
}