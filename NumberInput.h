#pragma once

#include <4dm.h>

using namespace fdm;
using namespace fdm::gui;

class NumberInput : public TextInput
{
public:
	bool floatingPointInput = false;

	int maxValue = INT_MAX;
	int minValue = INT_MIN;

	NumberInput(bool floatingPointInput = false);

	bool charInput(const gui::Window* w, uint32_t codepoint) override;
	bool keyInput(const gui::Window* w, int key, int scancode, int action, int mods) override;

	int getInt();
	float getFloat();
};
