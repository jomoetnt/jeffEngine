#pragma once

#include "jeff2D.h"
#include <dwrite.h>

namespace jeffNamespace
{
	class jeffText : public jeff2D
	{
	public:
		ID2D1SolidColorBrush* jBrush;
		IDWriteFactory* jWriteFactory;
		IDWriteTextFormat* jTextFormat;

		std::string* text;

		jeffText(const char* textName, ID2D1RenderTarget* pjRT) : jeff2D::jeff2D(textName, pjRT)
		{
			pjRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Green), &jBrush);

			// DirectWrite
			HRESULT hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&jWriteFactory));
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

		jeffText(const char* textName, ID2D1RenderTarget* pjRT, std::string* ptext) : jeff2D::jeff2D(textName, jRT)
		{
			text = ptext;
		}

		void draw(float delta) override
		{
			D2D1_RECT_F layoutRect = D2D1::RectF(transformPosition.x, transformPosition.y, transformPosition.x + transformScale.x, transformPosition.y + transformScale.y);
			std::wstring frameHz = std::to_wstring((int)(1.0f / delta));
			jRT->DrawText(frameHz.c_str(), (UINT32)frameHz.size(), jTextFormat, layoutRect, jBrush);
		}

	};
}