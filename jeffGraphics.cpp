#include "jeffGraphics.h"
#include <stdio.h>

jGraphics::jGraphics(HWND handle)
{
	hwnd = handle;
	setScreenSize();
	initDevice();

	testMesh.loadFromObj("cube.obj");
	testMesh2.loadFromObj("cube.obj");

	testMat.jDev = jDev;
	testMat.createShaders(L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl");
}

jGraphics::~jGraphics()
{
	// D2D
	jWriteFactory->Release();
	jTextFormat->Release();
	jRT->Release();
	jBrush->Release();
	jD2DFactory->Release();

	// D3D
	jBackBuf->Release();
	jDepthStencil->Release();
	jDSV->Release();
	jVertBuf->Release();
	jIndexBuf->Release();
	jVConstBuf->Release();
	jPConstBuf->Release();
	testMat.releaseResources();
	jLayout->Release();
	jRast->Release();
	jDepthState->Release();
	jRTarget->Release();
	jContext->Release();
	jDev->Release();

	jSurface->Release();
	jSwap->Release();
}

void jGraphics::jDraw()
{
	draw3D();
	draw2D();

	jSwap->Present(1, 0);
}

void jGraphics::draw3D()
{
	setRasterizer();
	setVBuf(testMesh);
	setIBuf(testMesh);
	setConstantBuffer(1.0f);
	setShaders(testMat);
	setInputLayout();

	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	jContext->ClearRenderTargetView(jRTarget, color); 
	jContext->ClearDepthStencilView(jDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	jContext->DrawIndexed(36, 0, 0);
	setConstantBuffer(-0.8f);
	jContext->DrawIndexed(36, 0, 0);
}

void jGraphics::draw2D()
{
	jRT->BeginDraw();

	D2D1_RECT_F layoutRect = D2D1::RectF(static_cast<FLOAT>(screenSize.left), static_cast<FLOAT>(screenSize.top),
		static_cast<FLOAT>(80), static_cast<FLOAT>(50));

	std::wstring frameHz = std::to_wstring(frameRate);
	jRT->DrawText(frameHz.c_str(), (UINT32)frameHz.size(), jTextFormat, layoutRect, jBrush);

	HRESULT hr = jRT->EndDraw();
}

void jGraphics::initDevice()
{
	setScreenSize();

	makeSwapchain();
	makeRenderTarget();

	init2D();

	jContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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

void jGraphics::setScreenSize()
{
	GetClientRect(hwnd, &screenSize);
	width = screenSize.right - screenSize.left;
	height = screenSize.bottom - screenSize.top;
}

void jGraphics::makeRenderTarget()
{
	HRESULT hr = jSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&jBackBuf);

	D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
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

	D3D11_TEXTURE2D_DESC descDepth;
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

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	descDSV.Flags = 0;

	hr = jDev->CreateDepthStencilView(jDepthStencil, &descDSV, &jDSV);

	jContext->OMSetRenderTargets(1, &jRTarget, jDSV);
}

void jGraphics::makeSwapchain()
{
	DXGI_SWAP_CHAIN_DESC jSDesc;
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
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &jSDesc, &jSwap, &jDev, nullptr, &jContext);
}

void jGraphics::setInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC jLayoutDescs[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	HRESULT hr = jDev->CreateInputLayout(jLayoutDescs, sizeof(jLayoutDescs) / sizeof(D3D11_INPUT_ELEMENT_DESC), testMat.jVShaderBlob->GetBufferPointer(), testMat.jVShaderBlob->GetBufferSize(), &jLayout);
	jContext->IASetInputLayout(jLayout);
}

void jGraphics::setShaders(jeffMaterialShader mat)
{
	jContext->VSSetShader(mat.jVShader, 0, 0);
	jContext->PSSetShader(mat.jPShader, 0, 0);
}

void jGraphics::setRasterizer()
{
	D3D11_VIEWPORT jViewport;
	jViewport.TopLeftX = 0.0f;
	jViewport.TopLeftY = 0.0f;
	jViewport.Width = (float)width;
	jViewport.Height = (float)height;
	jViewport.MinDepth = 0.0f;
	jViewport.MaxDepth = 1.0f;
	jContext->RSSetViewports(1, &jViewport);

	D3D11_RECT jRect;
	jRect.left = 0;
	jRect.right = width;
	jRect.top = 0;
	jRect.bottom = height;
	jContext->RSSetScissorRects(1, &jRect);

	D3D11_RASTERIZER_DESC jRDesc = CD3D11_RASTERIZER_DESC(D3D11_FILL_SOLID, D3D11_CULL_BACK, true, 0, 0, 0, false, false, false, false);
	jDev->CreateRasterizerState(&jRDesc, &jRast);
	jContext->RSSetState(jRast);
}

void jGraphics::setVBuf(jeffMesh jMesh)
{
	D3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(jMesh.vertices.size() * sizeof(jeffMesh::jeffVertex)), D3D11_BIND_VERTEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = jMesh.vertices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	HRESULT hr = jDev->CreateBuffer(&bufferDesc, &InitData, &jVertBuf);

	UINT stride = sizeof(jeffMesh::jeffVertex);
	UINT offset = 0;
	jContext->IASetVertexBuffers(0, 1, &jVertBuf, &stride, &offset);
}

void jGraphics::setIBuf(jeffMesh jMesh)
{
	D3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(static_cast<UINT>(jMesh.indices.size() * sizeof(int)), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = jMesh.indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	jDev->CreateBuffer(&bufferDesc, &InitData, &jIndexBuf);

	jContext->IASetIndexBuffer(jIndexBuf, DXGI_FORMAT_R32_UINT, 0);
}

void jGraphics::setConstantBuffer(float cubenum)
{
	struct 
	{
		DirectX::XMMATRIX projMat;
		DirectX::XMMATRIX transformMat;
	} jVConstBufStruct;
	jVConstBufStruct.projMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH(1.0f, (float)(height * 1.0f / width), 0.1f, 100.0f));
	if (cubenum == 1)
		jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(time * cubenum) * DirectX::XMMatrixRotationX(time * 0.5f * cubenum) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f));
	else
		jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(time * cubenum) * DirectX::XMMatrixRotationX(time * 0.5f * cubenum) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f + offset));

	struct
	{
		DirectX::XMFLOAT4 dirLight;
		DirectX::XMFLOAT4 dirLightColour;
	} jPConstBufStruct;
	jPConstBufStruct.dirLight = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	jPConstBufStruct.dirLightColour = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	D3D11_BUFFER_DESC jVDesc;
	jVDesc.ByteWidth = sizeof(jVConstBufStruct);
	jVDesc.Usage = D3D11_USAGE_DYNAMIC;
	jVDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	jVDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	jVDesc.MiscFlags = 0;
	jVDesc.StructureByteStride = 0;

	D3D11_BUFFER_DESC jPDesc;
	jPDesc.ByteWidth = sizeof(jPConstBufStruct);
	jPDesc.Usage = D3D11_USAGE_DYNAMIC;
	jPDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	jPDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	jPDesc.MiscFlags = 0;
	jPDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA jVInitData;
	jVInitData.pSysMem = &jVConstBufStruct;
	jVInitData.SysMemPitch = 0;
	jVInitData.SysMemSlicePitch = 0;

	D3D11_SUBRESOURCE_DATA jPInitData;
	jPInitData.pSysMem = &jPConstBufStruct;
	jPInitData.SysMemPitch = 0;
	jPInitData.SysMemSlicePitch = 0;

	jDev->CreateBuffer(&jVDesc, &jVInitData, &jVConstBuf);
	jContext->VSSetConstantBuffers(0, 1, &jVConstBuf);

	jDev->CreateBuffer(&jPDesc, &jPInitData, &jPConstBuf);
	jContext->PSSetConstantBuffers(1, 1, &jPConstBuf);
}