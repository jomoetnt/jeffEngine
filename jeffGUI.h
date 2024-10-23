#pragma once

#include "jeff2D.h"

namespace jeffNamespace
{
	class jeffGUI : public jeff2D
	{
	public:
		std::vector<jeff2D*> guiElements;

		jeffGUI(const char* guiName, ID2D1RenderTarget* jRT) : jeff2D::jeff2D(guiName, jRT)
		{
			
		}

		~jeffGUI()
		{
			for (jeff2D* element : guiElements)
			{
				delete element;
			}
		}

		void draw(float delta) override
		{
			for (auto& element : guiElements)
			{
				element->draw(delta);
			}
		}


	};
}