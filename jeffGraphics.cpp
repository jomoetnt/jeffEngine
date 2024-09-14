#include "jeffGraphics.h"
#include <stdio.h>
#include <d3dcompiler.h>

using namespace jeffNamespace;

//--------------------------------------------------------
// Initialization
//--------------------------------------------------------

void jGraphics::makeInstance(HWND handle)
{
	instance = new jGraphics();

	instance->hwnd = handle;
	jeffDeviceState::makeInstance();

	RECT screenSize{};
	GetClientRect(instance->hwnd, &screenSize);
	instance->screenWidth = screenSize.right - screenSize.left;
	instance->screenHeight = screenSize.bottom - screenSize.top;

	instance->initDevice();
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
	jDepthState->Release();
	jRTarget->Release();
	jeffDeviceState::getInstance()->destroyInstance();
	jVShaderBlob->Release(); jPShaderBlob->Release();
	jVShader->Release(); jPShader->Release();

	jSurface->Release();
	jSwap->Release();
}

void jGraphics::initDevice()
{
	makeSwapchain();
	makeRenderTarget();
	createInputLayout();
	createRasterizer();

	init2D();
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
	if (FAILED(hr)) throw std::runtime_error("error making swapchain");
	jeffDeviceState::getInstance()->jDev = jDev; jeffDeviceState::getInstance()->jContext = jContext;
}

void jGraphics::makeRenderTarget()
{
	HRESULT hr = jSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&jBackBuf);
	if (FAILED(hr)) throw std::runtime_error("error getting backbuffer");

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc{};
	viewDesc.Format = DXGI_FORMAT_UNKNOWN;
	viewDesc.ViewDimension = D3D11_RTV_DIMENSION_UNKNOWN;
	viewDesc.Texture2D.MipSlice = 0;

	if (hr != S_OK)
		return;
	hr = jDev->CreateRenderTargetView(jBackBuf, nullptr, &jRTarget);
	if (FAILED(hr)) throw std::runtime_error("error creating render target view");

	makeZBuf();
}

void jGraphics::makeZBuf()
{
	CD3D11_DEPTH_STENCIL_DESC depthDesc = CD3D11_DEPTH_STENCIL_DESC(TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
		FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);
	HRESULT hr = jDev->CreateDepthStencilState(&depthDesc, &jDepthState);
	if (FAILED(hr)) throw std::runtime_error("error creating depth stencil state");
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
	if (!SUCCEEDED(hr)) throw std::runtime_error("could not make depth stencil");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV{};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	hr = jDev->CreateDepthStencilView(jDepthStencil, &descDSV, &jDSV);
	if (FAILED(hr)) throw std::runtime_error("error creating depth stencil view");

	jContext->OMSetRenderTargets(1, &jRTarget, jDSV);
}

void jGraphics::createInputLayout()
{
	ID3DBlob* vErr; ID3DBlob* pErr;
	D3DCreateBlob(256, &vErr); D3DCreateBlob(256, &pErr);

	int jFlags1 = D3DCOMPILE_PARTIAL_PRECISION | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
#ifdef NDEBUG
	jFlags1 |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#else
	jFlags1 |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hr = D3DCompileFromFile(L"jeffVertexShader.hlsl", NULL, NULL, "main", "vs_5_0", jFlags1, 0, &jVShaderBlob, &vErr);
	if (FAILED(hr)) throw std::runtime_error("error compiling vertex shader");
	hr = D3DCompileFromFile(L"jeffPixelShader.hlsl", NULL, NULL, "main", "ps_5_0", jFlags1, 0, &jPShaderBlob, &pErr);
	if (FAILED(hr)) throw std::runtime_error("error compiling pixel shader");

	hr = jDev->CreateVertexShader(jVShaderBlob->GetBufferPointer(), jVShaderBlob->GetBufferSize(), nullptr, &jVShader);
	if (FAILED(hr)) throw std::runtime_error("error creating vertex shader");
	hr = jDev->CreatePixelShader(jPShaderBlob->GetBufferPointer(), jPShaderBlob->GetBufferSize(), nullptr, &jPShader);
	if (FAILED(hr)) throw std::runtime_error("error creating pixel shader");

	jContext->VSSetShader(jVShader, 0, 0);
	jContext->PSSetShader(jPShader, 0, 0);

	hr = jDev->CreateInputLayout(jLayoutDescs, sizeof(jLayoutDescs) / sizeof(D3D11_INPUT_ELEMENT_DESC), jVShaderBlob->GetBufferPointer(), jVShaderBlob->GetBufferSize(), &jLayout);
	if (FAILED(hr)) throw std::runtime_error("error creating input layout");
	jContext->IASetInputLayout(jLayout);
}

void jGraphics::createRasterizer()
{
	D3D11_VIEWPORT jViewport{};
	jViewport.TopLeftX = 0.0f;
	jViewport.TopLeftY = 0.0f;
	jViewport.Width = (float)screenWidth;
	jViewport.Height = (float)screenHeight;
	jViewport.MinDepth = 0.0f;
	jViewport.MaxDepth = 1.0f;

	D3D11_RECT jRect{};
	jRect.left = 0;
	jRect.right = screenWidth;
	jRect.top = 0;
	jRect.bottom = screenHeight;

	jContext->RSSetViewports(1, &jViewport);
	jContext->RSSetScissorRects(1, &jRect);
}

void jGraphics::init2D()
{
	HRESULT hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &jD2DFactory);
	if (FAILED(hr)) throw std::runtime_error("error creating direct2d factory");

	hr = jSwap->GetBuffer(0, __uuidof(IDXGISurface), (void**)&jSurface);
	if (FAILED(hr)) throw std::runtime_error("error getting backbuffer surface");

	float dpi = (float)GetDpiForWindow(hwnd);

	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED),
		dpi, dpi);
	hr = jD2DFactory->CreateDxgiSurfaceRenderTarget(jSurface, &props, &jRT);
	if (FAILED(hr)) throw std::runtime_error("error creating surface render target");

	jRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &jBrush);

	// DirectWrite
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&jWriteFactory));
	if (FAILED(hr)) throw std::runtime_error("error creating directwrite factory");

	// Make more text formats available
	hr = jWriteFactory->CreateTextFormat(L"Arial",
		NULL,
		DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		18.0f,
		L"en-us",
		&jTextFormat);
	if (FAILED(hr)) throw std::runtime_error("error creating text format");

	hr = jTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	if (FAILED(hr)) throw std::runtime_error("error setting text alignment");
	hr = jTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	if (FAILED(hr)) throw std::runtime_error("error setting paragraph alignment");
}

//--------------------------------------------------------
// Drawing
//--------------------------------------------------------

void jGraphics::draw2D()
{
	jRT->BeginDraw();

	// Temporary test
	D2D1_RECT_F layoutRect = D2D1::RectF(static_cast<FLOAT>(0), static_cast<FLOAT>(0), static_cast<FLOAT>(140), static_cast<FLOAT>(50));
	std::wstring frameHz = L"test";
	jRT->DrawText(frameHz.c_str(), (UINT32)frameHz.size(), jTextFormat, layoutRect, jBrush);

	HRESULT hr = jRT->EndDraw();
	if (FAILED(hr)) throw std::runtime_error("error ending 2d draw");
}

void jGraphics::beginFrame()
{
	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	jContext->ClearRenderTargetView(jRTarget, color);
	jContext->ClearDepthStencilView(jDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void jGraphics::endFrame()
{
	jSwap->Present(1, 0);
}