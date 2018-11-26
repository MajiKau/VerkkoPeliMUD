#pragma once
#include <enet/enet.h>
#include <list>

#include "game_structs.h"
#include "curses_windows.h"
#include "messages.h"

static unsigned int client_packet_sequence = 0;
static unsigned int server_packet_sequence = 0;

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

std::list <std::pair<unsigned int, Direction>> inputs;

void MovementPrediction(Direction direction)
{
	inputs.push_back({ client_packet_sequence ,direction });
	client_packet_sequence++;
}

void PlayerReconciliation(std::string name)
{
	if (server_packet_sequence == client_packet_sequence)
	{
		return;
	}

	if (inputs.size() != 0)
	{
		while (server_packet_sequence >= inputs.front().first)
		{
			inputs.pop_front();
			if (inputs.size() == 0)
			{
				break;
			}
		}
	}

	Player* player = NULL;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		std::string pl_name(players[i].name);
		if (pl_name == name)
		{
			player = &players[i];
		}
	}
	if (player == NULL)
	{
		return;
	}



	for (auto input : inputs)
	{
		Direction direction = input.second;


		switch (direction)
		{
		case NORTH:
			if (tile_map[player->x][player->y - 1].walkable)
			{
				player->y -= 1;
			}
			else if (tile_map[player->x][player->y - 1].type == DOOR)
			{
				//tile_map[player->x][player->y - 1].walkable = true;//Maybe remove?
			}
			break;
		case WEST:
			if (tile_map[player->x - 1][player->y].walkable)
			{
				player->x -= 1;
			}
			else if (tile_map[player->x - 1][player->y].type == DOOR)
			{
				//tile_map[player->x - 1][player->y].walkable = true;
			}
			break;
		case SOUTH:
			if (tile_map[player->x][player->y + 1].walkable)
			{
				player->y += 1;
			}
			else if (tile_map[player->x][player->y + 1].type == DOOR)
			{
				//tile_map[player->x][player->y + 1].walkable = true;
			}
			break;
		case EAST:
			if (tile_map[player->x + 1][player->y].walkable)
			{
				player->x += 1;
			}
			else if (tile_map[player->x + 1][player->y].type == DOOR)
			{
				//tile_map[player->x + 1][player->y].walkable = true;
			}
			break;

			break;
		default:
			break;
		}
	}
}

void HandleInput(ENetPeer * peer, std::string name)
{
	if (!typing)
	{
		//char * msg;
		std::string str;

		char input = wgetch(win_input);
		if (input != ERR)
		{
		}
		switch (input)
		{
		case KEY_RESIZE:
			resize_term(0, 0);
			UpdateWindowSizes();
			break;

		case 'w':
			SendMessageToPeer(peer, &MovementP(name, NORTH), client_packet_sequence);
			MovementPrediction(NORTH);
			break;

		case 'a':
			SendMessageToPeer(peer, &MovementP(name, WEST), client_packet_sequence);
			MovementPrediction(WEST);
			break;

		case 's':
			SendMessageToPeer(peer, &MovementP(name, SOUTH), client_packet_sequence);
			MovementPrediction(SOUTH);
			break;

		case 'd':
			SendMessageToPeer(peer, &MovementP(name, EAST), client_packet_sequence);
			MovementPrediction(EAST);
			break;

		case 'l':
			nodelay(win_input, false);
			input = wgetch(win_input);
			nodelay(win_input, true);
			switch (input)
			{
			case 'w':
				SendMessageToPeer(peer, &LookP(name, NORTH), client_packet_sequence);
				break;

			case 'a':
				SendMessageToPeer(peer, &LookP(name, WEST), client_packet_sequence);
				break;

			case 's':
				SendMessageToPeer(peer, &LookP(name, SOUTH), client_packet_sequence);
				break;

			case 'd':
				SendMessageToPeer(peer, &LookP(name, EAST), client_packet_sequence);
				break;

			case 'l':
				SendMessageToPeer(peer, &LookP(name, BELOW), client_packet_sequence);
				break;
			}
			break;

		case 't':
			typing = true;

			buffer = new char[1000];
			index = 0;
			wprintw(win_input, "MSG:");

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
			SendMessageToPeer(peer, &MessageP(name, buffer), client_packet_sequence);
			client_packet_sequence++;
			typing = false;
			wprintw(win_input, "\n");
			wprintw(win_chat, "%s: %s\n", name.c_str(), buffer);
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
	//tile_map = new Tile[MAP_SIZE_X*MAP_SIZE_Y];
	/*tile_map = new Tile*[MAP_SIZE_X];
	for (int i = 0; i < MAP_SIZE_X; i++)
	{
		tile_map[i] = new Tile[MAP_SIZE_Y];
	}*/
	/*Packet* pack;
	MessagePacket* msg_pack;
	LookPacket* look_pack;
	MovePacket* move_pack;
	JoinPacket* join_pack;
	MapPacket* map_pack;
	PlayersPacket* players_pack;*/

	Packet* pack = new Packet;
	MessagePacket* msg_pack = new MessagePacket;
	LookPacket* look_pack = new LookPacket;
	MovePacket* move_pack = new MovePacket;
	JoinPacket* join_pack = new JoinPacket;
	MapPacket* map_pack = new MapPacket;
	Tile* tiles;
	PlayersPacket* players_pack = new PlayersPacket; 

	ENetEvent event;
	nodelay(win_input, true);
	wprintw(win_input, "Username:");
	char name[20];
	echo();
	wgetstr(win_input, name);
	noecho();

	SendMessageToPeer(peer, &JoinP(name), client_packet_sequence);
	client_packet_sequence++;

	while (running)
	{
		HandleInput(peer, name);
		//wprintw(win_system, "-W-");
		int pos_x = 0;
		int pos_y = 0;
		int offset_x = 0;
		int offset_y = 0;
		for each (auto player in players)
		{
			if (strncmp(player.name, name, 20) == 0)
			{
				pos_x = player.x; 
				pos_y = player.y;
			}
		}
		if (pos_x > MAP_WIN_SIZE_X / 2)
		{
			offset_x = pos_x - MAP_WIN_SIZE_X / 2;
		}
		if (offset_x > MAP_SIZE_X - MAP_WIN_SIZE_X)
		{
			offset_x = MAP_SIZE_X - MAP_WIN_SIZE_X;
		}

		if (pos_y > MAP_WIN_SIZE_Y / 2)
		{
			offset_y = pos_y - MAP_WIN_SIZE_Y / 2;
		}
		if (offset_y > MAP_SIZE_Y - MAP_WIN_SIZE_Y)
		{
			offset_y = MAP_SIZE_Y - MAP_WIN_SIZE_Y;
		}


		PrintMap(tile_map, offset_x, offset_y);
		PrintPlayers(players, offset_x, offset_y, name);

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

				server_packet_sequence = ((Packet*)(event.packet->data))->sequence;
				//wprintw(win_system, "[%u]", server_packet_sequence);


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
					tiles = (Tile*)(((char*)map_pack) + sizeof(MapPacket));
					for (int x = 0; x < MAP_SIZE_X; x++)
					{
						for (int y = 0; y < MAP_SIZE_Y; y++)
						{
							tile_map[x][y] = tiles[x + y*MAP_SIZE_X];
						}
					}
					break;

				case PLAYERS:
					players_pack = (PlayersPacket*)event.packet->data;
					for (int i = 0; i < MAX_PLAYERS; i++)
					{
						players[i] = players_pack->players[i];
					}
					PlayerReconciliation(name);
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