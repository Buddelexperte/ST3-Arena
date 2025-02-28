#include "Inventory.h"

Inventory::Inventory(std::unique_ptr<Item> initItem)
	: Inventory()
{ 
	addItem(std::move(initItem));
}

Inventory::Inventory()
{
	Inventory::itemID = 0;
}

void Inventory::addItem(std::unique_ptr<Item> newItem)
{
	itemID++;
	items[itemID] = std::move(newItem);
	itemsOrder.push_back(itemID);
}

std::unique_ptr<Item> Inventory::removeItem(const size_t id)
{
    auto it = items.find(id);
    if (it == items.end() || !it->second)
        return nullptr; // Item not found or already removed

    std::unique_ptr<Item> removedItem = std::move(it->second);
    it->second = nullptr;
    return removedItem;
}

Item* Inventory::getItem(const size_t id)
{
	return items[id].get();
}

std::vector<Item*> Inventory::getAllItems()
{
    std::vector<Item*> res;
    res.reserve(itemsOrder.size());  // Optimize memory allocation

    for (size_t id : itemsOrder)
    {
        Item* item = getItem(id);
        if (item)
            res.push_back(item);
    }

    return res;
}