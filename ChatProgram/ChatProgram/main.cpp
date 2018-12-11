#pragma comment(lib, "pdcurses64.lib")
#pragma comment(lib, "enet64.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <thread>
#include <mutex>
#include <string>
#include <cstring>

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




int main(int argc, char ** argv)
{
	InitWindows();
	int w, h;
	getmaxyx(stdscr, h, w);

	wprintw(win_system, "Window Y:%i, X:%i\n", h, w);
	wprintw(win_system,"This is the system window.\n");
	wprintw(win_input, "This is the input window.\n");
	wprintw(win_map, "This is the map window.\n");
	wprintw(win_extra, "This is the extra window.\n");
	wprintw(win_chat, "This is the chat window.\n");

	if (enet_initialize() != 0)
	{
		wprintw(win_system, "An error occurred while initializing ENet.");
		return EXIT_FAILURE;
	}

	wprintw(win_system, "1:Server || 2:Client\n");

	UpdateWindows();

	int input = wgetch(win_input);

	if (input == '1')
	{
		ENetHost* server = CreateServer();
		std::thread server_thread(ServerThread, 0, server, &running);

		server_thread.join();
		DestroyClientOrHost(server);
	}
	else if (input == '2')
	{
		ENetHost* client = CreateClient();

		nodelay(win_input, true);
		wprintw(win_input, "Address (Leave empty for localhost):");
		char addr[30];
		echo();
		wgetstr(win_input, addr);
		noecho();

		ENetPeer* peer = ConnectToHost(client, addr);
		std::thread client_thread(ClientThread, 0, client, peer, &running);
		
		client_thread.join();
		DisconnectPeer(peer, client);
		DestroyClientOrHost(client);
	}

	atexit(enet_deinitialize); 
	endwin();
}