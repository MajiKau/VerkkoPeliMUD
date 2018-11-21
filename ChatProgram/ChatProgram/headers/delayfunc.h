#pragma once
#include <vector>
#include <functional>

std::vector<std::pair<std::function<void()>, float>> delayed_functions;

void DelayedFunction(std::function<void()> function, float time)
{
	delayed_functions.push_back({ function,time });
}

void DelayedFunctionUpdate(float deltatime)
{
	for (int i = 0; i < delayed_functions.size(); i++)
	{
		auto& pair = delayed_functions[i];
		pair.second -= deltatime;
		if (pair.second <= 0.0f)
		{
			pair.first();
			delayed_functions.erase(delayed_functions.begin() + i);
			i--;
		}
	}
}

/* EXAMPLE
std::function<void()> func = [&var1, var2]() { HandleEvent(&var1, var2); };
DelayedFunction(func, 1.0f); //1 second delay

while(true) //Game loop
{
	DelayedFunctionUpdate(deltatime);
}
*/