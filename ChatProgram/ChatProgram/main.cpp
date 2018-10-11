//#pragma comment(lib, "pdcurses.lib")
//#pragma comment(lib, "enet.lib")
#pragma comment(lib, "pdcurses64.lib")
#pragma comment(lib, "enet64.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")
#include <enet/enet.h>
#include <PDCurses/curses.h>
//#include <winsock2.h>
//#include <stdio.h>
#include <thread>
#include <mutex>
#include <string>
#include <cstring>
//#include <iostream>
//#include <Windows.h>

#include <map>

#include <vector>
#include <set>

#include <cstdarg>
#include <stdarg.h>

#include "client.h"
#include "server.h"
#include "curses_windows.h"

bool running = true;

void DestroyClientOrHost(ENetHost * host)
{
	enet_host_destroy(host);
}

void InputThread(ENetPeer * peer)
{
	char name[20];
	wprintw(win_input, "Username:");

	echo();
	wgetstr(win_input, name);
	noecho();

	SendMessageToPeer(peer, &JoinP(name));

	char * msg;
	while (running)
	{
		UpdateWindows();

		char input = wgetch(win_input);
		switch (input)
		{
		case 'w':
			msg = new char[2]{ "w" };
			SendMessageToPeer(peer, &MovementP(name, input));
			break;
		case 'a':
			msg = new char[2]{ "a" };
			SendMessageToPeer(peer, &MovementP(name, input));
			break;
		case 's':
			msg = new char[2]{ "s" };
			SendMessageToPeer(peer, &MovementP(name, input));
			break;
		case 'd':
			msg = new char[2]{ "d" };
			SendMessageToPeer(peer, &MovementP(name, input));
			break;
		case 't':

			msg = new char[1000];
			flushinp();
			wprintw(win_input, "MSG:");

			echo();
			wgetstr(win_input, msg);
			noecho();

			wprintw(win_chat, "%s: %s\n", name, msg);

			if (msg[0] == '/')
			{
				if (msg == "/quit")
				{
					running = false;
					break;
				}
				if (msg == "/stop")
				{
					SendMessageToPeer(peer, &MessageP(name, msg));
				}
			}
			else
			{
				SendMessageToPeer(peer, &MessageP(name, msg));
			}
			break;
		default:
			break;
		}
	}
}



int main(int argc, char ** argv)
{
	initscr(); 
	resize_term(70, 190);

	int w, h;
	getmaxyx(stdscr, h, w);
	stdscr = MakeWindowWithBorders(19, 90, 0, 0);

	win_input = MakeWindowWithBorders(10, 99, 31, 91);

	win_map = MakeWindowWithBorders(50, 90, 20, 0, false);
	
	win_chat = MakeWindowWithBorders(30, 99, 0, 91);

	win_extra = MakeWindowWithBorders(28, 99, 42, 91);

	cbreak();
	noecho();

	start_color();
	PDC_set_blink(true);

	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_GREEN);
	init_pair(3, COLOR_BLUE, COLOR_GREEN);


	keypad(win_input, TRUE);

	wprintw(stdscr, "Window Y:%i, X:%i\n", h, w);
	wprintw(stdscr,"This is the stdscr window.\n");
	wprintw(win_input, "This is the input window.\n");
	wprintw(win_map, "This is the map window.\n");
	wprintw(win_extra, "This is the extra window.\n");
	wprintw(win_chat, "This is the chat window.\n");
	//mvwaddch(win2, 3, 10, 'O');
	

	if (enet_initialize() != 0)
	{
		wprintw(stdscr, "An error occurred while initializing ENet.");
		return EXIT_FAILURE;
	}

	wprintw(stdscr, "1:Server || 2:Client\n");

	wrefresh(win_input_borders);
	refresh();

	wrefresh(win1_borders);
	wrefresh(win_map);

	wrefresh(win2_borders);
	wrefresh(win_extra);

	wrefresh(win_chat_borders);
	wrefresh(win_chat);

	int input = wgetch(win_input);

	if (input == '1')
	{
		ENetHost* server = CreateServer();
		std::thread server_thread(ServerThread, 0, server, &running);
		while (getch() != 'q')
		{

		}
		server_thread.join();
		DestroyClientOrHost(server);
	}
	else if (input == '2')
	{
		ENetHost* client = CreateClient();
		ENetPeer* peer = ConnectToHost(client);
		std::thread client_thread(ClientThread, 0, client, &running);
		std::thread message_thread(InputThread, peer);
		
		client_thread.join();
		message_thread.join();
		DisconnectPeer(peer, client);
		DestroyClientOrHost(client);
	}

	atexit(enet_deinitialize); 
	endwin();
}