#include "jeffManager.h"

using namespace jeffNamespace;

namespace jeffNamespace
{
	jeffModel* jModel;
}

jeffManager::jeffManager(HWND hWnd)
{
	GetClientRect(hWnd, &screenSize);
	width = screenSize.right - screenSize.left;
	height = screenSize.bottom - screenSize.top;

	jefGraf = new jGraphics(hWnd);
	jefGraf->screenSize = screenSize;
	jefGraf->width = width; jefGraf->height = height;

	jeffModel::graphicsStruct modelStruct(jefGraf->jDev, jefGraf->jContext, jefGraf->jLayout, jefGraf->testMat, width, height);

	jModel = new jeffModel("cube.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", modelStruct);
	jefGraf->testModel = jModel;
	jefGraf->testModel->jLayout = jefGraf->jLayout;

	std::vector<JEFF_DATATYPE> testParams; testParams.emplace_back(jeffNamespace::JEFF_BOOL);

	jeffFuncStruct testFunction(moveCube, testParams, jModel);
	functionLookup["moveCube"] = testFunction;

	doFrame();
}

int jeffManager::doFrame()
{
	std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
	jefGraf->jDraw();
	std::chrono::time_point<std::chrono::steady_clock> endTime = std::chrono::steady_clock::now();

	int elapsedTime = (int)std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
	float elapsedSeconds = elapsedTime / 1000000000.0f;
	jefGraf->time += elapsedSeconds;
	jefGraf->delta += elapsedSeconds;
	delta = elapsedSeconds;

	refreshTimer += elapsedSeconds;
	if (refreshTimer >= 0.1f)
	{
		refreshTimer = 0.0f;
		int frameRate = (int)(1.0f / elapsedSeconds);
		jefGraf->frameRate = frameRate;
	}

	jModel->tick(delta);

	return quit;
}

void jeffNamespace::moveCube(std::vector<jeffType> jParams)
{
	bool paramBool = jParams.front().jBool;
	float multiplier = paramBool ? 1.0f : -1.0f;
	jModel->transformPosition.z += delta * 5.0f * multiplier;
}		

void jeffManager::handleKeyEvent(char keycode)
{
	std::string funcSignature = jefInput.handleKeyEvent(keycode);
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
			bool parsedBool = args.data()[i].compare("true") == 0;
			jeffType parsedBoolType{}; parsedBoolType.jBool = parsedBool;
			parsedArgs.emplace_back(parsedBoolType);
		}
		break;
		}
		i++;
	}

	functionLookup[funcName].funcPtr(parsedArgs);
}