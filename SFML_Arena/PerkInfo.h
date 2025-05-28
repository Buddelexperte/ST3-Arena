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

// Structure to hold perk node information
struct PerkNodeInfo
{
	unsigned int id = 0;
	std::string tag;
	std::string name;
	std::string description;
	std::vector<PerkNodeInfo*> children;  // Changed to pointer
	bool bUnlocked = false;
	bool bSelected = false;
	bool isPathHovered = false;
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
		{ PerkFamily::Offensive,	{ "OFFENSIVE",	"Increases damage output." } },
		{ PerkFamily::Defensive,	{ "DEFENSIVE",	"Reduces damage taken." } },
		{ PerkFamily::Utility,		{ "UTILITY",	"Provides various utility effects." } },
		{ PerkFamily::Support,		{ "SUPPORT",	"Enhances team performance." } },
		{ PerkFamily::Count,		{ "FamilyName",	"Invalid perk family." } }
	};

	// Checking just for safety, though the enum should be valid in most cases
	size_t index = static_cast<size_t>(family);
	if (index > static_cast<size_t>(PerkFamily::Count))
		index = static_cast<size_t>(PerkFamily::Count);

	return perkFamilyInfo.at(family);
}