#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>

class jeffMaterialShader
{
public:
	void createShaders(LPCWSTR vFilename, LPCWSTR pFilename)
	{
		assert("material shader error: device not set prior to creating shaders" && jDev != nullptr);

		ID3DBlob* vErr;
		ID3DBlob* pErr;
		D3DCreateBlob(256, &vErr);
		D3DCreateBlob(256, &pErr);

		int jFlags1 = D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
#ifdef NDEBUG
		jFlags1 |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
		jFlags1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		D3DCompileFromFile(vFilename, NULL, NULL, "main", "vs_5_0", jFlags1, 0, &jVShaderBlob, &vErr);
		D3DCompileFromFile(pFilename, NULL, NULL, "main", "ps_5_0", jFlags1, 0, &jPShaderBlob, &pErr);
		
		jDev->CreateVertexShader(jVShaderBlob->GetBufferPointer(), jVShaderBlob->GetBufferSize(), nullptr, &jVShader);
		jDev->CreatePixelShader(jPShaderBlob->GetBufferPointer(), jPShaderBlob->GetBufferSize(), nullptr, &jPShader);
	}

	void releaseResources()
	{
		jVShaderBlob->Release();
		jPShaderBlob->Release();
		jVShader->Release();
		jPShader->Release();
	}

	ID3D11Device* jDev = nullptr;

	ID3DBlob* jVShaderBlob;
	ID3DBlob* jPShaderBlob;
	ID3D11VertexShader* jVShader;
	ID3D11PixelShader* jPShader;
};