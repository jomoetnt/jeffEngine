#include "jeffManager.h"

using namespace jeffNamespace;

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

	testModel->tick(delta);

	return quit;
}

void jeffNamespace::moveCube(std::vector<jeffType> jParams)
{

}