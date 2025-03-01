#pragma once
#include <string>

class Item
{
public:
	struct ItemInfo
	{
		std::string name;
		std::string desc;

		ItemInfo(std::string n = "ItemName", std::string d = "Item Description")
			: name(std::move(n)), desc(std::move(d))
		{}
	};

	enum class ItemUse
	{
		INVALID_USE		= -1,
		NO_USE			= 0,
		ATTACK,
		LOAD_UP,
		CANCEL_LOAD,
		PASSIVE_TRIGGER
	};

	enum class UseResult
	{
		INVALID_USE = -1,
		FAILURE = 0,
		FAILURE_NO_RESOURCES,
		SUCCESS
	};

protected:
	bool bReady = false;
	ItemInfo itemInfo;
public:
	virtual ~Item() = default;

	bool getIsReady() const 
		{ return bReady; }

	ItemInfo getItemInfo() const
		{ return itemInfo; }

	virtual UseResult use(const ItemUse) = 0;
};

class Perk : public Item
{

};

class Weapon : public Item
{

};