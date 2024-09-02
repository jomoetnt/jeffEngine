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
				jRast = nullptr;
				width = 0; height = 0;
			}
		};

		D3D11_INPUT_ELEMENT_DESC jLayoutDescs[3] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		jeffMesh mesh;
		jeffMaterialShader* mat;

		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;
		ID3D11InputLayout* jLayout = nullptr;
		ID3D11RasterizerState* jRast = nullptr;

		// Persistent per-object
		D3D11_VIEWPORT jViewport{};
		D3D11_RECT jRect{};
		ID3D11Buffer* jVertBuf = nullptr;
		ID3D11Buffer* jIndexBuf = nullptr;

		int width = 0; int height = 0;

		void createRasterizer();
		void createVBuf();
		void createIBuf();
		void createInputLayout();

		void draw(jeffLightPoint* lights, jeffLightDirectional dirLight, jeffCamera* camera);
		void setRasterizer();
		void setShaders();
		void setVBuf();
		void setIBuf();
		void setConstantBuffer(float time, jeffCamera* camera);

		void setInputLayout();

		jeffModel(const char* meshFilename, LPCWSTR vShaderFilename, LPCWSTR pShaderFilename, graphicsStruct graf);

		~jeffModel()
		{
			delete mat;
			jVertBuf->Release();
			jIndexBuf->Release();
		}

		void initObject() override;

		void handleKeyEvent(JEFF_KEY* key);

		void tick(float delta) override;

	};
}
