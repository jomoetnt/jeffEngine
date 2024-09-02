#include "jeffModel.h"	

using namespace jeffNamespace;

jeffModel::jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf)
{
	jDev = graf.jDev; jContext = graf.jContext; jLayout = graf.jLayout; jRast = graf.jRast; width = graf.width; height = graf.height;

	mat = new jeffMaterialShader(jDev, jContext, vShaderFilename, pShaderFilename);

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

	D3D11_RASTERIZER_DESC jRDesc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_BACK, true, 0, 0, 0, false, false, false, false);
	HRESULT hr = jDev->CreateRasterizerState(&jRDesc, &jRast);
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
}

void jeffModel::createInputLayout()
{
	HRESULT hr = jDev->CreateInputLayout(jLayoutDescs, sizeof(jLayoutDescs) / sizeof(D3D11_INPUT_ELEMENT_DESC), mat->jVShaderBlob->GetBufferPointer(), mat->jVShaderBlob->GetBufferSize(), &jLayout);
}

void jeffModel::draw(jeffLightPoint* lights, jeffLightDirectional dirLight, jeffCamera* camera)
{
	for (int i = 0; i < 4; i++)
	{
		mat->jPConstBufStruct.pointLights[i] = jeffLight::threeToFour(lights[i].transformPosition);
		mat->jPConstBufStruct.pointLightColours[i] = DirectX::XMFLOAT4(lights[i].lightColour);
		mat->jPConstBufStruct.pointLightParams[i] = jeffLight::jeffClamp(lights[i].lightParams);
	}
	mat->jPConstBufStruct.dirLight = jeffLight::threeToFour(dirLight.transformRotation);
	mat->jPConstBufStruct.dirLightColour = DirectX::XMFLOAT4(dirLight.lightColour);

	setRasterizer();
	setVBuf();
	setIBuf();
	setConstantBuffer(time, camera);
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

void jeffModel::setConstantBuffer(float time, jeffCamera* camera)
{
	mat->initConstBuf(time, getTransformMat(), camera);
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