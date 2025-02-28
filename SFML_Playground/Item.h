#pragma once
#include <string>

class Item
{
public:
	struct ItemInfo
	{
		std::string name = "ItemName";
		std::string desc = "Item Description";
	};
private:
	ItemInfo itemInfo;
public:
	ItemInfo getItemInfo() const
		{ return itemInfo; }
};

class Perk : public Item
{

};

class Weapon : public Item
{

};