#include "jeffScene.h"

using namespace jeffNamespace;

jeffScene::jeffScene(ID3D11Device* dev, ID3D11DeviceContext* context) : jDev(dev), jContext(context)
{
	initObjects();
}

jeffScene::~jeffScene()
{
	delete jActiveCam;
	delete jDirLight;
	delete cube;
	for (jeffModel* mod : jModels) delete mod;
	for (jeffLightPoint* light : jPointLights) delete light;
}

void jeffScene::initObjects()
{
	for (int i = 0; i < 4; i++)
	{
		jPointLights[i] = new jeffLightPoint();
		rootNode.addChild((jeffObject*)jPointLights[i]);
	}

	jDirLight = new jeffLightDirectional();
	jActiveCam = new jeffCamera();
	rootNode.addChild((jeffObject*)jDirLight);
	rootNode.addChild((jeffObject*)jActiveCam);

	cube = new jeffPhysicsObject(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f), DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	rootNode.addChild(cube);

	jeffModel* jModel = new jeffModel("models/cube.obj", jDev, jContext);
	jeffModel* jModel2 = new jeffModel("models/sphere.obj", jDev, jContext);
	jModel2->transformPosition.z += 7.0f;
	jModel2->transformPosition.x += 2.0f;

	cube->debugShape = new jeffModel("models/cube.obj", jDev, jContext);
	cube->debugShape->wireframe = true;

	jModels.emplace_back(jModel); jModels.emplace_back(jModel2); jModels.emplace_back(cube->debugShape);

	cube->addChild(jModel); rootNode.addChild(jModel2); rootNode.addChild(cube->debugShape);

	rootNode.initObject();
}

void jeffScene::handleKeyEvent(JEFF_KEY& eventKey)
{
	if (eventKey == LEFTCLICK)
	{
		DirectX::XMFLOAT3 origin = jActiveCam->transformPosition;

		float halfWidth = jGraphics::getInstance()->screenWidth * 0.5f; float halfHeight = jGraphics::getInstance()->screenHeight * 0.5f;
		float difX = (mouseX - halfWidth) / (halfWidth); float difY = (halfHeight - mouseY) / halfHeight;
		float aspect = jGraphics::getInstance()->screenWidth * 1.0f / jGraphics::getInstance()->screenHeight;
		difX *= aspect;

		DirectX::XMFLOAT4 dir = DirectX::XMFLOAT4(difX, difY, 1.0f, 0.0f);
		DirectX::XMVECTOR dirTransformed = DirectX::XMLoadFloat4(&dir);
		dirTransformed = DirectX::XMVector4Transform(dirTransformed, jActiveCam->getTransformMat());
		DirectX::XMStoreFloat4(&dir, dirTransformed);
		if (cube->isOverlappingRay(DirectX::XMFLOAT3(dir.x, dir.y, dir.z), origin))
		{
			if (cube->transformScale.x == 1.0f) cube->transformScale.x = 2.0f;
			else cube->transformScale.x = 1.0f;
			jeffAudio::getInstance()->playSound(L"testSound.wav");
		}
	}
	rootNode.handleEvent(JEFF_KEY_EVENT, &eventKey);
}

void jeffScene::handleMouseEvent(float x, float y)
{
	mouseX = x; mouseY = y;
	float coords[2] = { x, y };
	rootNode.handleEvent(JEFF_MOUSE_EVENT, coords);
}

void jeffScene::doPhysicsTick(float delta)
{
	rootNode.tick(delta);
}

void jeffScene::draw()
{
	for (jeffModel* mod : jModels) mod->draw(jPointLights, jDirLight, jActiveCam);
}