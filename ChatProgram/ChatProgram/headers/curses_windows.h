#pragma once
#include <PDCurses/curses.h>

#include "game_structs.h"

WINDOW * win_system;
//WINDOW * win_system_borders;
WINDOW * win_input;
//WINDOW * win_input_borders;
WINDOW * win_map;
//WINDOW * win_map_borders;
WINDOW * win_extra;
//WINDOW * win_extra_borders;
WINDOW * win_chat;
//WINDOW * win_chat_borders;

std::vector<WINDOW*> windows;
std::vector<WINDOW*> window_borders;

#define TERM_COLOR_DEFAULT 1
#define TERM_COLOR_GRASS 2
#define TERM_COLOR_PLAYER 3
#define TERM_COLOR_WALL 4
#define TERM_COLOR_SAND 5
#define TERM_COLOR_WATER 6

#define MAP_WIN_SIZE_X 93
#define MAP_WIN_SIZE_Y 54

int my_print(WINDOW * win, const char * str, ...)
{
	
	int ret;

	va_list args;
	va_start(args, str);

	ret = vwprintw(win, str, args);
	wrefresh(win);

	va_end(args);

	return ret;
}

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
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
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
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}

/*void PrintMap(Tile** map, int offset_x, int offset_y)
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
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
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
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}*/

/*void PrintMap(Tile map[MAP_SIZE_X][MAP_SIZE_Y], int offset_x, int offset_y)
{
	for (int x = 0; x < MAP_WIN_SIZE_X; x++)
	{
		if (x >= MAP_SIZE_X) break;

		for (int y = 0; y < MAP_WIN_SIZE_Y; y++)
		{
			if (y >= MAP_SIZE_Y) break;

			switch (map[x + offset_x][y + offset_y].type)
			{
			case GROUND:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
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
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}*/

void PrintMap(std::vector<std::vector<Tile>> &map)
{
	for (int x = 0; x < MAP_WIN_SIZE_X && x < MAP_SIZE_X; x++)
	{
		if (x > MAP_SIZE_X) break;

		for (int y = 0; y < MAP_WIN_SIZE_Y && y < MAP_SIZE_Y; y++)
		{
			if (y > MAP_SIZE_Y) break;
			switch (map[x][y].type)
			{
			case GROUND:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
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
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}

/*void PrintMap(Tile** map)
{
	for (int x = 0; x < MAP_WIN_SIZE_X; x++)
	{
		if (x > MAP_SIZE_X) break;

		for (int y = 0; y < MAP_WIN_SIZE_Y; y++)
		{
			if (y > MAP_SIZE_Y) break;
			switch (map[x][y].type)
			{
			case GROUND:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
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
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}*/

/*void PrintMap(Tile map[MAP_SIZE_X][MAP_SIZE_Y])
{
	for (int x = 0; x < MAP_WIN_SIZE_X; x++)
	{
		if (x > MAP_SIZE_X) break;

		for (int y = 0; y < MAP_WIN_SIZE_Y; y++)
		{
			if (y > MAP_SIZE_Y) break;
			switch (map[x][y].type)
			{
			case GROUND:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 177);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
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
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}*/


/*void PrintMap(char map[MAP_SIZE_X][MAP_SIZE_Y])
{
	for (int x = 0; x < MAP_SIZE_X; x++)
	{
		for (int y = 0; y < MAP_SIZE_Y; y++)
		{
			switch (map[x][y])
			{
			case 'S':
				wattron(win_map, COLOR_PAIR(TERM_COLOR_SAND));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_SAND));
				break;
			case 'G':
				wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
				break;
			case 'B':
				wattron(win_map, COLOR_PAIR(TERM_COLOR_WATER));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_WATER));
				break;
			case 'W':
				wattron(win_map, COLOR_PAIR(TERM_COLOR_WALL));
				mvwaddch(win_map, y, x, 178);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_WALL));
				break;
			case '0':
				wattron(win_map, COLOR_PAIR(TERM_COLOR_WATER));
				mvwaddch(win_map, y, x, 176);
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_WATER));
				break;
			default:
				wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				mvwaddch(win_map, y, x, 'O');
				wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
				break;
			}
		}
	}
}*/


/*static BOOL _fit_console_window(HANDLE con_out, CONST SMALL_RECT *rect)
{
	SMALL_RECT run;
	SHORT mx, my;

	if (SetConsoleWindowInfo(con_out, TRUE, rect))
		return TRUE;

	run = *rect;
	run.Right /= 2;
	run.Bottom /= 2;

	mx = run.Right;
	my = run.Bottom;

	if (!SetConsoleWindowInfo(con_out, TRUE, &run))
		return FALSE;

	for (run.Right = rect->Right; run.Right >= mx; run.Right--)
		if (SetConsoleWindowInfo(con_out, TRUE, &run))
			break;

	if (run.Right < mx)
		return FALSE;

	for (run.Bottom = rect->Bottom; run.Bottom >= my; run.Bottom--)
		if (SetConsoleWindowInfo(con_out, TRUE, &run))
			return TRUE;

	return FALSE;
}*/

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


	/*int nlines = h;
	int ncols = w;
	HANDLE hand = GetStdHandle(STD_OUTPUT_HANDLE);
	SMALL_RECT rect;
	COORD coord;
	COORD size, max;
	max = GetLargestConsoleWindowSize(hand);

	rect.Left = rect.Top = 0;
	rect.Right = ncols - 1;

	if (rect.Right > max.X)
		rect.Right = max.X;

	rect.Bottom = nlines - 1;

	if (rect.Bottom > max.Y)
		rect.Bottom = max.Y;

	size.X = rect.Right + 1;
	size.Y = rect.Bottom + 1;

	_fit_console_window(hand, &rect);
	SetConsoleScreenBufferSize(hand, size);
	SetConsoleActiveScreenBuffer(hand);
	resize_term(h, w);*/

	//wprintw(win_system, "Screenbuffer Y:%i, X:%i\n", info->dwSize.Y	, info->dwSize.X);


	float left_w = 0.5f;
	float right_w = 1.0f - left_w;

	float st_h = 0.2f;
	float ma_h = 1.0f - st_h;

	float ch_h = 0.4f;
	float in_h = 0.15f;
	float ex_h = 1.0f - ch_h - in_h;

	wresize(win_system,	(int)(st_h*h-2), (int)(left_w*w-2));
	wresize(win_map,	(int)(ma_h*h-2), (int)(left_w*w-2));
	wresize(win_chat,	(int)(ch_h*h-2), (int)(right_w*w-2));
	wresize(win_input,	(int)(in_h*h-2), (int)(right_w*w-2));
	wresize(win_extra,	(int)(ex_h*h-2), (int)(right_w*w-2));
	wresize(window_borders[0], (int)(st_h*h), (int)(left_w*w));
	wresize(window_borders[1], (int)(ma_h*h), (int)(left_w*w));
	wresize(window_borders[2], (int)(ch_h*h), (int)(right_w*w));
	wresize(window_borders[3], (int)(in_h*h), (int)(right_w*w));
	wresize(window_borders[4], (int)(ex_h*h), (int)(right_w*w));

	mvwin(win_system,	0+1,						0+1);
	mvwin(win_map,		(int)(st_h*h+1),			0+1);
	mvwin(win_chat,		0+1,						(int)(left_w*w+1));
	mvwin(win_input,	(int)(ch_h*h+1),			(int)(left_w*w+1));
	mvwin(win_extra,	(int)((ch_h + in_h)*h + 1), (int)(left_w*w + 1));
	mvwin(window_borders[0], 0, 0);
	mvwin(window_borders[1], (int)(st_h*h), 0);
	mvwin(window_borders[2], 0, (int)(left_w*w));
	mvwin(window_borders[3], (int)(ch_h*h), (int)(left_w*w));
	mvwin(window_borders[4], (int)((ch_h + in_h)*h), (int)(left_w*w));

	wborder(window_borders[0], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[1], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[2], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[3], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[4], 0, 0, 0, 0, 0, 0, 0, 0);

	wprintw(win_system, "Window Y:%i, X:%i\n", h, w);
	wprintw(win_system, "Terminal Y:%i, X:%i\n", LINES, COLS);
}

void PrintPlayers(Player players[MAX_PLAYERS], int offset_x, int offset_y)
{
	wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
	for each (auto player in players)
	{
		if (player.x > -1 && player.y > -1 && player.x <= MAP_WIN_SIZE_X + offset_x && player.y <= MAP_WIN_SIZE_Y + offset_y)
		mvwaddch(win_map, player.y - offset_y, player.x - offset_x, '@');
	}
	wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
}

void PrintPlayers(Player players[MAX_PLAYERS])
{
	wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
	//wattron(win_map, A_BLINK);
	for each (auto player in players)
	{
		if(player.x > -1 && player.y > -1 && player.x <= MAP_WIN_SIZE_X && player.y <= MAP_WIN_SIZE_Y)
		mvwaddch(win_map, player.y , player.x, '@');
	}
	//wattroff(win_map, A_BLINK);
	wattroff(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
}

SYSTEMTIME* sys_time = new SYSTEMTIME();
void UpdateWindows()
{
	if (is_termresized())
	{
		
		//resize_term(0, 0);
		resize_term(0, 0);
		UpdateWindowSizes();
		SP->resized = false;
	}

	GetSystemTime(sys_time);
	wclear(win_extra);

	//wattron(win_extra, A_BLINK);
	my_print(win_extra, "%02i:%02i:%02i | %02i.%02i.%04i\n", sys_time->wHour, sys_time->wMinute, sys_time->wSecond, sys_time->wDay, sys_time->wMonth, sys_time->wYear);
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
	float ex_h = 1.0f - ch_h - in_h;

	getmaxyx(stdscr, h, w);

	win_system = MakeWindowWithBorders((int)(st_h*h), (int)(left_w*w), 0, 0);

	win_map = MakeWindowWithBorders((int)(ma_h*h), (int)(left_w*w), (int)(st_h*h), 0, false);

	win_chat = MakeWindowWithBorders((int)(ch_h*h), (int)(right_w*w), 0, (int)(left_w*w));

	win_input = MakeWindowWithBorders((int)(in_h*h), (int)(right_w*w), (int)(ch_h*h), (int)(left_w*w));

	win_extra = MakeWindowWithBorders((int)(ex_h*h), (int)(right_w*w), (int)((ch_h + in_h)*h), (int)(left_w*w));

	cbreak();
	noecho();
	curs_set(0);

	start_color();
	PDC_set_blink(true);

	init_pair(TERM_COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
	init_pair(TERM_COLOR_GRASS, COLOR_BLACK, COLOR_GREEN);
	init_pair(TERM_COLOR_PLAYER, COLOR_BLUE, COLOR_GREEN);
	init_pair(TERM_COLOR_WALL, COLOR_BLACK, COLOR_WHITE);
	init_pair(TERM_COLOR_SAND, COLOR_BLACK, COLOR_YELLOW);
	init_pair(TERM_COLOR_WATER, COLOR_CYAN, COLOR_BLUE);

	keypad(win_input, TRUE);

}