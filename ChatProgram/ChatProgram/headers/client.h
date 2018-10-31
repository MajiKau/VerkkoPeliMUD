#pragma once
#include <enet/enet.h>

#include "game_structs.h"
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

/*void InputWindowThread(ENetPeer * peer, bool* running) //Not thread safe, do not use
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
}*/
bool typing = false;
char* buffer;
int index;

void HandleInput(ENetPeer * peer, std::string name)
{
	if (!typing)
	{
		char * msg;
		std::string str;

		char input = wgetch(win_input);
		switch (input)
		{
		case KEY_RESIZE:
			resize_term(0, 0);
			UpdateWindowSizes();
			break;

		case 'w':
			SendMessageToPeer(peer, &MovementP(name, NORTH));
			break;

		case 'a':
			SendMessageToPeer(peer, &MovementP(name, WEST));
			break;

		case 's':
			SendMessageToPeer(peer, &MovementP(name, SOUTH));
			break;

		case 'd':
			SendMessageToPeer(peer, &MovementP(name, EAST));
			break;

		case 'l':
			nodelay(win_input, false);
			input = wgetch(win_input);
			nodelay(win_input, true);
			switch (input)
			{
			case 'w':
				SendMessageToPeer(peer, &LookP(name, NORTH));
				break;

			case 'a':
				SendMessageToPeer(peer, &LookP(name, WEST));
				break;

			case 's':
				SendMessageToPeer(peer, &LookP(name, SOUTH));
				break;

			case 'd':
				SendMessageToPeer(peer, &LookP(name, EAST));
				break;

			case 'l':
				SendMessageToPeer(peer, &LookP(name, BELOW));
				break;
			}
			break;

		case 't':
			typing = true;

			buffer = new char[1000];
			index = 0;
			wprintw(win_input, "MSG:");

			/*msg = new char[1000];


			flushinp();
			wprintw(win_input, "MSG:");
			echo();

			wgetstr(win_input, msg);
			noecho();
			wprintw(win_chat, "%s: %s\n", name.c_str(), msg);

			str = std::string(msg);
			if (msg[0] == '/')
			{
				if (str == "/quit")
				{
					//running = false;
					exit(0);
					break;
				}
				if (str == "/stop")
				{
					SendMessageToPeer(peer, &MessageP(name, msg));
				}
			}
			else if(str != "")
			{
				SendMessageToPeer(peer, &MessageP(name, msg));
			}*/
			break;

		default:
			break;

		}
	}
	else
	{
		char input = wgetch(win_input);
		switch (input)
		{
		case 10:
			buffer[index] = '\0';
			index++;
			SendMessageToPeer(peer, &MessageP(name, buffer));
			typing = false;
			wprintw(win_input, "\n");
			break;

		case 8:
			if (index > 0)
			{
				wprintw(win_input, "%c", input);
				wdelch(win_input);
				index--;
				buffer[index] = '\0';
			}
			break;

		default:
			if (input != -1)
			{
				wprintw(win_input, "%c", input);
				buffer[index] = input;
				index++;
				buffer[index] = '\0';
			}
			break;
		}
	}
}

void ClientThread(int id, ENetHost* client, ENetPeer* peer, bool* running)
{

	Packet* pack;
	MessagePacket* msg_pack;
	LookPacket* look_pack;
	MovePacket* move_pack;
	JoinPacket* join_pack;
	MapPacket* map_pack;
	PlayersPacket* players_pack;

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
		//wprintw(win_system, "-W-");
		PrintMap(tile_map);
		PrintPlayers(players);

		UpdateWindows();

		int enet_int = enet_host_service(client, &event, 0);
		//MessagePacket msg_pack;
		/* Wait up to 0 milliseconds for an event. */
		if (enet_int > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				wprintw(win_system, "Someone is trying to connect to a client?");
				break;
			case ENET_EVENT_TYPE_RECEIVE:
				switch (((Packet*)(event.packet->data))->type)
				{
				case CHAT:
					msg_pack = (MessagePacket*)event.packet->data;

					wprintw(win_chat, "%s: %s\n", msg_pack->sender, msg_pack->message);

					if (msg_pack->message == "/stop")
					{
						wprintw(win_system, "Server shutdown");
					}

					break;

				case MAP:
					map_pack = (MapPacket*)event.packet->data;
					for (int x = 0; x < MAP_SIZE_X; x++)
					{
						for (int y = 0; y < MAP_SIZE_Y; y++)
						{
							tile_map[x][y] = map_pack->map[x][y];
						}
					}
					break;

				case PLAYERS:
					players_pack = (PlayersPacket*)event.packet->data;
					for (int i = 0; i < MAX_PLAYERS; i++)
					{
						players[i] = players_pack->players[i];
					}
					break;

				default:
					break;

				}


				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				wprintw(win_system, "Lost connection to server.");
				running = false;
				break;
			}

			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);
		}
		//Sleep(10);
	}
}