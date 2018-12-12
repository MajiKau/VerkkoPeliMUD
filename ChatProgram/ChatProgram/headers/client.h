#pragma once
#include <enet/enet.h>
#include <list>

#include "game_structs.h"
#include "curses_windows.h"
#include "messages.h"

static unsigned int client_packet_sequence = 0;
static unsigned int server_packet_sequence = 0;

ENetPeer * ConnectToHost(ENetHost* client, const char* server_address = "localhost")
{
	ENetAddress address;
	ENetEvent event;
	ENetPeer *peer;
	/* Connect to server_address:8888. */
	if (server_address != "")
	{
		enet_address_set_host(&address, server_address);
	}
	else
	{
		enet_address_set_host(&address, "localhost");
	}
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
	return client;
}

bool typing = false;
bool looking = false;
bool digging = false;
char* buffer;
int index;

std::list <std::pair<unsigned int, Direction>> inputs;

void MovementPrediction(Direction direction)
{
	inputs.push_back({ client_packet_sequence ,direction });
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
				return;
			}
		}
	}

	Player* player = NULL;
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		std::string pl_name(players[i].name);
		if (pl_name == name)
		{
			players_copy[i] = players[i];
			player = &players_copy[i];
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
				//The next line is commented, because the client doesn't assume it can open doors.
				//tile_map[player->x][player->y - 1].walkable = true;
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

bool HandleInput(ENetPeer * peer, std::string name)
{
	if (typing)
	{
		char input = wgetch(win_input);
		switch (input)
		{
		case 10:
			buffer[index] = '\0';
			index++;
			client_packet_sequence++;
			SendMessageToPeer(peer, &MessageP(name, buffer), client_packet_sequence);
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
	else if(looking)
	{
		char input = wgetch(win_input);
		if (input != ERR)
		{
			looking = false;

			switch (input)
			{
			case 'w':
				client_packet_sequence++;
				SendMessageToPeer(peer, &LookP(name, NORTH), client_packet_sequence);
				wprintw(win_input, "[NORTH]\n");
				break;

			case 'a':
				client_packet_sequence++;
				SendMessageToPeer(peer, &LookP(name, WEST), client_packet_sequence);
				wprintw(win_input, "[WEST]\n");
				break;

			case 's':
				client_packet_sequence++;
				SendMessageToPeer(peer, &LookP(name, SOUTH), client_packet_sequence);
				wprintw(win_input, "[SOUTH]\n");
				break;

			case 'd':
				client_packet_sequence++;
				SendMessageToPeer(peer, &LookP(name, EAST), client_packet_sequence);
				wprintw(win_input, "[EAST]\n");
				break;

			case 'l':
				client_packet_sequence++;
				SendMessageToPeer(peer, &LookP(name, BELOW), client_packet_sequence);
				wprintw(win_input, "[BELOW]\n");
				break;

			default:
				wprintw(win_input, "[CANCEL]\n");
				break;
			}
		}
	}
	else if (digging)
	{
		char input = wgetch(win_input);
		if (input != ERR)
		{
			digging = false;

			switch (input)
			{
			case 'w':
				client_packet_sequence++;
				SendMessageToPeer(peer, &DigP(name, NORTH), client_packet_sequence);
				wprintw(win_input, "[NORTH]\n");
				break;

			case 'a':
				client_packet_sequence++;
				SendMessageToPeer(peer, &DigP(name, WEST), client_packet_sequence);
				wprintw(win_input, "[WEST]\n");
				break;

			case 's':
				client_packet_sequence++;
				SendMessageToPeer(peer, &DigP(name, SOUTH), client_packet_sequence);
				wprintw(win_input, "[SOUTH]\n");
				break;

			case 'd':
				client_packet_sequence++;
				SendMessageToPeer(peer, &DigP(name, EAST), client_packet_sequence);
				wprintw(win_input, "[EAST]\n");
				break;

			default:
				wprintw(win_input, "[CANCEL]\n");
				break;
			}
		}
	}
	else
	{
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
			client_packet_sequence++;
			SendMessageToPeer(peer, &MovementP(name, NORTH), client_packet_sequence);
			MovementPrediction(NORTH);
			break;

		case 'a':
			client_packet_sequence++;
			SendMessageToPeer(peer, &MovementP(name, WEST), client_packet_sequence);
			MovementPrediction(WEST);
			break;

		case 's':
			client_packet_sequence++;
			SendMessageToPeer(peer, &MovementP(name, SOUTH), client_packet_sequence);
			MovementPrediction(SOUTH);
			break;

		case 'd':
			client_packet_sequence++;
			SendMessageToPeer(peer, &MovementP(name, EAST), client_packet_sequence);
			MovementPrediction(EAST);
			break;

		case 'l':
		{
			wprintw(win_input, "[Look]:");
			looking = true;			
			break;
		}

		case 't':
		{
			typing = true;

			buffer = new char[1000];
			index = 0;
			wprintw(win_input, "[MSG]:");
			break;
		}

		case 'k':
		{
			wprintw(win_input, "[Dig]:");
			digging = true;
			break;
		}

		case 'q':
		{
			wprintw(win_system, "\nQuitting.\n");
			return 1;
			break;
		}
			

		default:
			break;

		}
		
	}
	return 0;
}

void DisconnectClient(ENetPeer* peer, ENetHost* client)
{
	ENetEvent event;
	enet_peer_disconnect(peer, 0);
	/* Allow up to 3 seconds for the disconnect to succeed
	* and drop any packets received packets.
	*/
	while (enet_host_service(client, &event, 3000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			wprintw(win_system, "Disconnection succeeded.\n");
			return;
		}
	}
	/* We've arrived here, so the disconnect attempt didn't */
	/* succeed yet.  Force the connection down.             */
	enet_peer_reset(peer);

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

	client_packet_sequence++;
	SendMessageToPeer(peer, &JoinP(name), client_packet_sequence);

	Player player;

	while (running)
	{
		if (HandleInput(peer, name))
		{
			DisconnectClient(peer, client);
			running = false;
		}

		PlayerReconciliation(name);

		//Calculate the correct offset for the map based on the player position
		int pos_x = 0;
		int pos_y = 0;
		int offset_x = 0;
		int offset_y = 0;
		for each (auto player in players_copy)
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
		PrintAnimals(animals, offset_x, offset_y);
		PrintPlayers(players_copy, offset_x, offset_y, name);
		PrintPlayerData(player);

		UpdateWindows();

		//0 = Don't for wait for events
		int enet_int = enet_host_service(client, &event, 0);

		//If an event was received
		if (enet_int > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				wprintw(win_system, "Someone is trying to connect to a client?");
				break;
			case ENET_EVENT_TYPE_RECEIVE:

				server_packet_sequence = ((Packet*)(event.packet->data))->sequence;

				//Prints the latest packet sequence number received from the server and the latest client packet sequence number
				//wprintw(win_system, "[ServerState:%u, ClientState:%u]\n", server_packet_sequence, client_packet_sequence);

				switch (((Packet*)(event.packet->data))->type)
				{
				case CHAT:
				{
					MessagePacket* msg_pack = (MessagePacket*)event.packet->data;

					wprintw(win_chat, "%s: %s\n", msg_pack->sender, msg_pack->message);

					if (msg_pack->message == "/stop")
					{
						wprintw(win_system, "Server shutdown");
					}

					break;
				}
				case MAP: 
				{
					MapPacket* map_pack = (MapPacket*)event.packet->data;
					Tile* tiles = (Tile*)(((char*)map_pack) + sizeof(MapPacket));
					for (int x = 0; x < MAP_SIZE_X; x++)
					{
						for (int y = 0; y < MAP_SIZE_Y; y++)
						{
							tile_map[x][y] = tiles[x + y*MAP_SIZE_X];
						}
					}
					break;
				}
				case PLAYERS:
				{
					PlayersPacket* players_pack = (PlayersPacket*)event.packet->data;
					for (int i = 0; i < MAX_PLAYERS; i++)
					{
						players[i] = players_pack->players[i];
						players_copy[i] = players[i];
						if (strncmp(players[i].name, name, 20) == 0)
						{
							player = players[i];
						}
					}
					break;
				}
				case ANIMALS:
				{
					AnimalsPacket* animals_pack = (AnimalsPacket*)event.packet->data;
					for (int i = 0; i < MAX_ANIMALS; i++)
					{
						animals[i] = animals_pack->animals[i];
					}
					break;
				}
				case TILE:
				{
					TileUpdatePacket* tile_pack = (TileUpdatePacket*)event.packet->data;
					tile_map[tile_pack->x][tile_pack->y] = tile_pack->tile;
				}

				default:
					break;

				}


				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				wprintw(win_system, "Lost connection to server. Exiting.\n");
				running = false;
				UpdateWindows();
				Sleep(500);
				break;
			}

			/* Clean up the packet now that we're done using it. */
			enet_packet_destroy(event.packet);
		}
	}
}