#pragma once

#include "jeffGraphics.h"
#include "jeffInput.h"
#include <vector>
#include <chrono>

namespace jeffNamespace
{
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

		jeffFuncStruct(pJeffFunc initPtr = nullptr, std::vector<JEFF_DATATYPE> initTypes = std::vector<JEFF_DATATYPE>()) : funcPtr(initPtr), paramTypes(initTypes) {}
	};

	void moveCube(std::vector<jeffType> jParams);

	class jeffManager
	{
	public:
		jeffManager(){}
		jeffManager(HWND hWnd)
		{
			GetClientRect(hWnd, &screenSize);
			width = screenSize.right - screenSize.left;
			height = screenSize.bottom - screenSize.top;

			jefGraf = new jGraphics(hWnd);
			jefGraf->screenSize = screenSize;
			jefGraf->width = width; jefGraf->height = height;

			jeffModel::graphicsStruct modelStruct(jefGraf->jDev, jefGraf->jContext, jefGraf->jLayout, jefGraf->testMat, width, height);

			testModel = new jeffModel("cube.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", modelStruct);
			jefGraf->testModel = testModel;
			jefGraf->testModel->jLayout = jefGraf->jLayout;

			std::vector<JEFF_DATATYPE> testParams; testParams.emplace_back(jeffNamespace::JEFF_FLOAT);

			jeffFuncStruct testFunction(moveCube, testParams);
			functionLookup["moveCube"] = testFunction;

			doFrame();
		}

		~jeffManager()
		{
			delete jefGraf;
			delete testModel;
		}

		//void makeEvent();
		//void registerObject();

		void handleKeyEvent(char keycode, bool character)
		{
			std::string funcSignature = jefInput.handleKeyEvent(keycode, character);
			if (funcSignature.compare("unknown") == 0)
				return;

			std::string funcName = std::string();
			for (size_t i = 0; i < funcSignature.size(); i++)
			{
				char c = funcSignature.at(i);
				if (c == '(')
				{
					funcSignature = funcSignature.substr(i + 1);
					funcSignature = funcSignature.substr(0, funcSignature.size() - 1);
					break;
				}

				funcName.push_back(c);
			}

			std::vector<std::string> args = jeffHelper::split(funcSignature, ", ");

			std::vector<jeffType> parsedArgs;
			int i = 0;
			for (auto type : functionLookup[funcName].paramTypes)
			{
				switch (type)
				{
				case JEFF_FLOAT:
				{
					float parsedFloat = std::stof(args.data()[i]);
					jeffType parsedFloatType{}; parsedFloatType.jFloat = parsedFloat;
					parsedArgs.emplace_back(parsedFloatType);
				}
				break;
				case JEFF_INT:
				{
					int parsedInt = std::stoi(args.data()[i]);
					jeffType parsedIntType{}; parsedIntType.jInt = parsedInt;
					parsedArgs.emplace_back(parsedIntType);
				}
				break;
				case JEFF_CHAR:
				{
					char parsedChar = args.data()[i].front();
					jeffType parsedCharType{}; parsedCharType.jChar = parsedChar;
					parsedArgs.emplace_back(parsedCharType);
				}
				break;
				case JEFF_BOOL:
				{
					bool parsedBool = args.data()[i].compare("true");
					jeffType parsedBoolType{}; parsedBoolType.jBool = parsedBool;
					parsedArgs.emplace_back(parsedBoolType);
				}
				break;
				}
				i++;
			}

			functionLookup[funcName].funcPtr(parsedArgs);
		}

		int doFrame();

	private:
		//std::vector<jeffObject*> jObjects;
		std::map<std::string, jeffFuncStruct> functionLookup;

		jGraphics* jefGraf = nullptr;
		jeffInput jefInput;

		float refreshTimer = 0.0f;
		float delta = 0.0f;
		int quit = 0;

		RECT screenSize{};
		int width = 0; int height = 0;
		
		jeffModel* testModel = nullptr;

	};
}