#include "jeffModel.h"	

using namespace jeffNamespace;

jeffModel::jeffModel(const char* modelName, const char* meshFilename) : jeffObject::jeffObject(modelName)
{
	jDev = jeffDeviceState::getInstance()->jDev; jContext = jeffDeviceState::getInstance()->jContext;

	loadFromObj(meshFilename);

	createRast();
	createVBuf();
	createIBuf();

	initObject();
}

jeffModel::~jeffModel()
{
	jVConstBuf->Release();
	jPConstBuf->Release();
}

// Remove duplicate vertices in future
void jeffModel::loadFromObj(const char* filename)
{
	std::string meshPath = filename;

	std::ifstream file(filename);

	jeffMesh::vertVecStruct vertStruct;

	std::string line;
	if (file.is_open())
	{
		int i = 0;

		while (std::getline(file, line))
		{
			// Get object name
			if (line[0] == 'o')
			{
				jeffMesh newMesh;
				newMesh.name = line.erase(0, 2);
				newMesh.meshPath = meshPath;
				meshes.emplace_back(newMesh);
				i = 0;
				continue;
			}

			if (line.substr(0, 6).compare("mtllib") == 0)
			{
				std::string materialPath = line.substr(7);

				size_t dir = meshPath.rfind('/');
				line = meshPath.substr(0, dir + 1);
				materialPath = line.append(materialPath);

				loadFromMtl(materialPath.c_str());
				continue;
			}

			if (line[0] == 'u')
			{
				if (line.substr(0, 6).compare("usemtl") == 0)
				{
					jeffMesh::jeffVertexGroup* newGroup = new jeffMesh::jeffVertexGroup();
					newGroup->mat = materialDictionary[line.substr(7)];
					meshes.back().groups.emplace_back(newGroup);
					i = 0;
				}
				continue;
			}

			if (meshes.empty()) continue;
			meshes.back().objProcessLine(line, i, &vertStruct);
		}

		file.close();
	}
	else
	{
		throw std::runtime_error("could not open obj");
	}
}

void jeffModel::loadFromMtl(const char* filename)
{
	std::wifstream file(filename);

	std::wstring line;

	std::wstring curMat;

	if (file.is_open())
	{
		int i = 0;

		while (std::getline(file, line))
		{
			if (line[0] == L'#') continue;

			if (line[0] == L'N')
			{
				continue;
			}

			if (line[0] == L'K')
			{
				continue;
			}

			if (line[0] == L'd')
			{
				continue;
			}

			if (line[0] == L'i')
			{
				continue;
			}

			if (line[0] == L'n')
			{
				if (line.substr(0, 6).compare(L"newmtl") == 0)
				{
					curMat = line.substr(7);
					materialDictionary[jeffJSON::fromWidestring(curMat)].name = curMat;
				}
				continue;
			}

			// diffuse texture
			if (line[0] == L'm')
			{
				if (line.substr(0, 6).compare(L"map_Kd") == 0)
				{
					std::wstring mapName = line.substr(7);
					materialDictionary[jeffJSON::fromWidestring(curMat)].initTexture(mapName.c_str(), jeffMaterial::DIFFUSE);
				}
				continue;
			}
		}

		file.close();
	}
	else
	{
		throw std::runtime_error("could not open mtl");
	}
}

void jeffModel::createVBuf()
{
	for (auto& mesh : meshes)
	{
		for (auto& group : mesh.groups)
		{
			D3D11_BUFFER_DESC vBufferDesc{};
			vBufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(group->vertices.size() * sizeof(jeffMesh::jeffVertex)), D3D11_BIND_VERTEX_BUFFER);

			D3D11_SUBRESOURCE_DATA vInitData{};
			vInitData.pSysMem = group->vertices.data();
			vInitData.SysMemPitch = 0;
			vInitData.SysMemSlicePitch = 0;

			HRESULT hr = jDev->CreateBuffer(&vBufferDesc, &vInitData, &group->jVertBuf);
			if (FAILED(hr)) throw std::runtime_error("error creating vertex buffer");
		}
	}
}

void jeffModel::createIBuf()
{
	for (auto& mesh : meshes)
	{
		for (auto& group : mesh.groups)
		{
			D3D11_BUFFER_DESC iBufferDesc{};
			iBufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(group->indices.size() * sizeof(int)), D3D11_BIND_INDEX_BUFFER);

			D3D11_SUBRESOURCE_DATA iInitData{};
			iInitData.pSysMem = group->indices.data();
			iInitData.SysMemPitch = 0;
			iInitData.SysMemSlicePitch = 0;

			HRESULT hr = jDev->CreateBuffer(&iBufferDesc, &iInitData, &group->jIndexBuf);
			if (FAILED(hr)) throw std::runtime_error("error creating index buffer");
		}
	}
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

	jContext->RSSetState(jRast);
	if (wireframe)
		jContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	else
		jContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (auto& mesh : meshes)
	{
		for (auto& group : mesh.groups)
		{
			setConstantBuffer(time, camera, group->mat);
			setVBuf(group->jVertBuf);
			setIBuf(group->jIndexBuf);
			jContext->DrawIndexed((UINT)group->indices.size(), 0, 0);
		}
	}
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

void jeffModel::setConstantBuffer(float time, jeffCamera* camera, jeffMaterial &mat)
{
	jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(getTransformMat());

	DirectX::XMMATRIX cam = camera->getTransformMat();
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(cam);
	jVConstBufStruct.cameraMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, cam));
	jVConstBufStruct.projMat = DirectX::XMMatrixTranspose(camera->projectionMatrix);
	jPConstBufStruct.wireframe = wireframe ? DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f):DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	jPConstBufStruct.reflectionConstants = mat.properties;

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

	jContext->PSSetShaderResources(0, (UINT)mat.jViews.size(), mat.jViews.data());
	jContext->PSSetSamplers(0, (UINT)mat.jSams.size(), mat.jSams.data());
}
