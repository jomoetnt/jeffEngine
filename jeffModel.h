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

		void draw(std::array<jeffLightPoint*, 4> lights, jeffLightDirectional* dirLight, jeffCamera* camera);
		void setVBuf(ID3D11Buffer* &buf);
		void setIBuf(ID3D11Buffer*& buf);
		void setConstantBuffer(float time, jeffCamera* camera, jeffMaterial &mat);

		jeffModel(const char* modelName, const char* meshFilename);
		jeffModel() = default;
		~jeffModel();


	};
}
