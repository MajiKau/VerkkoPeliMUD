#pragma once
#include <PDCurses/curses.h>

#include "game_structs.h"

WINDOW * win_system;
WINDOW * win_input;
WINDOW * win_map;
WINDOW * win_extra;
WINDOW * win_chat;
WINDOW * win_player;

std::vector<WINDOW*> windows;
std::vector<WINDOW*> window_borders;

#define TERM_COLOR_DEFAULT 1
#define TERM_COLOR_GRASS 2
#define TERM_COLOR_PLAYER 3
#define TERM_COLOR_WALL 4
#define TERM_COLOR_SAND 5
#define TERM_COLOR_WATER 6
#define TERM_COLOR_GROUND 7
#define TERM_COLOR_FLOOR 8
#define TERM_COLOR_SPAWN 9
#define TERM_COLOR_DOORCLOSED 10
#define TERM_COLOR_DOOROPEN 11
#define TERM_COLOR_HOLE 12

#define MAP_WIN_SIZE_X 93
#define MAP_WIN_SIZE_Y 54

SYSTEMTIME* sys_time = new SYSTEMTIME();

void PrintMap(std::vector<std::vector<Tile>> &map, int offset_x, int offset_y)
{
	for (int x = 0; x < MAP_WIN_SIZE_X && x < MAP_SIZE_X; x++)
	{
		if (x >= MAP_SIZE_X) break;

		for (int y = 0; y < MAP_WIN_SIZE_Y && y < MAP_SIZE_Y; y++)
		{
			if (y >= MAP_SIZE_Y) break;

			switch (map[x + offset_x][y + offset_y].type)
			{
			case GROUND:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GROUND));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GROUND));
				break;
			case GRASS:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				break;
			case SAND:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_SAND));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_SAND));
				break;
			case WATER:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_WATER));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_WATER));
				break;
			case WALL:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_WALL));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_WALL));
				break;
			case FLOOR:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_FLOOR));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_FLOOR));
				break;
			case SPAWN:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_SPAWN));
				mvwaddch(win_map, y, x, 'X');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_SPAWN));
				break;
			case ANIMALSPAWN:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_SPAWN));
				mvwaddch(win_map, y, x, 'Z');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_SPAWN));
				break;
			case DOOR:
				if (map[x + offset_x][y + offset_y].walkable) 
				{
					wattron(win_map, COLOR_PAIR(TERM_COLOR_DOOROPEN));
					mvwaddch(win_map, y, x, '#');
					wattroff(win_map, COLOR_PAIR(TERM_COLOR_DOOROPEN));
				}
				else
				{
					wattron(win_map, COLOR_PAIR(TERM_COLOR_DOORCLOSED));
					mvwaddch(win_map, y, x, '#');
					wattroff(win_map, COLOR_PAIR(TERM_COLOR_DOORCLOSED));

				}
				break;
			case HOLE:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_HOLE));
				mvwaddch(win_map, y, x, 178);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_HOLE));
				break;
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}

void UpdateWindowSizes()
{
	clear();
	for each (auto win_b in window_borders)
	{
		wclear(win_b);
	}
	for each (auto win in windows)
	{
		wclear(win);
	}

	int w, h;
	getmaxyx(stdscr, h, w); 

	float left_w = 0.5f;
	float right_w = 1.0f - left_w;

	float st_h = 0.2f;
	float ma_h = 1.0f - st_h;

	float ch_h = 0.4f;
	float in_h = 0.15f;
	float ex_h = 0.1f;
	float pl_h = 1.0f - ch_h - in_h - ex_h;

	wresize(win_system,	(int)(st_h*h-2), (int)(left_w*w-2));
	wresize(win_map,	(int)(ma_h*h-2), (int)(left_w*w-2));
	wresize(win_chat,	(int)(ch_h*h-2), (int)(right_w*w-2));
	wresize(win_input,	(int)(in_h*h-2), (int)(right_w*w-2));
	wresize(win_extra,	(int)(ex_h*h-2), (int)(right_w*w-2));
	wresize(win_player, (int)(pl_h*h-2), (int)(right_w*w-2));

	wresize(window_borders[0], (int)(st_h*h), (int)(left_w*w));
	wresize(window_borders[1], (int)(ma_h*h), (int)(left_w*w));
	wresize(window_borders[2], (int)(ch_h*h), (int)(right_w*w));
	wresize(window_borders[3], (int)(in_h*h), (int)(right_w*w));
	wresize(window_borders[4], (int)(ex_h*h), (int)(right_w*w));
	wresize(window_borders[5], (int)(pl_h*h), (int)(right_w*w));

	mvwin(win_system,	0+1,								0+1);
	mvwin(win_map,		(int)(st_h*h+1),					0+1);
	mvwin(win_chat,		0+1,								(int)(left_w*w+1));
	mvwin(win_input,	(int)(ch_h*h+1),					(int)(left_w*w+1));
	mvwin(win_extra,	(int)((ch_h + in_h)*h + 1),			(int)(left_w*w + 1));
	mvwin(win_player,   (int)((ch_h + in_h + ex_h)*h + 1),	(int)(left_w*w + 1));
	mvwin(window_borders[0], 0, 0);
	mvwin(window_borders[1], (int)(st_h*h), 0);
	mvwin(window_borders[2], 0, (int)(left_w*w));
	mvwin(window_borders[3], (int)(ch_h*h), (int)(left_w*w));
	mvwin(window_borders[4], (int)((ch_h + in_h)*h), (int)(left_w*w));
	mvwin(window_borders[5], (int)((ch_h + in_h + ex_h)*h), (int)(left_w*w));

	wborder(window_borders[0], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[1], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[2], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[3], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[4], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[5], 0, 0, 0, 0, 0, 0, 0, 0);

	wprintw(win_system, "Window Y:%i, X:%i\n", h, w);
	wprintw(win_system, "Terminal Y:%i, X:%i\n", LINES, COLS);
}

void PrintPlayers(Player players[MAX_PLAYERS], int offset_x, int offset_y, std::string name = "")
{
	wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
	for each (auto player in players)
	{
		if (player.x > -1 && player.y > -1 && player.x <= MAP_WIN_SIZE_X + offset_x && player.y <= MAP_WIN_SIZE_Y + offset_y)
		{
			if (player.name == name)
			{
				mvwaddch(win_map, player.y - offset_y, player.x - offset_x, '@');
			}
			else
			{
				mvwaddch(win_map, player.y - offset_y, player.x - offset_x, '$');
			}
		}
	}
	wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
}

void PrintPlayerData(Player player)
{
	wclear(win_player);
	wprintw(win_player, "Name: %s\nScore: %d\nDirt: %d\n", player.name, player.score, player.dirt_dug);
}


void PrintAnimals(Animal animals[MAX_ANIMALS], int offset_x, int offset_y)
{
	wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
	for each (auto animal in animals)
	{
		if (animal.x > -1 && animal.x > offset_x && animal.y > -1 && animal.y > offset_y && animal.x <= MAP_WIN_SIZE_X + offset_x && animal.y <= MAP_WIN_SIZE_Y + offset_y)
		{
			mvwaddch(win_map, animal.y - offset_y, animal.x - offset_x, '*');
		}
	}
	wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
}

void UpdateWindows()
{
	if (is_termresized())
	{
		resize_term(0, 0);
		UpdateWindowSizes();
		SP->resized = false;
	}

	GetSystemTime(sys_time);
	wclear(win_extra);

	//wattron(win_extra, A_BLINK);
	wprintw(win_extra, "%02i:%02i:%02i | %02i.%02i.%04i\n", sys_time->wHour, sys_time->wMinute, sys_time->wSecond, sys_time->wDay, sys_time->wMonth, sys_time->wYear);
	wprintw(win_extra, "\nW/A/S/D to move. L+Direction to look, K+Direction to dig/fill a hole.\n Walk into a door to open it. Dig holes under animals to score points.\nT to talk.");
	//wattroff(win_extra, A_BLINK);

	refresh();
	for each (auto win_b in window_borders)
	{
		wrefresh(win_b);
	}
	for each (auto win in windows)
	{
		wrefresh(win);
	}

	//Sleep(10);
}

WINDOW * MakeWindowWithBorders(int h, int w, int y, int x, bool auto_scroll = true)
{
	WINDOW * borders = newwin(h, w, y, x);
	wborder(borders, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(borders);
	window_borders.push_back(borders);
	WINDOW * win = newwin(h - 2, w - 2, y + 1, x + 1);
	scrollok(win, auto_scroll);
	wrefresh(win);
	windows.push_back(win);

	return win;
}



void InitWindows()
{
	window_borders = std::vector<WINDOW*>();
	windows = std::vector<WINDOW*>();

	initscr();
	resize_term(70, 190);

	int w, h;

	float left_w = 0.5f;
	float right_w = 1.0f - left_w;

	float st_h = 0.2f;
	float ma_h = 1.0f - st_h;

	float ch_h = 0.4f;
	float in_h = 0.15f;
	float ex_h = 0.1f;
	float pl_h = 1.0f - ch_h - in_h - ex_h;

	getmaxyx(stdscr, h, w);

	win_system = MakeWindowWithBorders((int)(st_h*h), (int)(left_w*w), 0, 0);

	win_map = MakeWindowWithBorders((int)(ma_h*h), (int)(left_w*w), (int)(st_h*h), 0, false);

	win_chat = MakeWindowWithBorders((int)(ch_h*h), (int)(right_w*w), 0, (int)(left_w*w));

	win_input = MakeWindowWithBorders((int)(in_h*h), (int)(right_w*w), (int)(ch_h*h), (int)(left_w*w));

	win_extra = MakeWindowWithBorders((int)(ex_h*h), (int)(right_w*w), (int)((ch_h + in_h)*h), (int)(left_w*w));

	win_player = MakeWindowWithBorders((int)(pl_h*h), (int)(right_w*w), (int)((ch_h + in_h + ex_h)*h), (int)(left_w*w));

	cbreak();
	noecho();
	curs_set(0);

	start_color();
	PDC_set_blink(true); //Blinking text only works while waiting for input.

	init_pair(TERM_COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
	init_pair(TERM_COLOR_GRASS, COLOR_BLACK, COLOR_GREEN);
	init_pair(TERM_COLOR_PLAYER, COLOR_BLUE, COLOR_GREEN);
	init_pair(TERM_COLOR_WALL, COLOR_BLACK, COLOR_WHITE);
	init_pair(TERM_COLOR_SAND, COLOR_BLACK, COLOR_YELLOW);
	init_pair(TERM_COLOR_WATER, COLOR_CYAN, COLOR_BLUE);
	init_pair(TERM_COLOR_GROUND, COLOR_RED, COLOR_YELLOW);
	init_pair(TERM_COLOR_FLOOR, COLOR_YELLOW, COLOR_GREEN);
	init_pair(TERM_COLOR_SPAWN, COLOR_WHITE, COLOR_BLUE);
	init_pair(TERM_COLOR_DOOROPEN, COLOR_WHITE, COLOR_YELLOW);
	init_pair(TERM_COLOR_DOORCLOSED, COLOR_BLACK, COLOR_YELLOW);
	init_pair(TERM_COLOR_HOLE, COLOR_BLACK, COLOR_GREEN);
	
	keypad(win_input, TRUE);

}