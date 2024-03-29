#pragma once

#include <4dm.h>

using namespace fdm;
using namespace fdm::gui;

class NumberInput : public TextInput
{
public:
	bool floatingPointInput = false;

	Text titleText;
	std::string title = "";

	int maxValue = INT_MAX;
	int minValue = INT_MIN;

	NumberInput() {}
	NumberInput(const std::string& title, bool floatingPointInput = false);

	void render(gui::Window* w) override;
	bool charInput(const gui::Window* w, uint32_t codepoint) override;
	bool keyInput(const gui::Window* w, int key, int scancode, int action, int mods) override;

	int getInt();
	float getFloat();
};
