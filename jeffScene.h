#pragma once

#include "jeffModel.h"
#include "jeffPhysicsObject.h"

namespace jeffNamespace
{
	class jeffScene
	{
	public:
		jeffObject rootNode;
		jeffCamera* jActiveCam = nullptr;
		std::array<jeffLightPoint*, 4> jPointLights;
		jeffLightDirectional* jDirLight;
		std::vector<jeffModel*> jModels;
		jeffPhysicsObject* cube = nullptr;
		jeffModel rayModel;

		float mouseX, mouseY;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;

		jeffScene(ID3D11Device* dev, ID3D11DeviceContext* context) : jDev(dev), jContext(context)
		{
			initObjects();
		}

		~jeffScene()
		{
			delete jActiveCam;
			delete jDirLight;
			delete cube;
			for (jeffModel* mod : jModels) delete mod;
			for (jeffLightPoint* light : jPointLights) delete light;
		}

		void initObjects()
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

			jeffModel* jModel = new jeffModel("cube.obj", jDev, jContext);
			jeffModel* jModel2 = new jeffModel("sphere.obj", jDev, jContext);
			jModel2->transformPosition.z += 7.0f;
			jModel2->transformPosition.x += 2.0f;

			jeffMesh::jeffVertex vert{};
			vert.position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vert.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vert.texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			rayModel.mesh.vertices.emplace_back(vert);
			jeffMesh::jeffVertex vert2{};
			vert2.position = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vert2.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vert2.texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			rayModel.mesh.vertices.emplace_back(vert2);
			jeffMesh::jeffVertex vert3{};
			vert3.position = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			vert3.normal = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
			vert3.texcoord = DirectX::XMFLOAT2(0.0f, 0.0f);
			rayModel.mesh.vertices.emplace_back(vert3);
			rayModel.mesh.indices.emplace_back(0); rayModel.mesh.indices.emplace_back(1); rayModel.mesh.indices.emplace_back(2);
			rayModel.wireframe = true;
			rayModel.mesh.name = "jeffRay"; 
			rayModel.jDev = jDev;
			rayModel.jContext = jContext;
			rayModel.createVBuf();
			rayModel.createIBuf();
			rayModel.initTexture();
			rayModel.initObject();
			jModels.emplace_back(&rayModel);

			jModels.emplace_back(jModel); jModels.emplace_back(jModel2);
			cube->addChild(jModel); rootNode.addChild(jModel2); rootNode.addChild(&rayModel);

			rootNode.initObject();
		}

		void handleKeyEvent(JEFF_KEY& eventKey)
		{
			if (eventKey == LEFTCLICK)
			{
				DirectX::XMFLOAT3 origin = jActiveCam->transformPosition;
				
				float difX = (960.0f - mouseX) / 1920.0f; float difY = (540.0f - mouseY) / 1080.0f;
				//float fovX = DirectX::XM_PIDIV2;
				//float aspect = 1920.0f / 1080.0f;
				//float fovY = fovX / aspect;

				DirectX::XMFLOAT3 dir = DirectX::XMFLOAT3(difX, difY, 1.0f);
				if (cube->isOverlappingRay(dir, origin))
				{
					cube->transformScale.x = 2.0f;
				}
			}
			rootNode.handleEvent(JEFF_KEY_EVENT, &eventKey);
		}

		void handleMouseEvent(float x, float y)
		{
			mouseX = x; mouseY = y;
			float coords[2] = { x, y };
			rootNode.handleEvent(JEFF_MOUSE_EVENT, coords);
		}

		void doPhysicsTick(float delta)
		{
			rootNode.tick(delta);
		}

		void draw()
		{
			rayModel.transformPosition = cube->testRayStart;

			DirectX::XMFLOAT3 upfloat = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
			DirectX::XMFLOAT3 forwardfloat = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			DirectX::XMVECTOR dir = DirectX::XMLoadFloat3(&cube->testRayDir);
			DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&upfloat);
			DirectX::XMMATRIX lookat = DirectX::XMMatrixLookAtLH(DirectX::XMVectorZero(), dir, up);
			up = DirectX::XMLoadFloat3(&forwardfloat);
			up = DirectX::XMVector3Transform(up, lookat);
			DirectX::XMStoreFloat3(&rayModel.transformRotation, up);

			for (jeffModel* mod : jModels) mod->draw(jPointLights, jDirLight, jActiveCam);
		}

	};
}