#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <DirectXMath.h>
#include <vector>
#include "jeffMaterialShader.h"
#include "jeffMesh.h"

class jGraphics 
{
public:
	jGraphics(HWND handle);
	~jGraphics();

	void jDraw();

	float time = 0.0f;
	float delta = 0.0f;
	int frameRate = 0;

	float offset = 0.0f;

private:
	HWND hwnd;

	IDXGISwapChain* jSwap;
	IDXGISurface* jSurface;

	// D3D
	ID3D11Device* jDev;
	ID3D11DeviceContext* jContext;
	ID3D11RenderTargetView* jRTarget;
	ID3D11Texture2D* jBackBuf = nullptr;
	ID3D11Texture2D* jDepthStencil = NULL;
	ID3D11DepthStencilView* jDSV;
	ID3D11DepthStencilState* jDepthState;
	ID3D11RasterizerState* jRast;
	ID3D11InputLayout* jLayout;
	ID3D11Buffer* jVertBuf;
	ID3D11Buffer* jIndexBuf;
	ID3D11Buffer* jVConstBuf;
	ID3D11Buffer* jPConstBuf;

	// D2D
	ID2D1Factory* jD2DFactory;
	ID2D1RenderTarget* jRT;
	ID2D1SolidColorBrush* jBrush;

	// DirectWrite
	IDWriteFactory* jWriteFactory;
	IDWriteTextFormat* jTextFormat;


	jeffMesh testMesh;
	jeffMesh testMesh2;
	jeffMaterialShader testMat;

	RECT screenSize; int width; int height;


	// Shared
	void initDevice();
	void init2D();
	void setScreenSize();
	void makeSwapchain();
	void makeRenderTarget();
	void makeZBuf();

	// Per object
	void draw3D();
	void draw2D();
	void setShaders(jeffMaterialShader mat);
	void setRasterizer();
	void setVBuf(jeffMesh jMesh);
	void setIBuf(jeffMesh jMesh);
	void setConstantBuffer(float cubenum);
	void setInputLayout();
};