#include "jeffMaterialShader.h"
#include <DDSTextureLoader.h>

using namespace jeffNamespace;

jeffMaterialShader::jeffMaterialShader(ID3D11Device* jDevice, ID3D11DeviceContext* jCont, LPCWSTR vFilename, LPCWSTR pFilename)
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

	initTexture();

	hr = jDev->CreateVertexShader(jVShaderBlob->GetBufferPointer(), jVShaderBlob->GetBufferSize(), nullptr, &jVShader);
	hr = jDev->CreatePixelShader(jPShaderBlob->GetBufferPointer(), jPShaderBlob->GetBufferSize(), nullptr, &jPShader);
}

void jeffMaterialShader::initTexture()
{
	HRESULT hr = DirectX::CreateDDSTextureFromFile(jDev, jContext, L"testimage.dds", (ID3D11Resource**)&jDiffuseTexture, &jSRView);

	D3D11_SAMPLER_DESC jSamplerDesc{};
	jSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	jSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	jSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	jSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	jSamplerDesc.MipLODBias = 0;
	jSamplerDesc.MaxAnisotropy = 1;
	jSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
	jSamplerDesc.MinLOD = 0;
	jSamplerDesc.MaxLOD = 0;
	hr = jDev->CreateSamplerState(&jSamplerDesc, &jSamState);
}

void jeffMaterialShader::initConstBuf(float time, DirectX::XMMATRIX transform)
{
	jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(transform);

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

	jContext->PSSetShaderResources(0, 1, &jSRView);
	jContext->PSSetSamplers(0, 1, &jSamState);
}