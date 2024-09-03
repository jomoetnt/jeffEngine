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
	struct graphicsStruct
	{
		ID3D11Device* jDev;
		ID3D11DeviceContext* jContext;
		int width; int height;

		graphicsStruct(ID3D11Device* jDevice, ID3D11DeviceContext* jCont, int sWidth, int sHeight)
		{
			jDev = jDevice; jContext = jCont; width = sWidth; height = sHeight;
		}

		graphicsStruct()
		{
			jDev = nullptr;
			jContext = nullptr;
			width = 0; height = 0;
		}
	};

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
		} jPConstBufStruct;

		jeffMesh mesh;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;
		ID3D11Buffer* jVConstBuf = nullptr;
		ID3D11Buffer* jPConstBuf = nullptr;
		ID3D11Texture2D* jDiffuseTexture = nullptr;
		ID3D11ShaderResourceView* jSRView = nullptr;
		ID3D11SamplerState* jSamState = nullptr;

		ID3D11Buffer* jVertBuf = nullptr;
		ID3D11Buffer* jIndexBuf = nullptr;

		int width = 0; int height = 0;

		void createVBuf();
		void createIBuf();
		void initTexture();


		void draw(std::array<jeffLightPoint*, 4> lights, jeffLightDirectional* dirLight, jeffCamera* camera);
		void setVBuf();
		void setIBuf();
		void setConstantBuffer(float time, jeffCamera* camera);


		jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf);

		~jeffModel()
		{
			jVertBuf->Release();
			jIndexBuf->Release();
			jVConstBuf->Release();
			jPConstBuf->Release();
			jDiffuseTexture->Release();
			jSRView->Release();
			jSamState->Release();
		}

		void initObject() override;

		void tick(float delta) override;

	};
}
