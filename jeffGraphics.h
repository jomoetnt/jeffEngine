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
		jGraphics(HWND handle, int width, int height);
		~jGraphics();

		float time = 0.0f;
		float delta = 0.0f;
		int frameRate = 0;
		int width = 0; int height = 0;

		ID3D11Device* jDev;
		ID3D11DeviceContext* jContext;
		ID3D11InputLayout* jLayout;
		ID3D11RasterizerState* jRast;

		void draw3D(jeffModel* jModel);
		void draw2D();

		void endFrame();

	private:
		HWND hwnd;

		IDXGISwapChain* jSwap;
		IDXGISurface* jSurface;

		// D3D
		ID3D11RenderTargetView* jRTarget;
		ID3D11Texture2D* jBackBuf = nullptr;
		ID3D11Texture2D* jDepthStencil = NULL;
		ID3D11DepthStencilView* jDSV;
		ID3D11DepthStencilState* jDepthState;

		// D2D and DirectWrite
		ID2D1Factory* jD2DFactory;
		ID2D1RenderTarget* jRT;
		ID2D1SolidColorBrush* jBrush;
		IDWriteFactory* jWriteFactory;
		IDWriteTextFormat* jTextFormat;


		// Shared
		void initDevice();
		void init2D();
		void makeSwapchain();
		void makeRenderTarget();
		void makeZBuf();
	};
}
