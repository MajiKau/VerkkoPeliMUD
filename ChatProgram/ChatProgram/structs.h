#pragma once
struct Player
{
	char name[20];
	int x, y;
	Player(char _name[20], int _x, int _y)
	{
		strncpy_s(name, _name, 20);
		x = _x;
		y = _y;
	}
};