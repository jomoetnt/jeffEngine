#pragma once

#include "jeffGraphics.h"
#include "jeffInput.h"
#include "jeffAudio.h"
#include <vector>
#include <chrono>

namespace jeffNamespace
{
	class jeffManager;

	enum JEFF_DATATYPE
	{
		JEFF_FLOAT, JEFF_INT, JEFF_BOOL, JEFF_CHAR
	};
	union jeffType
	{
		float jFloat;
		int jInt;
		bool jBool;
		char jChar;
	};

	typedef void jeffFunc(std::vector<jeffType>);
	typedef jeffFunc* pJeffFunc;

	struct jeffFuncStruct
	{
		pJeffFunc funcPtr;
		std::vector<JEFF_DATATYPE> paramTypes;

		jeffFuncStruct(pJeffFunc initPtr = nullptr, std::vector<JEFF_DATATYPE> initTypes = std::vector<JEFF_DATATYPE>())
		{
			funcPtr = initPtr; paramTypes = initTypes;
		}
	};

	void moveCube(std::vector<jeffType> jParams);

	extern jeffModel* jModel;

	static float delta;

	class jeffManager
	{
	public:
		jeffManager() {}
		jeffManager(HWND hWnd);

		~jeffManager()
		{
			delete jefGraf;
			delete jModel;
		}

		void handleKeyEvent(char keycode);

		int doFrame();

		void playSound(LPCWSTR filename);

	private:
		std::vector<jeffObject*> jObjects;
		std::map<std::string, jeffFuncStruct> functionLookup;

		jGraphics* jefGraf = nullptr;
		jeffAudio* jefSound = nullptr;
		jeffInput jefInput;

		float refreshTimer = 0.0f;
		int quit = 0;

		RECT screenSize{};
		int width = 0; int height = 0;

		void parseFuncSignature(std::string funcSignature);
		void parseArgs(std::string funcName, std::vector<std::string> args);

	};
}