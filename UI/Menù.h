#pragma once

#include "UI.h"
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>

class Menu
{
public:

	Menu() = default;

	template <typename T, typename... Args>
	void add(Args&&... args) 
	{ 
		elements.emplace_back(std::make_unique<T>(std::forward<Args>(args)...)); 
	}
	bool remove(const std::string& id)
	{
		for (auto it = elements.begin(); it != elements.end(); ++it)
		{
			if ((*it)->getId() == id)
			{
				elements.erase(it);
				hiddenIDs.erase(id); // Rimuovi anche dalla hiddenIDs
				return true;
			}
		}
		return false;
	}
	void draw() 
	{
		for (auto& element : elements)
		{
			if (hiddenIDs.find(element->getId()) == hiddenIDs.end() && element->getActive())
			{
				element->draw();
			}
		}
	}
	void update() 
	{ 
		for (auto& element : elements)
		{
			if (element->getActive())
			{
				element->update();
			}
		}
	}

	const std::vector<std::unique_ptr<Widget>>& getVectorElements() const 
	{
		return elements;
	}
	Widget* getByID(const std::string& id)
	{
		for (size_t i = 0; i < elements.size(); i++)
		{
			if (elements[i]->getId() == id)
				return elements[i].get();
		}

		std::cerr << "Unhandled exception when accessing a deleted widget!\n";
		return nullptr;
	}

	void deactive(const std::string& id)
	{
		getByID(id)->setActive(false);
		hiddenIDs.insert(id);
	}

	void activate(const std::string& id)
	{
		hiddenIDs.erase(id);
		getByID(id)->setActive(true);
	}

private:

	std::unordered_set<std::string> hiddenIDs; // O(1) lookup
	std::vector<std::unique_ptr<Widget>> elements;
};