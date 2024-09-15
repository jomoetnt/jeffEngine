#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <d3d11.h>
#include <DDSTextureLoader.h>
#include "jeffDeviceState.h"

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

        jeffMaterial()
        {
            jDev = jeffDeviceState::getInstance()->jDev; jContext = jeffDeviceState::getInstance()->jContext;
        }

        void initTexture(LPCWSTR filename, JEFF_TEXTURE_TYPE type, D3D11_TEXTURE_ADDRESS_MODE extendMode = D3D11_TEXTURE_ADDRESS_WRAP)
        {
            ID3D11Texture2D* tex = nullptr;
            ID3D11ShaderResourceView* jView = nullptr;
            HRESULT hr = DirectX::CreateDDSTextureFromFile(jDev, jContext, filename, (ID3D11Resource**)&tex, &jView);
            if (FAILED(hr)) throw std::runtime_error("error creating texture");

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
            hr = jDev->CreateSamplerState(&jSamplerDesc, &sam);
            if (FAILED(hr)) throw std::runtime_error("error creating sampler state");

            jTypes.emplace_back(type);
            jTextures.emplace_back(tex);
            jViews.emplace_back(jView);
            jSams.emplace_back(sam);
        }

		void loadFromMtl(const char* filename)
		{
            std::wifstream file(filename);

            std::wstring line;

            if (file.is_open())
            {
                int i = 0;

                while (std::getline(file, line))
                {
                    if (line[0] == L'#') continue;

                    if (line[0] == L'N')
                    {
                        continue;
                    }

                    if (line[0] == L'K')
                    {
                        continue;
                    }

                    if (line[0] == L'd')
                    {
                        continue;
                    }

                    if (line[0] == L'i')
                    {
                        continue;
                    }

                    if (line[0] == L'n')
                    {
                        continue;
                    }

                    // diffuse texture
                    if (line[0] == L'm')
                    {
                        initTexture(line.substr(7).c_str(), DIFFUSE);
                    }
                }

                file.close();
            }
            else
            {
                throw std::runtime_error("could not open mtl");
            }
		}
	};
}