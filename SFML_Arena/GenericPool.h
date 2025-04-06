#pragma once
#include <vector>
#include <memory>

template <typename T>
class GenericPool
{
private:
	std::vector<std::unique_ptr<T>> pool;

	GenericPool() {}
	// Delete copy and assignment constructor
	GenericPool(const GenericPool&) = delete;
	GenericPool& operator=(const GenericPool&) = delete;

public:
	// Acces templated SINGLETON instance
	static GenericPool<T>& instance()
	{
		static GenericPool<T> instance;
		return instance;
	}

	std::unique_ptr<T> get()
	{
		if (!pool.empty())
		{
			std::unique_ptr<T> obj = std::move(pool.back());
			pool.pop_back();

			return obj;
		}

		return std::make_unique<T>();
	}

	void release(std::unique_ptr<T> obj)
	{
		pool.emplace_back(std::move(obj));
	}
};