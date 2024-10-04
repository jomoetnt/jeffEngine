#include "jeffManager.h"

using namespace jeffNamespace;

void jeffManager::makeInstance()
{
	instance = new jeffManager();
}

int jeffManager::doFrame()
{
	jGraphics::getInstance()->beginFrame();
	if (jActiveScene != nullptr)
		jActiveScene->draw();

	// make 2d nodes to replace
	jGraphics::getInstance()->draw2D();

	jGraphics::getInstance()->endFrame();
	return quit;
}

void jeffManager::doPhysicsTick(float delta)
{
	if (jActiveScene == nullptr) return;
	jActiveScene->doPhysicsTick(delta);
}

void jeffManager::addScene(jeffScene* newScene, bool change)
{
	jScenes.emplace_back(newScene);
	jActiveScene = newScene;
}

void jeffManager::changeScene(std::string scnName)
{
	for (auto& scn : jScenes)
	{
		if (scn->sceneName.compare(scnName) == 0)
			jActiveScene = scn;
	}
}

int jeffManager::handleInputEvent(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        jGraphics::destroyInstance();
        jeffAudio::destroyInstance();
        jeffManager::destroyInstance();
        PostQuitMessage(0);
        break;
    case WM_CLOSE:
        quit = 1;
        DestroyWindow(hWnd);
        break;
    case WM_KEYDOWN:
        jeffManager::getInstance()->handleKeyEvent((char)wParam);
        break;
    case WM_MOUSEMOVE:
        jeffManager::getInstance()->handleMouseEvent((float)GET_X_LPARAM(lParam), (float)GET_Y_LPARAM(lParam));
        break;
    case WM_LBUTTONDOWN:
        jeffManager::getInstance()->handleKeyEvent(0x01);
        break;
    case WM_LBUTTONUP:
        jeffManager::getInstance()->handleKeyEvent(0x01, false);
        break;
    case WM_RBUTTONDOWN:
        jeffManager::getInstance()->handleKeyEvent(0x02);
        break;
    case WM_MBUTTONDOWN:
        jeffManager::getInstance()->handleKeyEvent(0x04);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void jeffManager::handleKeyEvent(char keycode, bool keydown)
{
	JEFF_KEY eventKey = jeffInput::getInstance()->handleKeyEvent(keycode);
	if (eventKey == UNKNOWN) return;

	if (jActiveScene == nullptr) return;
	float xy[2] = {jActiveScene->mouseX, jActiveScene->mouseY};
	jActiveScene->handleInputEvent(eventKey, xy, keydown);
}

void jeffManager::handleMouseEvent(float x, float y)
{
	if (jActiveScene == nullptr) return;
	jActiveScene->mouseX = x; jActiveScene->mouseY = y;
}