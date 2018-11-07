#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <cstring>

std::vector<std::string> MY_ReadFile(std::string filename)
{
	std::fstream file;
	file.open(filename, std::ios::in);
	if (!file.is_open())
	{
		exit(1);
	}
	std::vector<std::string> data;

	std::string str;
	while (std::getline(file, str))
	{
		data.push_back(str);
	}

	return data;
}



std::vector<std::vector<char>> LoadMap(std::string filename)
{
	std::vector<std::vector<char>> map;

	std::fstream file;
	file.open(filename, std::ios::in);
	if (!file.is_open())
	{
		exit(1);
	}
	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		std::string str;
		std::getline(file, str);

		if (file.fail())
		{
		}

		map.push_back(std::vector<char>());
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			if (str.length() < x)
			{
				map[y].push_back('W');
			}
			else
			{
				map[y].push_back(str[x]);
				if ((str[x] == '\n') || (str[x] == '\0'))
				{
					break;
				}
			}
		}
	}
	
	return map;
}
