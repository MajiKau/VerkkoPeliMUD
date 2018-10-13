#pragma once
#include "structs.h"

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

#define COLOR_DEFAULT 1
#define COLOR_GRASS 2
#define COLOR_PLAYER 3


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

void PrintMap(char map[80][40])
{
	wattron(win_map, COLOR_PAIR(COLOR_GRASS));
	for (int x = 0; x < 80; x++)
	{
		for (int y = 0; y < 40; y++)
		{

			mvwaddch(win_map, y, x, map[x][y]); 
		}
	}
	//attron(COLOR_PAIR(2));
	wattroff(win_map, COLOR_PAIR(COLOR_GRASS));
}

void UpdateWindowSizes()
{

	int w, h;
	getmaxyx(stdscr, h, w);

	float left_w = 0.5f;
	float right_w = 1.0f - left_w;

	float st_h = 0.2f;
	float ma_h = 1.0f - st_h;

	float ch_h = 0.4f;
	float in_h = 0.15f;
	float ex_h = 1.0f - ch_h - in_h;

	wresize(win_system,	st_h*h-2, left_w*w-2);
	wresize(win_map,	ma_h*h-2, left_w*w-2);
	wresize(win_chat,	ch_h*h-2, right_w*w-2);
	wresize(win_input,	in_h*h-2, right_w*w-2);
	wresize(win_extra,	ex_h*h-2, right_w*w-2);
	wresize(window_borders[0], st_h*h, left_w*w);
	wresize(window_borders[1], ma_h*h, left_w*w);
	wresize(window_borders[2], ch_h*h, right_w*w);
	wresize(window_borders[3], in_h*h, right_w*w);
	wresize(window_borders[4], ex_h*h, right_w*w);

	mvwin(win_system,	0+1,				0+1);
	mvwin(win_map,		st_h*h+1,			0+1);
	mvwin(win_chat,		0+1,				left_w*w+1);
	mvwin(win_input,	ch_h*h+1,			left_w*w+1);
	mvwin(win_extra,	(ch_h + in_h)*h+1,	left_w*w+1);
	mvwin(window_borders[0], 0, 0);
	mvwin(window_borders[1], st_h*h, 0);
	mvwin(window_borders[2], 0, left_w*w);
	mvwin(window_borders[3], ch_h*h, left_w*w);
	mvwin(window_borders[4], (ch_h + in_h)*h, left_w*w);

	wborder(window_borders[0], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[1], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[2], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[3], 0, 0, 0, 0, 0, 0, 0, 0);
	wborder(window_borders[4], 0, 0, 0, 0, 0, 0, 0, 0);
}

void PrintPlayers(std::vector<Player> players)
{
	wattron(win_map, COLOR_PAIR(COLOR_PLAYER));
	wattron(win_map, A_BLINK);
	for each (auto player in players)
	{
		mvwaddch(win_map, player.y , player.x, '@');
	}
	wattroff(win_map, A_BLINK);
	wattroff(win_map, COLOR_PAIR(COLOR_PLAYER));
}

void UpdateWindows()
{
	if (is_termresized())
	{
		resize_term(0, 0);
		UpdateWindowSizes();
	}

	SYSTEMTIME* sys_time = new SYSTEMTIME();
	GetSystemTime(sys_time);
	wclear(win_extra);

	//wattron(win_extra, A_BLINK);
	my_print(win_extra, "%02i:%02i:%02i | %02i.%02i.%04i\n", sys_time->wHour, sys_time->wMinute, sys_time->wSecond, sys_time->wDay, sys_time->wMonth, sys_time->wYear);
	//wattroff(win_extra, A_BLINK);

	for each (auto win_b in window_borders)
	{
		wrefresh(win_b);
	}
	for each (auto win in windows)
	{
		wrefresh(win);
	}

	/*wrefresh(win_system);
	wrefresh(win_input);
	wrefresh(win_map);
	wrefresh(win_extra);
	wrefresh(win_chat*/
	Sleep(10);
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

	win_system = MakeWindowWithBorders(st_h*h, left_w*w, 0, 0);

	win_map = MakeWindowWithBorders(ma_h*h, left_w*w, st_h*h, 0, false);

	win_chat = MakeWindowWithBorders(ch_h*h, right_w*w, 0, left_w*w);

	win_input = MakeWindowWithBorders(in_h*h, right_w*w, ch_h*h, left_w*w);

	win_extra = MakeWindowWithBorders(ex_h*h, right_w*w, (ch_h + in_h)*h, left_w*w);

}