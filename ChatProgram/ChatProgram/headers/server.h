﻿#pragma once
#include <enet/enet.h>
#include <vector>
#include <map>
#include <time.h>

#include "game_structs.h"
#include "file_reader.h"
#include "delayfunc.h"

#define LAG_ENABLED 0
#define LAG_AMOUNT 0.040f

#define SERVER_SEND_RATE 0.05f

int spawnpoint_x = 1;
int spawnpoint_y = 1;


std::vector<std::pair<unsigned int, unsigned int>> animal_spawnpoints;
int animal_spawnpoint_x = 1;
int animal_spawnpoint_y = 1;

float deltatime = 0;

int num_of_connected_clients = 0;
std::map<ENetPeer*, unsigned int> connected_peers;

float send_players_cooldown = 0.0f;

void SpawnAnimals()
{
	int j = 0;
	for (int i = 0; i < MAX_ANIMALS; i++)
	{
		j = rand() % animal_spawnpoints.size();
		animals[i] = Animal(animal_spawnpoints[j].first, animal_spawnpoints[j].second);
		//animals.push_back(Animal(animal_spawnpoint_x, animal_spawnpoint_y));
	}
}

void UpdateAnimals(float dt)
{
	for (auto& animal : animals)
	{
		animal.cooldown -= dt;
		if (animal.cooldown <= 0)
		{
			int rand_dir = rand() % 4;
			int dx = 0;
			int dy = 0;
			switch (rand_dir)
			{
			case 0:
				dx = 1;
				break;
			case 1:
				dx = -1;
				break;
			case 2:
				dy = 1;
				break;
			case 3:
				dy = -1;
				break;
			default:
				break;
			}
			if (tile_map[animal.x + dx][animal.y + dy].walkable)
			{
				animal.x += dx;
				animal.y += dy;
				animal.cooldown = rand()%4 + 1 * 0.25f;
			}
		}
	}
}

void CreateMap()
{

	std::vector<std::vector<char>> map_v = LoadMap("../Content/map/map.txt");

	for (int y = 0; y < MAP_SIZE_Y; y++)
	{
		if (map_v.size() <= y)
		{
			break;
		}
		for (int x = 0; x < MAP_SIZE_X; x++)
		{
			if (map_v[y].size() <= x)
			{
			//	map[x][y] = '\n';
				break;
			}

			//map[x][y] = map_v[y][x];

			switch (map_v[y][x])
			{
			case '0':
			case 'G':
				tile_map[x][y].type = GRASS;
				tile_map[x][y].walkable = true;
				break;
			case '1':
			case 'W':
				tile_map[x][y].type = WALL;
				tile_map[x][y].walkable = false;
				break;
			case '2':
				tile_map[x][y].type = GROUND;
				tile_map[x][y].walkable = true;
				break;
			case '3':
			case 'S':
				tile_map[x][y].type = SAND;
				tile_map[x][y].walkable = true;
				break;
				break;
			case '4':
			case 'B':
				tile_map[x][y].type = WATER;
				tile_map[x][y].walkable = false;
				break;
				break;
			case '5':
			case 'F':
				tile_map[x][y].type = FLOOR;
				tile_map[x][y].walkable = true;
				break;
			case '6':
			case 'P':
				tile_map[x][y].type = SPAWN;
				tile_map[x][y].walkable = true;
				spawnpoint_x = x;
				spawnpoint_y = y;
				break;
			case '7':
			case 'D':
				tile_map[x][y].type = DOOR;
				tile_map[x][y].walkable = false;
				break;
			case '8':
			case 'A':
				tile_map[x][y].type = ANIMALSPAWN;
				tile_map[x][y].walkable = true;
				animal_spawnpoints.push_back({ x,y });
				animal_spawnpoint_x = x;
				animal_spawnpoint_y = y;
				break;
			case '9':
			case 'X':
				tile_map[x][y].type = HOLE;
				tile_map[x][y].walkable = false;
			default:
				break;
			}
		}
	}

	SpawnAnimals();
}

void UpdateTile(int x, int y, Tile tile)
{
	tile_map[x][y] = tile;
	for each (auto c_peer in connected_peers)
	{
		if (num_of_players != 0)
		{
			TileUpdatePacket* tile_pack = new TileUpdatePacket();
			*tile_pack = TileP("", x, y, tile_map[x][y]);

#if LAG_ENABLED
			std::function<void()> func = [c_peer, tile_pack]() { SendMessageToPeer(c_peer.first, tile_pack, c_peer.second); };
			DelayedFunction(func, LAG_AMOUNT);
#else
			SendMessageToPeer(c_peer.first, tile_pack, c_peer.second);
#endif
		}
	}
}

void OpenDoor(int x, int y)
{
	Tile tile = tile_map[x][y];
	tile.walkable = true;

	UpdateTile(x, y, tile);
}

std::string HandleMovement(MovePacket* packet)
{
	Player* player = NULL;
	std::string pa_name(packet->sender);
	std::string message = "";
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		std::string pl_name(players[i].name);
		if (pl_name == pa_name)
		{
			player = &players[i];
		}
	}
	if (player == NULL)
	{
		return "";
	}

	switch (packet->direction)
	{	
	case NORTH:
		if (tile_map[player->x][player->y - 1].walkable)
		{
			player->y -= 1;
		}
		else if (tile_map[player->x][player->y - 1].type == DOOR)
		{
			OpenDoor(player->x, player->y - 1);
			message = "You opened a door.";
		}
		break;
	case WEST:
		if (tile_map[player->x - 1][player->y].walkable)
		{
			player->x -= 1;
		}
		else if (tile_map[player->x - 1][player->y].type == DOOR)
		{
			OpenDoor(player->x - 1, player->y);
			message = "You opened a door.";
		}
		break;
	case SOUTH:
		if (tile_map[player->x][player->y + 1].walkable)
		{
			player->y += 1;
		}
		else if (tile_map[player->x][player->y + 1].type == DOOR)
		{
			OpenDoor(player->x, player->y + 1);
			message = "You opened a door.";
		}
		break;
	case EAST:
		if (tile_map[player->x + 1][player->y].walkable)
		{
			player->x += 1;
		}
		else if (tile_map[player->x + 1][player->y].type == DOOR)
		{
			OpenDoor(player->x + 1, player->y);
			message = "You opened a door.";
		}
		break;
		
		break;
	default:
		break;
	}
	return message;
}

std::string HandleLook(LookPacket* packet)
{
	Player* player = NULL;
	std::string pa_name(packet->sender);
	std::string message = "";
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		std::string pl_name(players[i].name);
		if (pl_name == pa_name)
		{
			player = &players[i];
		}
	}
	if (player == NULL)
	{
		return "";
	}

	tiletype tile = GROUND;

	switch (packet->direction)
	{
	case NORTH:
		tile = tile_map[player->x][player->y - 1].type;			
		break;

	case WEST:
		tile = tile_map[player->x - 1][player->y].type;
		break;

	case SOUTH:
		tile = tile_map[player->x][player->y + 1].type;
		break;

	case EAST:
		tile = tile_map[player->x + 1][player->y].type;
		break;

	case BELOW:
		tile = tile_map[player->x][player->y].type;
		break;

	default:
		break;
	}

	switch (tile)
	{
	case GRASS:
		message = "Green grass.";
		break;

	case SAND:
		message = "Sand.";
		break;

	case WALL:
		message = "Wall.";
		break;

	case WATER:
		message = "Water.";
		break;

	case GROUND:
		message = "Ground.";
		break;

	default:
		message = "You don't know what that is.";
		break;

	}

	return message;
}

void FloodTiles(int x, int y, tiletype type = HOLE)
{
	Tile tile = tile_map[x][y];
	if (tile.type == type)
	{
		tile.type = WATER;
		tile.walkable = false;
		UpdateTile(x, y, tile);
	}

	int x1 = x - 1;
	int x2 = x + 1;
	int y1 = y - 1;
	int y2 = y + 1;

	if (x1 >= 0)
	{
		if (tile_map[x1][y].type == type)
		{
			FloodTiles(x1, y);
		}
	}
	if (x2 < MAP_SIZE_X)
	{
		if (tile_map[x2][y].type == type)
		{
			FloodTiles(x2, y);
		}
	}
	if (y1 >= 0)
	{
		if (tile_map[x][y1].type == type)
		{
			FloodTiles(x, y1);
		}
	}
	if (y2 < MAP_SIZE_Y)
	{
		if (tile_map[x][y2].type == type)
		{
			FloodTiles(x, y2);
		}
	}
}

bool TileAroundTile(int x, int y, tiletype type)
{
	int x1 = x - 1;
	int x2 = x + 1;
	int y1 = y - 1;
	int y2 = y + 1;

	if (x1 >= 0)
	{
		if (tile_map[x1][y].type == type)
		{
			return true;
		}
	}
	if (x2 < MAP_SIZE_X)
	{
		if (tile_map[x2][y].type == type)
		{
			return true;
		}
	}
	if (y1 >= 0)
	{
		if (tile_map[x][y1].type == type)
		{
			return true;
		}
	}
	if (y2 < MAP_SIZE_Y)
	{
		if (tile_map[x][y2].type == type)
		{
			return true;
		}
	}
	return false;
}

std::string HandleDig(DigPacket* packet)
{
	Player* player = NULL;
	std::string pa_name(packet->sender);
	std::string message = "";
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		std::string pl_name(players[i].name);
		if (pl_name == pa_name)
		{
			player = &players[i];
		}
	}
	if (player == NULL)
	{
		return "";
	}

	int x;
	int y;

	switch (packet->direction)
	{
	case NORTH:
		x = player->x;
		y = player->y - 1;
		break;

	case WEST:
		x = player->x - 1;
		y = player->y;
		break;

	case SOUTH:
		x = player->x;
		y = player->y + 1;
		break;

	case EAST:
		x = player->x + 1;
		y = player->y;
		break;

	default:
		break;
	}

	for (auto& animal : animals)
	{
		if (animal.x == x && animal.y == y)
		{
			animal.Respawn(animal_spawnpoints);
			player->score++;
		}
	}

	Tile tile = tile_map[x][y];
	switch (tile.type)
	{

	case GRASS:
	{
		tile.type = HOLE;
		tile.walkable = false;
		UpdateTile(x, y, tile);

		if (TileAroundTile(x, y, WATER))
		{
			FloodTiles(x, y);
		}

		player->dirt_dug++;
		message = "You dug a hole!";
		break;
	}
	case HOLE:
	{
		if (player->dirt_dug > 0)
		{
			player->dirt_dug--;
			tile.type = GRASS;
			tile.walkable = true;
			UpdateTile(x, y, tile);
			message = "You filled a hole!";
		}
		break;
	}

	case WATER:
	{
		if (player->dirt_dug > 0)
		{
			player->dirt_dug--;
			tile.type = GRASS;
			tile.walkable = true;
			UpdateTile(x, y, tile);
			message = "You filled a hole!";
		}
		break;
	}

	default:
		break;
	}
	if (tile.type == GRASS)
	{
	}

	return message;
}

ENetHost * CreateServer()
{
	ENetAddress address;
	ENetHost * server;
	/* Bind the server to the default localhost.     */
	/* A specific host address can be specified by   */
	/* enet_address_set_host (& address, "x.x.x.x"); */
	address.host = ENET_HOST_ANY;
	/* Bind the server to port 8888. */
	address.port = 8888;
	server = enet_host_create(&address /* the address to bind the server host to */,
		32      /* allow up to 32 clients and/or outgoing connections */,
		2      /* allow up to 2 channels to be used, 0 and 1 */,
		0      /* assume any amount of incoming bandwidth */,
		0      /* assume any amount of outgoing bandwidth */);
	if (server == NULL)
	{
		wprintw(win_system, "An error occurred while trying to create an ENet server host.");
		exit(EXIT_FAILURE);
	}


	CreateMap();


	//DO THINGS HERE
	return server;
	//CreateClient();


	//enet_host_destroy(server);
}

void DisconnectPeer(ENetPeer* peer, ENetHost* client)
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
			wprintw(win_system, "Disconnection succeeded.");
			return;


			//DO THINGS HERE


		}
	}
	/* We've arrived here, so the disconnect attempt didn't */
	/* succeed yet.  Force the connection down.             */
	enet_peer_reset(peer);


	//DO THINGS HERE
}


/*Packet* pack = new Packet;
MessagePacket* msg_pack = new MessagePacket;
LookPacket* look_pack = new LookPacket;
MovePacket* move_pack = new MovePacket;
JoinPacket* join_pack = new JoinPacket;
MapPacket* map_pack = new MapPacket;
PlayersPacket* players_pack = new PlayersPacket;*/

void HandleEvent(ENetEvent event, ENetHost* server)
{

	std::string name = "Client:" + std::to_string(num_of_connected_clients);
	std::string message = "";
	ENetPeer* peer = event.peer;
	unsigned int sequence = 0;

	switch (event.type)
	{
	case ENET_EVENT_TYPE_CONNECT:
		wprintw(win_system, "A new client connected from %x:%u.",
			event.peer->address.host,
			event.peer->address.port);
		/* Store any relevant client information here. */
		event.peer->data = (void*)name.c_str();
		connected_peers.insert({ event.peer,0 });
		num_of_connected_clients++;
		break;
	case ENET_EVENT_TYPE_RECEIVE:
	{

		Packet* pack = (Packet*)event.packet->data;

		wprintw(win_system, "A packet[%u] of length %u was received from %s on channel %u.\n",
			pack->sequence,
			(unsigned int)event.packet->dataLength,
			pack->sender,
			event.channelID);

		sequence = pack->sequence;

		for (auto&& c_peer : connected_peers)
		{
			if (c_peer.first == event.peer)
			{
				c_peer.second = sequence;
			}
		}

		switch (((Packet*)(event.packet->data))->type)
		{
			
		case CHAT:
		{
			MessagePacket* msg_pack = new MessagePacket();
			*msg_pack = *(MessagePacket*)event.packet->data;

			for each (auto c_peer in connected_peers)
			{
				if (c_peer.first == event.peer)
				{
					continue;
				}

#if LAG_ENABLED
				std::function<void()> func = [c_peer, msg_pack]() { SendMessageToPeer(c_peer.first, msg_pack, c_peer.second); };
				DelayedFunction(func, LAG_AMOUNT);
#else
				SendMessageToPeer(c_peer.first, msg_pack, c_peer.second);
#endif

			}

			wprintw(win_chat, "%s: %s\n", msg_pack->sender, msg_pack->message);

			wprintw(win_system, "[%s]", msg_pack->message);
			break;
		}

		case MOVEMENT:
		{
			MovePacket* move_pack = (MovePacket*)event.packet->data;
			message = HandleMovement(move_pack);
			if (message != "")
			{
#if LAG_ENABLED
				std::function<void()> func = [peer, message, sequence]() { SendMessageToPeer(peer, &MessageP("[Movement]", message), sequence); };
				DelayedFunction(func, LAG_AMOUNT);
#else
				SendMessageToPeer(peer, &MessageP("[Movement]", message), sequence);
#endif

			}
			wprintw(win_system, "[%d]", move_pack->direction);
			break;
		}

		case LOOK:
		{
			LookPacket* look_pack = (LookPacket*)event.packet->data;
			message = HandleLook(look_pack);
			if (message != "")
			{
#if LAG_ENABLED
				std::function<void()> func = [peer, message, sequence]() { SendMessageToPeer(peer, &MessageP("[Look]", message), sequence); };
				DelayedFunction(func, LAG_AMOUNT);
#else
				SendMessageToPeer(peer, &MessageP("[Look]", message), sequence);
#endif
			}
			wprintw(win_system, "[%d]", look_pack->direction);
			break;
		}

		case COMMAND:
		{
			break;
		}

		case JOIN:
		{
			JoinPacket* join_pack = (JoinPacket*)event.packet->data;
			players[num_of_players] = (Player(join_pack->sender, spawnpoint_x, spawnpoint_y));
			num_of_players++;
			MapPacket* map_pack = new MapPacket();
			*map_pack = MapP("", tile_map);
#if LAG_ENABLED
				std::function<void()> func = [peer, map_pack, sequence]() { SendMessageToPeer(peer, map_pack, sequence); };
				DelayedFunction(func, LAG_AMOUNT);
#else
			SendMessageToPeer(peer, map_pack, sequence);
#endif
			break;
		}

		case DIG:
		{
			DigPacket* look_pack = (DigPacket*)event.packet->data;
			message = HandleDig(look_pack);
			if (message != "")
			{
#if LAG_ENABLED
				std::function<void()> func = [peer, message, sequence]() { SendMessageToPeer(peer, &MessageP("[Look]", message), sequence); };
				DelayedFunction(func, LAG_AMOUNT);
#else
				SendMessageToPeer(peer, &MessageP("[Look]", message), sequence);
#endif
			}
			wprintw(win_system, "[%d]", look_pack->direction);
			break;
		}

		default:
		{
			wprintw(win_system, "Packet of unknown type received.\n");
			break;
		}

		}


		/* Clean up the packet now that we're done using it. */
		enet_packet_destroy(event.packet);

		break;
	}

	case ENET_EVENT_TYPE_DISCONNECT:
	{
		wprintw(win_system, "%s disconnected.", (char*)event.peer->data);
		/* Reset the peer's client information. */


		event.peer->data = NULL;
		connected_peers.erase(event.peer);
		break;
	}

	}
}



void ServerThread(int id, ENetHost* server, bool* running)
{
	//nodelay(win_input, true);
	//halfdelay(1);
	//std::vector<ENetPeer*> connected_peers;


	ENetEvent event;

	char c1 = (char)176;

	char c2 = (char)177;

	char c3 = (char)178;

	char c4 = (char)219;

	wprintw(win_system, "%c(%i),%c(%i),%c(%i),%c(%i)\n", c1, c1, c2, c2, c3, c3, c4, c4);


	clock_t current_time = clock();
	clock_t previous_time = current_time;

	while (running)
	{
		previous_time = current_time;
		current_time = clock();
		deltatime = (float)(current_time - previous_time) / CLOCKS_PER_SEC;
		//wprintw(win_system, "Deltatime:%f\n", deltatime);

		UpdateAnimals(deltatime);

#if LAG_ENABLED
		DelayedFunctionUpdate(deltatime);
#endif // LAG_ENABLED


		PrintMap(tile_map, 0, 0);
		PrintPlayers(players, 0, 0);
		PrintAnimals(animals, 0, 0);
		UpdateWindows();

		if (send_players_cooldown < 0.0f)
		{
			send_players_cooldown = SERVER_SEND_RATE;
			for each (auto c_peer in connected_peers)
			{
				if (num_of_players != 0)
				{
					PlayersPacket* players_pack = new PlayersPacket();
					*players_pack = PlayersP("", players);

					AnimalsPacket* animals_pack = new AnimalsPacket();
					*animals_pack = AnimalsP("", animals);

#if LAG_ENABLED
					std::function<void()> func = [c_peer, players_pack]() { SendMessageToPeer(c_peer.first, players_pack, c_peer.second); };
					DelayedFunction(func, LAG_AMOUNT); 
					std::function<void()> func2 = [c_peer, animals_pack]() { SendMessageToPeer(c_peer.first, animals_pack, c_peer.second); };
					DelayedFunction(func2, LAG_AMOUNT);
#else
					SendMessageToPeer(c_peer.first, players_pack, c_peer.second);
					SendMessageToPeer(c_peer.first, animals_pack, c_peer.second);
#endif
				}
			}
		}
		else
		{
			send_players_cooldown -= deltatime;
		}
		int enet_int = enet_host_service(server, &event, 0);
		
		/* Wait up to 0 milliseconds for an event. */
		if (enet_int > 0)
		{

#if LAG_ENABLED
			std::function<void()> func = [event, server]() { HandleEvent(event, server); };
			DelayedFunction(func, LAG_AMOUNT);
#else
			HandleEvent(event, server);
#endif

		}
		Sleep(1);
	}
}