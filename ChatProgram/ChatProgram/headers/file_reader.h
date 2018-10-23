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
	for (int y = 0; y < 50; y++)
	{
		//std::u32string
		//char c = file.get();
		std::string str;
		std::getline(file, str);
		
		//unsigned char str[128];
		//file.getline((char*)str, 128);
		if (file.fail())
		{
			//break;
		}

		map.push_back(std::vector<char>());
		for (int x = 0; x < 128; x++)
		{
			/*switch (str[x])
			{
			case 'W':
				//map[y].push_back(219);
				map[y].push_back('#');
				break;
			case 'S':
				//map[y].push_back(176);
				map[y].push_back('.');
				break;
			case 'G':
				//map[y].push_back(177);
				map[y].push_back(';');
				break;
			case 'B':
				//map[y].push_back(178);
				map[y].push_back('M');
				break;
			default:
				map[y].push_back(str[x]);
				break;
			}*/
			map[y].push_back(str[x]);
			if ((str[x] == '\n') || (str[x] == '\0'))
			{
				break;
			}
		}
	}
	
	return map;
}
