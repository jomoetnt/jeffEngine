#include "jeffModel.h"	

using namespace jeffNamespace;

void jeffModel::initObject()
{
	transformPosition.z += 5.0f;
}

void jeffModel::tick(float delta)
{
	transformRotation.x += delta;
	transformRotation.z += delta * 0.5f;
	time += delta;
}

void jeffModel::prepDraw()
{
	setVBuf();
	setIBuf();
	setConstantBuffer(time);
	setShaders();
}

void jeffModel::setVBuf()
{
	D3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(mesh.vertices.size() * sizeof(jeffMesh::jeffVertex)), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = mesh.vertices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&bufferDesc, &InitData, &jVertBuf);

	UINT stride = sizeof(jeffMesh::jeffVertex);
	UINT offset = 0;
	jContext->IASetVertexBuffers(0, 1, &jVertBuf, &stride, &offset);
}

void jeffModel::setIBuf()
{
	D3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(mesh.indices.size() * sizeof(int)), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData{};
	InitData.pSysMem = mesh.indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&bufferDesc, &InitData, &jIndexBuf);

	jContext->IASetIndexBuffer(jIndexBuf, DXGI_FORMAT_R32_UINT, 0);
}

void jeffModel::setConstantBuffer(float time)
{
	DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&transformRotation);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transformPosition);
	transformMat = DirectX::XMMatrixRotationRollPitchYawFromVector(rot) * DirectX::XMMatrixTranslationFromVector(pos);
	mat->initConstBuf(aspectRatio, time, transformMat);
}

void jeffModel::setShaders()
{
	jContext->VSSetShader(mat->jVShader, 0, 0);
	jContext->PSSetShader(mat->jPShader, 0, 0);
}