#pragma once

#include "WidgetElements.h"
#include "PerkFamilyDisplayElement.h" // Component
#include <functional>

class PerkFamilyMenu : public InputWidget
{
private:
	void delegateEvents() override;

	Button B_Skip;

	PerkFamily_Element pf_display_1;
	PerkFamily_Element pf_display_2;
	PerkFamily_Element pf_display_3;
	PerkFamily_Element pf_display_4;

	const std::vector<PerkFamily_Element*> familyDisplays = {
		&pf_display_1, &pf_display_2, &pf_display_3, &pf_display_4
	};

public:
	PerkFamilyMenu(InputWidget* parent);
	void tick(const float& deltaTime) override;
	void construct() override;

	bool isMouseOver(const bool&) override;

	std::function<void(PerkFamily)> onSelectFamily = nullptr;
};

