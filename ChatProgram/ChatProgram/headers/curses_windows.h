#pragma once
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
	wattron(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
	for (int x = 0; x < 80; x++)
	{
		for (int y = 0; y < 40; y++)
		{

			mvwaddch(win_map, y, x, map[x][y]); 
		}
	}
	//attron(COLOR_PAIR(2));
	wattroff(win_map, COLOR_PAIR(TERM_COLOR_GRASS));
}
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

	wprintw(win_system, "Window Y:%i, X:%i\n", h, w);
	wprintw(win_system, "Terminal Y:%i, X:%i\n", LINES, COLS);
}

void PrintPlayers(std::vector<Player> players)
{
	wattron(win_map, COLOR_PAIR(TERM_COLOR_PLAYER));
	wattron(win_map, A_BLINK);
	for each (auto player in players)
	{
		mvwaddch(win_map, player.y , player.x, '@');
	}
	wattroff(win_map, A_BLINK);
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

	win_system = MakeWindowWithBorders(st_h*h, left_w*w, 0, 0);

	win_map = MakeWindowWithBorders(ma_h*h, left_w*w, st_h*h, 0, false);

	win_chat = MakeWindowWithBorders(ch_h*h, right_w*w, 0, left_w*w);

	win_input = MakeWindowWithBorders(in_h*h, right_w*w, ch_h*h, left_w*w);

	win_extra = MakeWindowWithBorders(ex_h*h, right_w*w, (ch_h + in_h)*h, left_w*w);

	cbreak();
	noecho();
	curs_set(0);

	start_color();
	PDC_set_blink(true);

	init_pair(TERM_COLOR_DEFAULT, COLOR_WHITE, COLOR_BLACK);
	init_pair(TERM_COLOR_GRASS, COLOR_BLACK, COLOR_GREEN);
	init_pair(TERM_COLOR_PLAYER, COLOR_BLUE, COLOR_GREEN);

	keypad(win_input, TRUE);

}