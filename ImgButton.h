#pragma once

#include <4dm.h>

using namespace fdm;
using namespace fdm::gui;

class ImgButton : public Button
{
public:
	Image img;

	int edgeW = 1;
	int edgeH = 1;

	ImgButton() {}
	ImgButton(const std::string& imagePath, int width = 0, int height = 0, int edgeW = 1, int edgeH = 1);

	void render(Window* w) override;
};

