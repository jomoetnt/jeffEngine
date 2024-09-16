#pragma once

#include <DirectXMath.h>
#include "jeffJSON.h"
#include "jeffDeviceState.h"
#include "jeffMaterial.h"

namespace jeffNamespace
{
	class jeffMesh
	{
	public:
		enum JEFF_TOPOLOGY
		{
			TRIANGLE, LINE
		};
		struct jeffVertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT2 texcoord;
		};
		struct jeffVertexGroup
		{
			ID3D11Buffer* jVertBuf = nullptr;
			ID3D11Buffer* jIndexBuf = nullptr;

			std::vector<jeffVertex> vertices;
			std::vector<int> indices; 

			jeffMaterial mat;

			~jeffVertexGroup()
			{
				jVertBuf->Release();
				jIndexBuf->Release();
			}
		}; 
		struct vertVecStruct
		{
			std::vector<DirectX::XMFLOAT3> vpositions;
			std::vector<DirectX::XMFLOAT3> vnormals;
			std::vector<DirectX::XMFLOAT2> vtexcoords;
		};

		std::vector<jeffVertexGroup*> groups;

		std::string name;
		std::string meshPath;

		bool smoothShading = false;
		JEFF_TOPOLOGY meshTopology = TRIANGLE;

		void objProcessLine(std::string line, int& i, vertVecStruct* vertStruct);
		void handleFace(std::string line, int& i, vertVecStruct* vertStruct);

	};
}