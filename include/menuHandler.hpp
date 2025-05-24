#pragma once

#include "specs.hpp"

class MenuHandler
{
private:

	std::stack<std::shared_ptr<Menu>> MenuStack;

public:

	void PushMenu(std::shared_ptr<Menu> menu)
	{ 
		MenuStack.push(menu); 
	}
	void PopMenu() 
	{ 
		if (!MenuStack.empty()) 
			MenuStack.pop(); 
	}

	void Update()
	{
		if (!MenuStack.empty())
			MenuStack.top()->update();
	}

	void Draw()
	{
		if (!MenuStack.empty())
			MenuStack.top()->draw();
	}

	std::shared_ptr<Menu> TopMenu()
	{
		if (!MenuStack.empty())
			return MenuStack.top();
	}
};