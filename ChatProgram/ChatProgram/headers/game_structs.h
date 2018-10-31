#pragma once

#define MAP_SIZE_X 90
#define MAP_SIZE_Y 42

#define MAX_PLAYERS 20

struct Player
{
	char name[20];
	int x, y;
	Player()
	{
		x = -1;
		y = -1;
	}
	Player(char _name[20], int _x, int _y)
	{
		strncpy_s(name, _name, 20);
		x = _x;
		y = _y;
	}
};

enum tiletype
{
	GROUND,
	GRASS,
	SAND,
	WATER,
	WALL
};

struct Tile
{
	enum tiletype type;
	bool walkable;
	Tile(tiletype _type, bool _walkable)
	{
		type = type;
		walkable = walkable;
	}
	Tile()
	{
		type = WALL;
		walkable = false;
	}
}; 

/*struct Map
{
};*/

Tile tile_map[MAP_SIZE_X][MAP_SIZE_Y];

int num_of_players = 0;
Player players[MAX_PLAYERS];

//Map map;

//Tile tile_map[MAP_SIZE_X][MAP_SIZE_Y];