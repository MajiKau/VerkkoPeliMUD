#pragma once
//#include <enet/enet.h>
#include "curses_windows.h"
#include "messages.h"

ENetPeer * ConnectToHost(ENetHost* client)
{
	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;
	/* Connect to localhost:8888. */
	enet_address_set_host(&address, "localhost");
	address.port = 8888;
	/* Initiate the connection, allocating the two channels 0 and 1. */
	peer = enet_host_connect(client, &address, 2, 0);
	if (peer == NULL)
	{
		wprintw(win_system, "No available peers for initiating an ENet connection.\n");

		exit(EXIT_FAILURE);
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		wprintw(win_system, "Connection to localhost:8888 succeeded.");

		//DO THINGS HERE
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds   */
		/* had run out without any significant event.            */
		enet_peer_reset(peer);

		wprintw(win_system, "Connection to localhost:8888 failed.");
	}

	//DO THINGS HERE
	return peer;
}


ENetHost * CreateClient()
{
	ENetHost * client;
	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		0 /* assume any amount of incoming bandwidth */,
		0 /* assume any amount of incoming bandwidth */);
	if (client == NULL)
	{
		wprintw(win_system, "An error occurred while trying to create an ENet client host.");

		exit(EXIT_FAILURE);
	}


	//DO THINGS HERE
	return client;

	//enet_host_destroy(client);
}

void InputWindowThread(ENetPeer * peer, bool* running) //Not thread safe, do not use
{
	char name[20];
	echo();
	wgetstr(win_input, name);
	noecho();

	SendMessageToPeer(peer, &JoinP(name));

	char * msg;

	while (running)
	{
		wrefresh(win_input);

		char input = wgetch(win_input);
		switch (input)
		{
		case KEY_RESIZE:
			//UpdateWindowSizes();
			break;
		case 'r':
			break;
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

void HandleInput(ENetPeer * peer, std::string name)
{

	char * msg;

	char input = wgetch(win_input);
	switch (input)
	{
	case KEY_RESIZE:
		resize_term(0, 0);
		UpdateWindowSizes();
		break;

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
				//running = false;
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

void ClientThread(int id, ENetHost* client, ENetPeer* peer, bool* running)
{
	ENetEvent event;
	nodelay(win_input, true);
	wprintw(win_input, "Username:");

	char name[20];
	echo();
	wgetstr(win_input, name);
	noecho();

	SendMessageToPeer(peer, &JoinP(name));

	while (running)
	{
		HandleInput(peer, name);
		wprintw(win_system, "-W-");
		UpdateWindows();

		int enet_int = enet_host_service(client, &event, 0);
		MessagePacket pack;
		/* Wait up to 0 milliseconds for an event. */
		if (enet_int > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				wprintw(win_system, "Someone is trying to connect to a client?");
				break;
			case ENET_EVENT_TYPE_RECEIVE:

				pack = *(MessagePacket*)event.packet->data;

				wprintw(win_chat, "%s: %s\n", pack.sender, pack.message);

				if (pack.message == "/stop")
				{
					wprintw(win_system, "Server shutdown");
				}

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				wprintw(win_system, "Lost connection to server.");
				running = false;
				break;
			}
		}
		//Sleep(10);
	}
}