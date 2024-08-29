#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>


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
			DirectX::XMFLOAT4 dirLight = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
			DirectX::XMFLOAT4 dirLightColour = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
		} jPConstBufStruct;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;


		ID3DBlob* jVShaderBlob = NULL;
		ID3DBlob* jPShaderBlob = NULL;
		ID3D11VertexShader* jVShader = NULL;
		ID3D11PixelShader* jPShader = NULL;
		ID3D11Buffer* jVConstBuf = nullptr;
		ID3D11Buffer* jPConstBuf = nullptr;

		jeffMaterialShader() {}
		jeffMaterialShader(ID3D11Device* jDevice, ID3D11DeviceContext* jCont, LPCWSTR vFilename, LPCWSTR pFilename)
		{
			jDev = jDevice; jContext = jCont;

			ID3DBlob* vErr; ID3DBlob* pErr;
			D3DCreateBlob(256, &vErr); D3DCreateBlob(256, &pErr);

			int jFlags1 = D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
#ifdef NDEBUG
			jFlags1 |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
			jFlags1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

			HRESULT hr = D3DCompileFromFile(vFilename, NULL, NULL, "main", "vs_5_0", jFlags1, 0, &jVShaderBlob, &vErr);
			hr = D3DCompileFromFile(pFilename, NULL, NULL, "main", "ps_5_0", jFlags1, 0, &jPShaderBlob, &pErr);

			jDev->CreateVertexShader(jVShaderBlob->GetBufferPointer(), jVShaderBlob->GetBufferSize(), nullptr, &jVShader);
			jDev->CreatePixelShader(jPShaderBlob->GetBufferPointer(), jPShaderBlob->GetBufferSize(), nullptr, &jPShader);
		}

		~jeffMaterialShader()
		{
			jVShaderBlob->Release();
			jPShaderBlob->Release();
			jVShader->Release();
			jPShader->Release();
			jVConstBuf->Release();
			jPConstBuf->Release();
		}

		void initConstBuf(float aspect, float time, DirectX::XMMATRIX transform)
		{
			struct
			{
				DirectX::XMMATRIX projMat;
				DirectX::XMMATRIX transformMat;
			} jVConstBufStruct;
			jVConstBufStruct.projMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, aspect, 0.5f, 100.0f));
			jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(transform);

			struct
			{
				DirectX::XMFLOAT4 dirLight;
				DirectX::XMFLOAT4 dirLightColour;
			} jPConstBufStruct;
			jPConstBufStruct.dirLight = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
			jPConstBufStruct.dirLightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

			D3D11_BUFFER_DESC jVDesc{};
			jVDesc.ByteWidth = sizeof(jVConstBufStruct);
			jVDesc.Usage = D3D11_USAGE_DYNAMIC;
			jVDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			jVDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			jVDesc.MiscFlags = 0;
			jVDesc.StructureByteStride = 0;

			D3D11_BUFFER_DESC jPDesc{};
			jPDesc.ByteWidth = sizeof(jPConstBufStruct);
			jPDesc.Usage = D3D11_USAGE_DYNAMIC;
			jPDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			jPDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			jPDesc.MiscFlags = 0;
			jPDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA jVInitData{};
			jVInitData.pSysMem = &jVConstBufStruct;
			jVInitData.SysMemPitch = 0;
			jVInitData.SysMemSlicePitch = 0;

			D3D11_SUBRESOURCE_DATA jPInitData{};
			jPInitData.pSysMem = &jPConstBufStruct;
			jPInitData.SysMemPitch = 0;
			jPInitData.SysMemSlicePitch = 0;

			HRESULT hr = jDev->CreateBuffer(&jVDesc, &jVInitData, &jVConstBuf);
			jContext->VSSetConstantBuffers(0, 1, &jVConstBuf);

			hr = jDev->CreateBuffer(&jPDesc, &jPInitData, &jPConstBuf);
			jContext->PSSetConstantBuffers(1, 1, &jPConstBuf);
		}
	};
}