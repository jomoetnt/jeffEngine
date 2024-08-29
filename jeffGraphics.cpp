#include "jeffGraphics.h"
#include <stdio.h>

using namespace jeffNamespace;

//--------------------------------------------------------
// Initialization
//--------------------------------------------------------

jGraphics::jGraphics(HWND handle, int width, int height)
{
	hwnd = handle;
	initDevice();
}

jGraphics::~jGraphics()
{
	// D2D and DirectWrite
	jWriteFactory->Release();
	jTextFormat->Release();
	jRT->Release();
	jBrush->Release();
	jD2DFactory->Release();

	// D3D
	jBackBuf->Release();
	jDepthStencil->Release();
	jDSV->Release();
	jLayout->Release();
	jRast->Release();
	jDepthState->Release();
	jRTarget->Release();
	jContext->Release();
	jDev->Release();

	jSurface->Release();
	jSwap->Release();
}

void jGraphics::initDevice()
{
	makeSwapchain();
	makeRenderTarget();

	init2D();

	jContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void jGraphics::makeSwapchain()
{
	DXGI_SWAP_CHAIN_DESC jSDesc{};
	jSDesc.BufferDesc.Width = 0; jSDesc.BufferDesc.Height = 0;
	jSDesc.BufferDesc.RefreshRate.Numerator = 0; jSDesc.BufferDesc.RefreshRate.Denominator = 0;
	jSDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	jSDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	jSDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	jSDesc.SampleDesc.Count = 1;
	jSDesc.SampleDesc.Quality = 0;
	jSDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	jSDesc.BufferCount = 1;
	jSDesc.OutputWindow = hwnd;
	jSDesc.Windowed = true;
	jSDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	jSDesc.Flags = 0;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &jSDesc, &jSwap, &jDev, nullptr, &jContext);
}

void jGraphics::makeRenderTarget()
{
	HRESULT hr = jSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&jBackBuf);

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc{};
	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_UNKNOWN;
	viewDesc.Texture2D.MipSlice = 0;

	if (hr != S_OK)
		return;
	hr = jDev->CreateRenderTargetView(jBackBuf, nullptr, &jRTarget);

	makeZBuf();
}

void jGraphics::makeZBuf()
{
	CD3D11_DEPTH_STENCIL_DESC depthDesc = CD3D11_DEPTH_STENCIL_DESC(TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
		FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);
	HRESULT hr = jDev->CreateDepthStencilState(&depthDesc, &jDepthState);
	jContext->OMSetDepthStencilState(jDepthState, 1);

	D3D11_TEXTURE2D_DESC descBuf;
	jBackBuf->GetDesc(&descBuf);

	D3D11_TEXTURE2D_DESC descDepth{};
	descDepth.Width = descBuf.Width; descDepth.Height = descBuf.Height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = jDev->CreateTexture2D(&descDepth, nullptr, &jDepthStencil);
	if (!SUCCEEDED(hr))
		throw std::runtime_error("could not make depth stencil");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	hr = jDev->CreateDepthStencilView(jDepthStencil, &descDSV, &jDSV);

	jContext->OMSetRenderTargets(1, &jRTarget, jDSV);
}

void jGraphics::init2D()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &jD2DFactory);

	hr = jSwap->GetBuffer(0, __uuidof(IDXGISurface), (void**)&jSurface);
	if (!SUCCEEDED(hr))
		return;

	float dpi = (float)GetDpiForWindow(hwnd);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpi, dpi);
	hr = jD2DFactory->CreateDxgiSurfaceRenderTarget(jSurface, &props, &jRT);

	jRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &jBrush);

	// DirectWrite
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&jWriteFactory));

	hr = jWriteFactory->CreateTextFormat(L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		36.0f,
		L"en-us",
		&jTextFormat);

	hr = jTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	hr = jTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
}

//--------------------------------------------------------
// Drawing
//--------------------------------------------------------

void jGraphics::draw3D(jeffModel* jModel)
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	jContext->ClearRenderTargetView(jRTarget, color); 
	jContext->ClearDepthStencilView(jDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);

	jModel->draw();
	jLayout = jModel->jLayout;
	jRast = jModel->jRast;
}

void jGraphics::draw2D()
{
	jRT->BeginDraw();

	D2D1_RECT_F layoutRect = D2D1::RectF(static_cast<FLOAT>(0), static_cast<FLOAT>(0), static_cast<FLOAT>(120), static_cast<FLOAT>(50));

	std::wstring frameHz = std::to_wstring(frameRate);
	jRT->DrawText(frameHz.c_str(), (UINT32)frameHz.size(), jTextFormat, layoutRect, jBrush);

	HRESULT hr = jRT->EndDraw();
}

void jGraphics::endFrame()
{
	jSwap->Present(1, 0);
}