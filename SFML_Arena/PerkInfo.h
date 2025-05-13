#pragma once

enum class PerkFamily
{
	None = 0,
	Offensive,
	Defensive,
	Utility,
	Support,
	Count
};

struct PerkFamilyInfo
{
	std::string name;
	std::string description;
};

inline PerkFamilyInfo getPerkFInfo(const PerkFamily& family)
{
	static const std::unordered_map<PerkFamily, PerkFamilyInfo> perkFamilyInfo = {
		{ PerkFamily::None,			{ "None",		"No perks selected." } },
		{ PerkFamily::Offensive,	{ "Offensive",	"Increases damage output." } },
		{ PerkFamily::Defensive,	{ "Defensive",	"Reduces damage taken." } },
		{ PerkFamily::Utility,		{ "Utility",	"Provides various utility effects." } },
		{ PerkFamily::Support,		{ "Support",	"Enhances team performance." } },
		{ PerkFamily::Count,		{ "FamilyName",	"Invalid perk family." } }
	};

	// Checking just for safety, though the enum should be valid in most cases
	size_t index = static_cast<size_t>(family);
	if (index > static_cast<size_t>(PerkFamily::Count))
		index = static_cast<size_t>(PerkFamily::Count);

	return perkFamilyInfo.at(family);
}