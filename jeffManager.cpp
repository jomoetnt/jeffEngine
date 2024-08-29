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
	jefGraf = new jGraphics(hWnd, width, height);

	jefSound = new jeffAudio();
	jefSound->loadSound(L"testSound.wav");
	jefSound->playSound(L"testSound.wav");

	jeffModel::graphicsStruct modelStruct(jefGraf->jDev, jefGraf->jContext, jefGraf->jLayout, jefGraf->jRast, width, height);
	jModel = new jeffModel("cube.obj", L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl", modelStruct);


	std::vector<JEFF_DATATYPE> testParams; testParams.emplace_back(jeffNamespace::JEFF_BOOL);
	jeffFuncStruct testFunction(moveCube, testParams);
	functionLookup["moveCube"] = testFunction;

	jefInput.callbackDictionary[jeffInput::W].emplace_back("moveCube(true)");
	jefInput.callbackDictionary[jeffInput::S].emplace_back("moveCube(false)");
}

int jeffManager::doFrame()
{
	std::chrono::time_point<std::chrono::steady_clock> startTime = std::chrono::steady_clock::now();
	jefGraf->draw3D(jModel);
	jefGraf->draw2D();
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

	jefGraf->endFrame();

	return quit;
}

void jeffManager::playSound(LPCWSTR filename)
{
	jefSound->playSound(filename);
}

void jeffNamespace::moveCube(std::vector<jeffType> jParams)
{
	bool paramBool = jParams.front().jBool;
	float multiplier = paramBool ? 1.0f : -1.0f;
	jModel->transformPosition.z += delta * 50.0f * multiplier;
}

void jeffManager::handleKeyEvent(char keycode)
{
	std::vector<std::string> funcSignatures = jefInput.handleKeyEvent(keycode);
	for (auto &funcSignature : funcSignatures)
	{
		if (funcSignature.compare("unknown") == 0)
			continue;
		parseFuncSignature(funcSignature);
	}
}

void jeffManager::parseFuncSignature(std::string funcSignature)
{
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

	parseArgs(funcName, args);
}

void jeffManager::parseArgs(std::string funcName, std::vector<std::string> args)
{
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