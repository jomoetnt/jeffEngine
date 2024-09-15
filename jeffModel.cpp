#include "jeffModel.h"	

using namespace jeffNamespace;

jeffModel::jeffModel(const char* modelName, const char* meshFilename) : jeffObject::jeffObject(modelName)
{
	jDev = jeffDeviceState::getInstance()->jDev; jContext = jeffDeviceState::getInstance()->jContext;

	mesh.loadFromObj(meshFilename);
	if (!mesh.materialPath.empty())
		mat.loadFromMtl(mesh.materialPath.c_str());

	createRast();
	createVBuf();
	createIBuf();

	initObject();
}

jeffModel::~jeffModel()
{
	jVertBuf->Release();
	jIndexBuf->Release();
	jVConstBuf->Release();
	jPConstBuf->Release();
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

void jeffModel::createRast()
{
	D3D11_CULL_MODE mode = transformScale.x * transformScale.y * transformScale.z < 0.0f ? D3D11_CULL_BACK: D3D11_CULL_FRONT;
	D3D11_RASTERIZER_DESC jRDesc = CD3D11_RASTERIZER_DESC(wireframe ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID, wireframe ? D3D11_CULL_NONE : mode, true, 0, 0, 0, true, true, false, false);
	HRESULT hr = jDev->CreateRasterizerState(&jRDesc, &jRast);
	if (FAILED(hr)) throw std::runtime_error("error creating rasterizer state");
}

void jeffModel::draw(std::array<jeffLightPoint*, 4> lights, jeffLightDirectional* dirLight, jeffCamera* camera)
{
	// four nearest point lights
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
	jContext->RSSetState(jRast);
	if (wireframe)
		jContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	else
		jContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
	jPConstBufStruct.wireframe = wireframe ? DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f):DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);

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

	jContext->PSSetShaderResources(0, mat.jViews.size(), mat.jViews.data());
	jContext->PSSetSamplers(0, mat.jSams.size(), mat.jSams.data());
}
