#pragma once

#include <4dm.h>

using namespace fdm;
using namespace fdm::gui;

#include "ImgButton.h"

class WaypointElement : public Button
{
public:
	Text nameText;
	Text coordsText;
	ImgButton removeBtn;
	ImgButton editBtn;

	WaypointElement();

	void render(Window* w) override;
	bool mouseButtonInput(const Window* w, int button, int action, int mods) override;
};
