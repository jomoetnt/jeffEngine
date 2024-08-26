#include "jeffGraphics.h"
#include <stdio.h>

jGraphics::jGraphics(HWND handle)
{
	hwnd = handle;
	initDevice();

	setScreenSize();

	testMesh.loadFromObj("cube.obj");

	testMat.jDev = jDev;
	testMat.createShaders(L"jeffVertexShader.hlsl", L"jeffPixelShader.hlsl");
}

jGraphics::~jGraphics()
{
	jVertBuf->Release();
	testMat.releaseResources();
	jLayout->Release();
	jRast->Release();
	jDepthState->Release();
	jBlendState->Release();
	jRTarget->Release();
	jContext->Release();
	jSwap->Release();
	jDev->Release();
}

void jGraphics::jDraw()
{
	time += delta;

	setRasterizer();
	setVBuf(testMesh);
	setIBuf(testMesh);
	setConstantBuffer();
	setShaders(testMat);
	setInputLayout();

	const float color[] = { 0.0f, 0.0f, 0.0f, 1.0f};
	jContext->ClearRenderTargetView(jRTarget, color);
	jContext->DrawIndexed(36, 0, 0);
	jSwap->Present(1, 0);
}

void jGraphics::initDevice()
{
	setScreenSize();
	makeSwapchain();
	makeRenderTarget();

	jContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void jGraphics::setScreenSize()
{
	RECT screenSize;
	GetClientRect(hwnd, &screenSize);
	width = screenSize.right - screenSize.left;
	height = screenSize.bottom - screenSize.top;
}

void jGraphics::makeRenderTarget()
{
	ID3D11Texture2D* backBuf = nullptr;
	HRESULT hr = jSwap->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuf);

	if (hr != S_OK)
		return;
	hr = jDev->CreateRenderTargetView(backBuf, nullptr, &jRTarget);
	LPWSTR errorText;
	if (FAILED(hr))
	{
		FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&errorText, 0, nullptr);
	}

	auto blendDesc = CD3D11_BLEND_DESC(CD3D11_DEFAULT());
	jDev->CreateBlendState(&blendDesc, &jBlendState);
	jContext->OMSetBlendState(jBlendState, NULL, 0xffffffff);

	auto depthDesc = CD3D11_DEPTH_STENCIL_DESC(
		FALSE, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_LESS,
		FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS);
	jDev->CreateDepthStencilState(&depthDesc, &jDepthState);
	jContext->OMSetDepthStencilState(jDepthState, 1);
	jContext->OMSetRenderTargets(1, &jRTarget, nullptr);
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
	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &jSDesc, &jSwap, &jDev, nullptr, &jContext);
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
	D3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(jMesh.vertices.size() * sizeof(jeffMesh::jeffVertex), D3D11_BIND_VERTEX_BUFFER);

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
	D3D11_BUFFER_DESC bufferDesc = CD3D11_BUFFER_DESC(jMesh.indices.size() * sizeof(int), D3D11_BIND_INDEX_BUFFER);

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = jMesh.indices.data();
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	jDev->CreateBuffer(&bufferDesc, &InitData, &jIndexBuf);

	jContext->IASetIndexBuffer(jIndexBuf, DXGI_FORMAT_R32_UINT, 0);
}

void jGraphics::setConstantBuffer()
{
	struct 
	{
		DirectX::XMMATRIX projMat;
		DirectX::XMMATRIX transformMat;
	} jVConstBufStruct;
	jVConstBufStruct.projMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveLH((float)(width * 1.0f / height), 1.0f, 0.1f, 100.0f));
	jVConstBufStruct.transformMat = DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationZ(time) * DirectX::XMMatrixRotationX(time * 0.5) * DirectX::XMMatrixTranslation(0.0f, 0.0f, 3.0f));

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