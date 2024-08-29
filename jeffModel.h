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
			ID3D11RasterizerState* jRast;
			int width; int height;

			graphicsStruct(ID3D11Device* jDevice, ID3D11DeviceContext* jCont, ID3D11InputLayout* &jILayout, ID3D11RasterizerState* &jR, int sWidth, int sHeight)
			{
				jDev = jDevice; jContext = jCont; jLayout = jILayout; width = sWidth; height = sHeight; jRast = jR;
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
		ID3D11RasterizerState* jRast = nullptr;

		ID3D11Buffer* jVertBuf = nullptr;
		ID3D11Buffer* jIndexBuf = nullptr;

		int width = 0; int height = 0;

		void draw();
		void setRasterizer();
		void setShaders();
		void setVBuf();
		void setIBuf();
		void setConstantBuffer(float time);

		void setInputLayout();

		jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf);

		~jeffModel()
		{
			delete mat;
			jVertBuf->Release();
			jIndexBuf->Release();
		}

		void initObject() override;

		void tick(float delta) override;

	};
}
