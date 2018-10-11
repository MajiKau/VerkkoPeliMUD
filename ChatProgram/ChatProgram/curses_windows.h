#pragma once
#include "structs.h"

WINDOW * win_input;
WINDOW * win_input_borders;
WINDOW * win_map;
WINDOW * win1_borders;
WINDOW * win_extra;
WINDOW * win2_borders;
WINDOW * win_chat;
WINDOW * win_chat_borders;

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
	SYSTEMTIME* sys_time = new SYSTEMTIME();
	GetSystemTime(sys_time);
	wclear(win_extra);
	my_print(win_extra, "%02i:%02i:%02i | %02i.%02i.%04i\n", sys_time->wHour, sys_time->wMinute, sys_time->wSecond, sys_time->wDay, sys_time->wMonth, sys_time->wYear);

	refresh();
	wrefresh(win_input);
	wrefresh(win_map);
	wrefresh(win_extra);
	wrefresh(win_chat);
	Sleep(10);
}

WINDOW * MakeWindowWithBorders(int h, int w, int y, int x, bool auto_scroll = true)
{
	WINDOW * borders = newwin(h, w, y, x);
	wborder(borders, 0, 0, 0, 0, 0, 0, 0, 0);
	wrefresh(borders);
	WINDOW * win = newwin(h - 2, w - 2, y + 1, x + 1);
	scrollok(win, auto_scroll);
	wrefresh(win);
	return win;
}