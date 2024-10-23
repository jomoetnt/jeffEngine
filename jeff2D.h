#pragma once

#include "jeffObject.h"
#include <d2d1.h>

namespace jeffNamespace
{
	class jeff2D : public jeffObject
	{
	public:
		ID2D1RenderTarget* jRT;

		jeff2D(const char* name, ID2D1RenderTarget* pjRT) : jeffObject::jeffObject(name)
		{
			jRT = pjRT;
			dimension3 = false;
		}

		bool visible = true;

		virtual void draw(float delta) {}


	};
}