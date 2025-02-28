#pragma once
#include <vector>
#include <unordered_map>
#include <memory>

#include "Item.h"

class Inventory
{
private:
	static size_t itemID;
	std::vector<size_t> itemsOrder;
	std::unordered_map<size_t, std::unique_ptr<Item>> items;
public:
	Inventory();
	Inventory(std::unique_ptr<Item>); // Start with an item already added

	void addItem(std::unique_ptr<Item>);
	std::unique_ptr<Item> removeItem(const size_t);

	Item* getItem(const size_t);
	std::vector<Item*> getAllItems();
};