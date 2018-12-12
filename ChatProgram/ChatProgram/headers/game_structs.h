#pragma once
#include <vector>

#define MAP_SIZE_X 300
#define MAP_SIZE_Y 300

#define MAX_PLAYERS 20
#define MAX_ANIMALS 200

struct Player
{
	char name[20];
	int x, y;
	int score;
	int dirt_dug;
	unsigned int last_input_sequence_number = 0;
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
		score = 0;
		dirt_dug = 0;
	}
};

struct Animal
{
	int x, y;
	float cooldown;
	Animal()
	{
		x = -1;
		y = -1;
	}
	Animal(int _x, int _y)
	{
		x = _x;
		y = _y;
		cooldown = 1.0f;
	}
	void Respawn(std::vector<std::pair<unsigned int, unsigned int>> spawns)
	{
		int i = rand() % spawns.size();
		x = spawns[i].first;
		y = spawns[i].second;
	}
};

enum tiletype
{
	GROUND,
	GRASS,
	SAND,
	WATER,
	WALL,
	FLOOR,
	SPAWN,
	DOOR,
	ANIMALSPAWN,
	HOLE
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


std::vector<std::vector<Tile>> tile_map(MAP_SIZE_X, std::vector<Tile>(MAP_SIZE_Y, Tile()));

int num_of_players = 0;
Player players[MAX_PLAYERS];
Player players_copy[MAX_PLAYERS];

Animal animals[MAX_ANIMALS];
