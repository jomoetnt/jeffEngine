#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include "jeffDeviceState.h"
#include <DirectXMath.h>

namespace jeffNamespace
{
	class jeffMaterial
	{
	public:
        enum JEFF_TEXTURE_TYPE
        {
            DIFFUSE, ROUGHNESS, SPECULAR, NORMAL, AO,
        };

        ID3D11Device* jDev = nullptr;
        ID3D11DeviceContext* jContext = nullptr;

        std::vector<JEFF_TEXTURE_TYPE> jTypes;
        std::vector <ID3D11Texture2D*> jTextures;
        std::vector <ID3D11ShaderResourceView*> jViews;
        std::vector <ID3D11SamplerState*> jSams;

        // k_s, k_d, k_a, alpha (shininess)
        DirectX::XMFLOAT4 properties = DirectX::XMFLOAT4(15.0f, 1.0f, 1.0f, 3.0f);

        std::wstring name;

        jeffMaterial()
        {
            jDev = jeffDeviceState::getInstance()->jDev; jContext = jeffDeviceState::getInstance()->jContext;
        }

        void initTexture(std::wstring filename, JEFF_TEXTURE_TYPE type, D3D11_TEXTURE_ADDRESS_MODE extendMode = D3D11_TEXTURE_ADDRESS_WRAP)
        {
            ID3D11Texture2D* tex = nullptr;
            ID3D11ShaderResourceView* jView = nullptr;
            // Create sampler state
            ID3D11SamplerState* sam = nullptr;
            D3D11_SAMPLER_DESC jSamplerDesc{};
            jSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
            jSamplerDesc.AddressU = extendMode;
            jSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
            jSamplerDesc.AddressW = extendMode;
            jSamplerDesc.MipLODBias = 0;
            jSamplerDesc.MaxAnisotropy = 1;
            jSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
            jSamplerDesc.MinLOD = 0;
            jSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
            HRESULT hr = jDev->CreateSamplerState(&jSamplerDesc, &sam);
            if (FAILED(hr)) throw std::runtime_error("error creating sampler state");

            if (filename.find(L".dds") == -1)
            {
                hr = DirectX::CreateWICTextureFromFile(jDev, jContext, filename.c_str(), (ID3D11Resource**)&tex, &jView);
            }
            else
            {
                hr = DirectX::CreateDDSTextureFromFile(jDev, jContext, filename.c_str(), (ID3D11Resource**)&tex, &jView);
            }
            if (FAILED(hr)) throw std::runtime_error("error creating texture");

            jTypes.emplace_back(type);
            jTextures.emplace_back(tex);
            jViews.emplace_back(jView);
            jSams.emplace_back(sam);
        }

	};
}