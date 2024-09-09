#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include <vector>
#include "jeffModel.h"

namespace jeffNamespace
{
	class jGraphics
	{
	public:
		HWND hwnd;

		int screenWidth = 0; int screenHeight = 0;

		ID3D11Device* jDev;
		ID3D11DeviceContext* jContext;

		void draw2D(DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 start);

		void beginFrame();
		void endFrame();

		static void makeInstance(HWND handle);
		static jGraphics* getInstance() { return instance; }
		static void destroyInstance() { delete instance; }

	private:
		static inline jGraphics* instance;

		D3D11_INPUT_ELEMENT_DESC jLayoutDescs[3] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		RECT screenSize{};

		IDXGISwapChain* jSwap;
		IDXGISurface* jSurface;

		// D3D
		ID3D11InputLayout* jLayout = nullptr;
		//ID3D11RasterizerState* jRast = nullptr;
		ID3D11RenderTargetView* jRTarget = nullptr;
		ID3D11Texture2D* jBackBuf = nullptr;
		ID3D11Texture2D* jDepthStencil = NULL;
		ID3D11DepthStencilView* jDSV;
		ID3D11DepthStencilState* jDepthState;
		ID3DBlob* jVShaderBlob = NULL;
		ID3DBlob* jPShaderBlob = NULL;
		ID3D11VertexShader* jVShader = NULL;
		ID3D11PixelShader* jPShader = NULL;

		// D2D and DirectWrite
		ID2D1Factory* jD2DFactory;
		ID2D1RenderTarget* jRT;
		ID2D1SolidColorBrush* jBrush;
		IDWriteFactory* jWriteFactory;
		IDWriteTextFormat* jTextFormat;


		jGraphics() = default;
		~jGraphics();
		// Shared
		void initDevice();
		void init2D();
		void makeSwapchain();
		void makeRenderTarget();
		void makeZBuf();
		void createInputLayout();
		void createRasterizer();

	};
}
