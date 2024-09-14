#pragma once
#include <d3d11.h>

namespace jeffNamespace
{
	class jeffDeviceState
	{
	public:
		ID3D11Device* jDev = nullptr;
		ID3D11DeviceContext* jContext = nullptr;

		static void makeInstance()
		{
			instance = new jeffDeviceState();
		}
		static jeffDeviceState* getInstance()
		{
			return instance;
		}
		static void destroyInstance()
		{
			delete instance;
		}
	private:
		jeffDeviceState() = default;
		~jeffDeviceState()
		{
			jContext->Release();
			jDev->Release();
		}
		static inline jeffDeviceState* instance;
	};
}