#pragma once

#include "jeffGraphics.h"
#include "jeffInput.h"
#include "jeffAudio.h"
#include "jeffScene.h"
#include <windowsx.h>
#include <vector>
#include <chrono>

namespace jeffNamespace
{
	class jeffManager;

	//static float delta;

	class jeffManager
	{
	public:
		void handleKeyEvent(char keycode, bool keydown = true);
		void handleMouseEvent(float x, float y);
		int handleInputEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		int doFrame();
		void doPhysicsTick(float delta);

		void addScene(jeffScene* newScene, bool change = true);
		void changeScene(std::string scnName);

		static void makeInstance();
		static jeffManager* getInstance() { return instance; }
		static void destroyInstance() { delete instance; }

	private:
		jeffManager() = default;

		~jeffManager()
		{
			for (auto& scene : jScenes)
				delete scene;
		}

	    static inline jeffManager* instance;

		std::vector<jeffScene*> jScenes;
		jeffScene* jActiveScene = nullptr;

		float refreshTimer = 0.0f;
		int quit = 0;

	};
}