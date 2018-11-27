#pragma once
#include <vector>

#define MAP_SIZE_X 301
#define MAP_SIZE_Y 80

#define MAX_PLAYERS 20

struct Player
{
	char name[20];
	int x, y;
	int score;
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
	}
};

struct Animal
{
	int x, y;
	float cooldown;
	Animal(int _x, int _y)
	{
		x = _x;
		y = _y;
		cooldown = 1.0f;
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
	ANIMALSPAWN

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
//Tile tile_map[MAP_SIZE_X][MAP_SIZE_Y];
//Tile** tile_map;
//Tile* tile_map;

int num_of_players = 0;
Player players[MAX_PLAYERS];
Player players_copy[MAX_PLAYERS];

std::vector<Animal> animals;
