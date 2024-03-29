#include "NumberInput.h"

NumberInput::NumberInput(bool floatingPointInput) : floatingPointInput(floatingPointInput)
{
	width = 150;
	height = 40;
}

bool NumberInput::charInput(const gui::Window* w, uint32_t codepoint)
{
	std::string oldText = text;
	int oldCursorPos = cursorPos;
	int point = text.find('.');
	bool result = false;

	if (codepoint == ',')
		codepoint = '.';

	if (isdigit(codepoint) || (codepoint == '.' && floatingPointInput) || (cursorPos == 0 && codepoint == '-'))
		result = TextInput::charInput(w, codepoint);
	else return result;

	if (text == "-")
		return result;


	// moved the floating point
	if (point != std::string::npos && codepoint == '.' && floatingPointInput)
	{
		text.erase(point, 1);
		cursorPos--;
	}
	// check if its actually a proper motehrfuckigjmdf integer (or float, based on the mode) now
	char* p;
	if (!floatingPointInput)
	{
		int64_t int64t = _strtoi64(text.c_str(), &p, 10);
		if (*p) { text = oldText; cursorPos = oldCursorPos;  return result; }

		if (int64t >= maxValue)
			text = std::to_string(maxValue);
		else if (int64t <= minValue)
			text = std::to_string(minValue);
		else
			text = std::to_string(int64t);
	}
	else
	{
		double doubl = strtod(text.c_str(), &p);
		if (*p) { text = oldText; cursorPos = oldCursorPos;  return result; }

		if (doubl >= maxValue)
			text = std::to_string(maxValue);
		else if (doubl <= minValue)
			text = std::to_string(minValue);
	}
	return result;
}

bool NumberInput::keyInput(const gui::Window* w, int key, int scancode, int action, int mods)
{
	std::string oldText = text;
	int oldCursorPos = cursorPos;

	bool a = TextInput::keyInput(w, key, scancode, action, mods);

	// check if its actually a proper motehrfuckigjmdf integer (or float, based on the mode) now

	if (text == "-")
		return a;

	char* p;
	if (!floatingPointInput)
		_strtoi64(text.c_str(), &p, 10);
	else
		strtod(text.c_str(), &p);
	if (*p) { text = oldText; cursorPos = oldCursorPos; }
}

int NumberInput::getInt() 
{
	return strtol(text.c_str(), nullptr, 10);
}
float NumberInput::getFloat() 
{
	return strtof(text.c_str(), nullptr);
}