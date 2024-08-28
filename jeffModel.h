#pragma once

#include "jeffObject.h"
#include "jeffMesh.h"
#include "jeffMaterialShader.h"

namespace jeffNamespace
{
	class jeffModel : public jeffObject
	{
	public:
		struct graphicsStruct
		{
			ID3D11Device* jDev;
			ID3D11DeviceContext* jContext;
			ID3D11InputLayout* jLayout;
			jeffMaterialShader* jMat;
			int width; int height;

			graphicsStruct(ID3D11Device* jDevice, ID3D11DeviceContext* jCont, ID3D11InputLayout*& jILayout, jeffMaterialShader*& mat, int sWidth, int sHeight)
			{
				jDev = jDevice; jContext = jCont; jLayout = jILayout; jMat = mat; width = sWidth; height = sHeight;
			}

			graphicsStruct()
			{
				jDev = nullptr;
				jContext = nullptr;
				jLayout = nullptr;
				width = 0; height = 0;
			}
		};

		jeffMesh mesh;
		jeffMaterialShader* mat;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;
		ID3D11InputLayout* jLayout = nullptr;

		ID3D11Buffer* jVertBuf = nullptr;
		ID3D11Buffer* jIndexBuf = nullptr;

		float aspectRatio = 16.0f / 9.0f;
		float time = 0.0f;

		void prepDraw();
		void setShaders();
		void setVBuf();
		void setIBuf();
		void setConstantBuffer(float time);

		jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf)
		{
			jDev = graf.jDev;
			jContext = graf.jContext;
			jLayout = graf.jLayout;
			mat = graf.jMat;
			aspectRatio = (float)(graf.width * 1.0f / graf.height);

			mesh.loadFromObj(meshFilename);

			initObject();
		}

		~jeffModel()
		{
			jVertBuf->Release();
			jIndexBuf->Release();
		}

		void initObject() override;

		void tick(float delta) override;

	};
}
