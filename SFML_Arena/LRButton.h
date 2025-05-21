#pragma once

#include "Button.h"

class LRButton : public Button
{
private:
	static const inline std::string l_lable = "<";
	static const inline std::string r_lable = ">";

	Button left;
	Button right;

	void constructLR(const RawButton& CONSTR, const bool startDisabled);
	void delegateEvents();
public:
	LRButton(InputWidget* parent)
		: Button(parent), left(parent), right(parent)
	{
		shapes.push_back(&left);
		shapes.push_back(&right);
	}

	void construct() override;
	void construct(const RawButton& CONSTR) override;
	void construct(const RawButton& CONSTR, const bool startDisabled) override
	{
		constructLR(CONSTR, startDisabled);
		Button::construct(CONSTR, startDisabled);	
	}

	void tick(const float& deltaTime) override;

	bool isMouseOver(const bool& bCheckForClick = false) override;

	std::function<void()> onLeftClick = nullptr;
	std::function<void()> onRightClick = nullptr;
};