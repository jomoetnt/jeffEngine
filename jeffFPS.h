#pragma once

#include "jeffText.h"

namespace jeffNamespace
{
	class jeffFPS : public jeffText
	{
	public:
		jeffFPS(const char* name, ID2D1RenderTarget* jRT) : jeffText::jeffText(name, jRT)
		{

		}

		void draw(float delta) override
		{
			std::wstring fps = std::to_wstring((int)(1.0f / delta));
			text = &fps;
			jeffText::draw(delta);
		}
	};
}