#pragma once

#include <DirectXMath.h>
#include "jeffJSON.h"
#include "jeffDeviceState.h"

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

		std::vector<jeffVertex> vertices;
		std::vector<DirectX::XMFLOAT3> vpositions;
		std::vector<DirectX::XMFLOAT2> vtexcoords;
		std::vector<DirectX::XMFLOAT3> vnormals;
		std::vector<int> indices;

		std::string name;

		bool smoothShading = false;
		JEFF_TOPOLOGY meshTopology = TRIANGLE;

		void loadFromObj(const char* filename);
		void objProcessLine(std::string line, int& i);
		void handleFace(std::string line, int& i);
	};
}