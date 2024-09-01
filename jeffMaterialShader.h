#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>


namespace jeffNamespace
{
	class jeffMaterialShader
	{
	public:
		struct
		{
			DirectX::XMMATRIX projMat = DirectX::XMMatrixIdentity();
			DirectX::XMMATRIX transformMat = DirectX::XMMatrixIdentity();
		} jVConstBufStruct;

		struct
		{
			DirectX::XMFLOAT4 dirLight = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			DirectX::XMFLOAT4 dirLightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
			DirectX::XMFLOAT4 pointLight = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMFLOAT4 pointLightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
			DirectX::XMFLOAT4 pointLightParams = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 0.0f);
		} jPConstBufStruct;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;


		ID3DBlob* jVShaderBlob = NULL;
		ID3DBlob* jPShaderBlob = NULL;
		ID3D11VertexShader* jVShader = NULL;
		ID3D11PixelShader* jPShader = NULL;
		ID3D11Buffer* jVConstBuf = nullptr;
		ID3D11Buffer* jPConstBuf = nullptr;
		ID3D11Texture2D* jDiffuseTexture = nullptr;
		ID3D11ShaderResourceView* jSRView = nullptr;
		ID3D11SamplerState* jSamState = nullptr;

		jeffMaterialShader() {}
		jeffMaterialShader(ID3D11Device* jDevice, ID3D11DeviceContext* jCont, LPCWSTR vFilename, LPCWSTR pFilename);

		~jeffMaterialShader()
		{
			jVShaderBlob->Release(); jPShaderBlob->Release();
			jVShader->Release(); jPShader->Release();
			jVConstBuf->Release(); jPConstBuf->Release();
			jDiffuseTexture->Release();
			jSRView->Release();
			jSamState->Release();
		}

		void initTexture();

		void setProjMat(float aspect)
		{
			jVConstBufStruct.projMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, aspect, 0.5f, 100.0f));
		}

		void initConstBuf(float time, DirectX::XMMATRIX transform);

	};
}