#include "jeffModel.h"	

using namespace jeffNamespace;

jeffModel::jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf)
{
	jDev = graf.jDev; jContext = graf.jContext; jLayout = graf.jLayout; jRast = graf.jRast; width = graf.width; height = graf.height;

	mat = new jeffMaterialShader(jDev, jContext, vShaderFilename, pShaderFilename);

	mesh.loadFromObj(meshFilename);

	initObject();
}

void jeffModel::draw()
{
	setRasterizer();
	setVBuf();
	setIBuf();
	setConstantBuffer(time);
	setShaders();
	setInputLayout();
	jContext->DrawIndexed(mesh.indices.size(), 0, 0);
}

void jeffModel::setRasterizer()
{
	D3D11_VIEWPORT jViewport{};
	jViewport.TopLeftX = 0.0f;
	jViewport.TopLeftY = 0.0f;
	jViewport.Width = (float)width;
	jViewport.Height = (float)height;
	jViewport.MinDepth = 0.0f;
	jViewport.MaxDepth = 1.0f;
	jContext->RSSetViewports(1, &jViewport);

	D3D11_RECT jRect{};
	jRect.left = 0;
	jRect.right = width;
	jRect.top = 0;
	jRect.bottom = height;
	jContext->RSSetScissorRects(1, &jRect);

	D3D11_RASTERIZER_DESC jRDesc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_BACK, true, 0, 0, 0, false, false, false, false);
	HRESULT hr = jDev->CreateRasterizerState(&jRDesc, &jRast);
	jContext->RSSetState(jRast);
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
	DirectX::XMVECTOR scale = DirectX::XMLoadFloat3(&transformScale);
	transformMat = DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationRollPitchYawFromVector(rot) * DirectX::XMMatrixTranslationFromVector(pos);
	mat->initConstBuf((float)(height * 1.0f / width), time, transformMat);
}

void jeffModel::setInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC jLayoutDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr = jDev->CreateInputLayout(jLayoutDescs, sizeof(jLayoutDescs) / sizeof(D3D11_INPUT_ELEMENT_DESC), mat->jVShaderBlob->GetBufferPointer(), mat->jVShaderBlob->GetBufferSize(), &jLayout);
	jContext->IASetInputLayout(jLayout);
}

void jeffModel::setShaders()
{
	jContext->VSSetShader(mat->jVShader, 0, 0);
	jContext->PSSetShader(mat->jPShader, 0, 0);
}

void jeffModel::initObject()
{
	transformPosition.z += 5.0f;
}

void jeffModel::tick(float delta)
{
	jeffObject::tick(delta);

	transformRotation.x += delta;
	transformRotation.z += delta * 0.5f;
	//transformScale.x = sinf(time);
	//transformScale.y = sinf(time + 1);
	//transformScale.z = sinf(time + 2);
}