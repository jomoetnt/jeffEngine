#pragma once

#include "jeffMesh.h"
#include "jeffCamera.h"
#include "jeffLightPoint.h"
#include "jeffLightDirectional.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <array>

namespace jeffNamespace
{
	class jeffModel : public jeffObject
	{
	public:
		struct
		{
			DirectX::XMMATRIX projMat = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX transformMat = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX cameraMat = DirectX::XMMatrixIdentity();
		} jVConstBufStruct;

		struct
		{
			DirectX::XMFLOAT4 pointLights[4] = { DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) };
			DirectX::XMFLOAT4 pointLightColours[4] = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f) };
			DirectX::XMFLOAT4 pointLightParams[4] = { DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f), DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f) };
			DirectX::XMFLOAT4 dirLight = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			DirectX::XMFLOAT4 dirLightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
			DirectX::XMFLOAT4 wireframe = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMFLOAT4 reflectionConstants = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		} jPConstBufStruct;

		std::vector<jeffMesh> meshes;
		std::unordered_map<std::string, jeffMaterial> materialDictionary;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;
		ID3D11RasterizerState* jRast = nullptr;
		ID3D11Buffer* jVConstBuf = nullptr;
		ID3D11Buffer* jPConstBuf = nullptr;

		bool wireframe = false;

		void createVBuf();
		void createIBuf();
		void createRast();

		void loadFromObj(const char* filename);
		void loadFromMtl(const char* filename);
		void loadFromGlb(const char* filename);

		void draw(std::array<jeffLightPoint*, 4> lights, jeffLightDirectional* dirLight, jeffCamera* camera);
		void setVBuf(ID3D11Buffer* &buf);
		void setIBuf(ID3D11Buffer*& buf);
		void setConstantBuffer(float time, jeffCamera* camera, jeffMaterial &mat);

		float* mutableParams[4];
		int curParam = 0;

		// temporary test
		void handleInputEvent(JEFF_KEY key, float* coords, bool keydown) override
		{
			for (auto& mesh : meshes)
			{
				for (auto& group : mesh.groups)
				{
					mutableParams[0] = &group->mat.properties.z; mutableParams[1] = &group->mat.properties.y; mutableParams[2] = &group->mat.properties.x;
					mutableParams[3] = &group->mat.properties.w;
				}
			}
			switch (key)
			{
			case W:
				*mutableParams[curParam] += 0.1f;
				break;
			case S:
				*mutableParams[curParam] -= 0.1f;
				break;
			case D:
				curParam++;
				if (curParam >= 4)
					curParam = 0;
				break;
			case A:
				curParam--;
				if (curParam <= 0)
					curParam = 3;
				break;
			}
		}

		jeffModel(const char* modelName, const char* meshFilename);
		jeffModel() = default;
		~jeffModel();


	};
}
