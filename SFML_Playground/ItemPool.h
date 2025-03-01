#pragma once
#include <memory>
#include <queue>

#include "Item.h"

class ItemPool
{
private:
    std::queue<std::unique_ptr<Item>> pool;
public:
    template <typename T, typename = std::enable_if_t<std::is_base_of_v<Item, T>>> // Restrict use of 'T' to subclasses of 'Item'
    //template <typename T>
    std::unique_ptr<T> get()
    {
        static_assert(std::is_base_of_v<Item, T>, "T must be a subclass of Item!");

        for (auto it = pool.begin(); it != pool.end(); ++it)
        {
            T* castedItem = dynamic_cast<T*>(it->get());
            if (castedItem)  // Found a matching subclass
            {
                std::unique_ptr<T> item = std::unique_ptr<T>(it->release());
                pool.erase(it); // Remove it from the pool
                return item;
            }
        }

        // No existing item found, create a new one
        return std::make_unique<T>();
    }

    void release(std::unique_ptr<Item> itemPtr)
    {
        pool.push(std::move(itemPtr));
    }
};