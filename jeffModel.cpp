#include "jeffModel.h"	

using namespace jeffNamespace;

jeffModel::jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf)
{
	jDev = graf.jDev; jContext = graf.jContext; jLayout = graf.jLayout; jRast = graf.jRast; width = graf.width; height = graf.height;

	mat = new jeffMaterialShader(jDev, jContext, vShaderFilename, pShaderFilename);
	mat->setProjMat((float)(height * 1.0f / width));

	mesh.loadFromObj(meshFilename);

	createRasterizer();
	createVBuf();
	createIBuf();
	createInputLayout();

	initObject();
}

void jeffModel::createRasterizer()
{
	jViewport.TopLeftX = 0.0f;
	jViewport.TopLeftY = 0.0f;
	jViewport.Width = (float)width;
	jViewport.Height = (float)height;
	jViewport.MinDepth = 0.0f;
	jViewport.MaxDepth = 1.0f;

	jRect.left = 0;
	jRect.right = width;
	jRect.top = 0;
	jRect.bottom = height;

	HRESULT hr = jDev->CreateRasterizerState(&jRDesc, &jRast);
}

void jeffModel::createVBuf()
{
	vBufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(mesh.vertices.size() * sizeof(jeffMesh::jeffVertex)), D3D11_BIND_VERTEX_BUFFER);

	vInitData.pSysMem = mesh.vertices.data();
	vInitData.SysMemPitch = 0;
	vInitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&vBufferDesc, &vInitData, &jVertBuf);
}

void jeffModel::createIBuf()
{
	iBufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(mesh.indices.size() * sizeof(int)), D3D11_BIND_INDEX_BUFFER);

	iInitData.pSysMem = mesh.indices.data();
	iInitData.SysMemPitch = 0;
	iInitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&iBufferDesc, &iInitData, &jIndexBuf);
}

void jeffModel::createInputLayout()
{
	HRESULT hr = jDev->CreateInputLayout(jLayoutDescs, 4, mat->jVShaderBlob->GetBufferPointer(), mat->jVShaderBlob->GetBufferSize(), &jLayout);
}

void jeffModel::draw()
{
	setRasterizer();
	setVBuf();
	setIBuf();
	setConstantBuffer(time);
	setShaders();
	setInputLayout();
	jContext->DrawIndexed((UINT)mesh.indices.size(), 0, 0);
}

void jeffModel::setRasterizer()
{
	jContext->RSSetViewports(1, &jViewport);
	jContext->RSSetScissorRects(1, &jRect);
	jContext->RSSetState(jRast);
}

void jeffModel::setVBuf()
{
	constexpr UINT stride = sizeof(jeffMesh::jeffVertex);
	UINT offset = 0;
	jContext->IASetVertexBuffers(0, 1, &jVertBuf, &stride, &offset);
}

void jeffModel::setIBuf()
{
	jContext->IASetIndexBuffer(jIndexBuf, DXGI_FORMAT_R32_UINT, 0);
}

void jeffModel::setConstantBuffer(float time)
{
	DirectX::XMVECTOR rot = DirectX::XMLoadFloat3(&transformRotation);
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&transformPosition);
	DirectX::XMVECTOR scale = DirectX::XMLoadFloat3(&transformScale);
	transformMat = DirectX::XMMatrixScalingFromVector(scale) * DirectX::XMMatrixRotationRollPitchYawFromVector(rot) * DirectX::XMMatrixTranslationFromVector(pos);

	mat->initConstBuf(time, transformMat);
}

void jeffModel::setInputLayout()
{
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

void jeffModel::handleKeyEvent(JEFF_KEY* key)
{
	if (mesh.name.compare("Cube") == 0)
		return;
	if (*key == W)
	{
		mat->jPConstBufStruct.pointLight.y += 0.5f;
	}
	if (*key == A)
	{
		mat->jPConstBufStruct.pointLight.x -= 0.5f;
	}
	if (*key == S)
	{
		mat->jPConstBufStruct.pointLight.y -= 0.5f;
	}
	if (*key == D)
	{
		mat->jPConstBufStruct.pointLight.x += 0.5f;
	}
	if (*key == I)
	{
		mat->jPConstBufStruct.pointLightParams.x += 0.2f;
	}
	if (*key == J)
	{
		mat->jPConstBufStruct.pointLightParams.y -= 0.2f;
	}
	if (*key == K)
	{
		mat->jPConstBufStruct.pointLightParams.x -= 0.2f;
	}
	if (*key == L)
	{
		mat->jPConstBufStruct.pointLightParams.y += 0.2f;
	}
	if (*key == U)
	{
		mat->jPConstBufStruct.pointLightParams.z -= 0.2f;
	}
	if (*key == O)
	{
		mat->jPConstBufStruct.pointLightParams.z += 0.2f;
	}
}

void jeffModel::tick(float delta)
{
	jeffObject::tick(delta);

	transformRotation.x += delta;
	transformRotation.z += delta * 0.5f;
	//transformScale.x = sinf(time) + 1;
	//transformScale.y = sinf(time + 1) + 1;
	//transformScale.z = sinf(time + 2) + 1;
}