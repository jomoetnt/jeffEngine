#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
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

	double delta = 0.0;
private:
	HWND hwnd;
	ID3D11Device* jDev;
	IDXGISwapChain* jSwap;
	ID3D11DeviceContext* jContext;
	ID3D11RenderTargetView* jRTarget;
	ID3D11BlendState* jBlendState;
	ID3D11DepthStencilState* jDepthState;
	ID3D11RasterizerState* jRast;
	ID3D11InputLayout* jLayout;
	ID3D11Buffer* jVertBuf;
	ID3D11Buffer* jIndexBuf;
	ID3D11Buffer* jVConstBuf;
	ID3D11Buffer* jPConstBuf;

	jeffMesh testMesh;
	jeffMaterialShader testMat;

	int width; int height;
	
	double time = 0.0;

	void initDevice();
	void setScreenSize();
	void makeSwapchain();
	void makeRenderTarget();

	void setShaders(jeffMaterialShader mat);
	void setRasterizer();
	void setVBuf(jeffMesh jMesh);
	void setIBuf(jeffMesh jMesh);
	void setConstantBuffer();
	void setInputLayout();
};