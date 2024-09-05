#include "jeffModel.h"	
#include <DDSTextureLoader.h>

using namespace jeffNamespace;

jeffModel::jeffModel(const char* meshFilename, ID3D11Device* dev, ID3D11DeviceContext* context) : jDev(dev), jContext(context)
{
	mesh.loadFromObj(meshFilename);

	createVBuf();
	createIBuf();
	initTexture();

	initObject();
}

void jeffModel::initTexture()
{
	HRESULT hr = DirectX::CreateDDSTextureFromFile(jDev, jContext, L"testimage.dds", (ID3D11Resource**)&jDiffuseTexture, &jSRView);
	if (FAILED(hr)) throw std::runtime_error("error creating texture");

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
	if (FAILED(hr)) throw std::runtime_error("error creating sampler state");
}

void jeffModel::createVBuf()
{
	D3D11_BUFFER_DESC vBufferDesc{};
	vBufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(mesh.vertices.size() * sizeof(jeffMesh::jeffVertex)), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA vInitData{};
	vInitData.pSysMem = mesh.vertices.data();
	vInitData.SysMemPitch = 0;
	vInitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&vBufferDesc, &vInitData, &jVertBuf);
	if (FAILED(hr)) throw std::runtime_error("error creating vertex buffer");
}

void jeffModel::createIBuf()
{
	D3D11_BUFFER_DESC iBufferDesc{};
	iBufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(mesh.indices.size() * sizeof(int)), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA iInitData{};
	iInitData.pSysMem = mesh.indices.data();
	iInitData.SysMemPitch = 0;
	iInitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&iBufferDesc, &iInitData, &jIndexBuf);
	if (FAILED(hr)) throw std::runtime_error("error creating index buffer");
}

void jeffModel::draw(std::array<jeffLightPoint*, 4> lights, jeffLightDirectional* dirLight, jeffCamera* camera)
{
	for (int i = 0; i < 4; i++)
	{
		jPConstBufStruct.pointLights[i] = jeffLight::threeToFour(lights[i]->transformPosition);
		jPConstBufStruct.pointLightColours[i] = DirectX::XMFLOAT4(lights[i]->lightColour);
		jPConstBufStruct.pointLightParams[i] = jeffLight::jeffClamp(lights[i]->lightParams);
	}
	jPConstBufStruct.dirLight = jeffLight::threeToFour(dirLight->transformRotation);
	jPConstBufStruct.dirLightColour = DirectX::XMFLOAT4(dirLight->lightColour);

	setVBuf(jVertBuf);
	setIBuf(jIndexBuf);
	setConstantBuffer(time, camera);
	jContext->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
}

void jeffModel::setVBuf(ID3D11Buffer* &buf)
{
	constexpr UINT stride = sizeof(jeffMesh::jeffVertex);
	UINT offset = 0;
	jContext->IASetVertexBuffers(0, 1, &buf, &stride, &offset);
}

void jeffModel::setIBuf(ID3D11Buffer* &buf)
{
	jContext->IASetIndexBuffer(buf, DXGI_FORMAT_R32_UINT, 0);
}

void jeffModel::setConstantBuffer(float time, jeffCamera* camera)
{
	jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(getTransformMat());

	DirectX::XMMATRIX cam = camera->getTransformMat();
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(cam);
	jVConstBufStruct.cameraMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, cam));

	jVConstBufStruct.projMat = DirectX::XMMatrixTranspose(camera->projectionMatrix);

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
	if (FAILED(hr)) throw std::runtime_error("error creating vertex constant buffer");
	jContext->VSSetConstantBuffers(0, 1, &jVConstBuf);

	hr = jDev->CreateBuffer(&jPDesc, &jPInitData, &jPConstBuf);
	if (FAILED(hr)) throw std::runtime_error("error creating pixel constant buffer");
	jContext->PSSetConstantBuffers(1, 1, &jPConstBuf);

	jContext->PSSetShaderResources(0, 1, &jSRView);
	jContext->PSSetSamplers(0, 1, &jSamState);
}

void jeffModel::initObject()
{
	transformPosition.z += 5.0f;
	jeffObject::initObject();
}

void jeffModel::tick(float delta)
{
	jeffObject::tick(delta);

	transformRotation.x += delta;
	transformRotation.z += delta * 0.5f;
	transformScale.x = 0.5 * (sinf(time) + 1);
	transformScale.y = 0.5 * (sinf(time + 1) + 1);
	transformScale.z = 0.5 * (sinf(time + 2) + 1);
}