#pragma once

#include "Button.h"

class DebugindicatorText : public InputWidget
{
private:
	sf::Vector2f getTickCorrection() const override;
	static const inline std::string DEBUG_TEXT = "DEBUG MODE";
	bool bIsDebug = false;

	Button T_Text;

public:
	DebugindicatorText();
	void construct() override;
	void tick(const float&) override;
};