#include "ImgButton.h"

ImgButton::ImgButton(const std::string& imagePath, int width, int height, int edgeW, int edgeH) : edgeW(edgeW), edgeH(edgeH)
{
	img = Image(imagePath, true);

	if (width != 0)
		img.width = width;

	if (height != 0)
		img.height = height;

	this->width = img.width + edgeW * 2;
	this->height = img.height + edgeH * 2;
}

void ImgButton::render(Window* w)
{
	Button::render(w);
	
	img.width = width - edgeW * 2;
	img.height = height - edgeH * 2;

	img.xOffset = xOffset + edgeW;
	img.yOffset = yOffset + edgeH;
	img.render(w);
}
