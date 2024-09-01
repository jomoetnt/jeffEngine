#pragma once

#include <DirectXMath.h>
#include "jeffHelper.h"
#include <fstream>
#include <iostream>

namespace jeffNamespace
{
	class jeffMesh
	{
	public:
		struct jeffVertex
		{
			DirectX::XMFLOAT3 position;
			DirectX::XMFLOAT2 texcoord;
			DirectX::XMFLOAT3 normal;
		};

		std::vector<jeffVertex> vertices;
		std::vector<DirectX::XMFLOAT3> vpositions;
		std::vector<DirectX::XMFLOAT2> vtexcoords;
		std::vector<DirectX::XMFLOAT3> vnormals;
		std::vector<int> indices;

		std::string name;

		bool smoothShading = false;

		void loadFromObj(const char* filename);

		void objProcessLine(std::string line, int& i);

		void handleFace(std::string line, int& i);
	};
}