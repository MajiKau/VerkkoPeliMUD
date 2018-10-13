#pragma once
#include "structs.h"
//#include <enet/enet.h>

char map[80][40];



std::vector<Player> players;

void CreateMap()
{
	for (int x = 0; x < 80; x++)
	{
		for (int y = 0; y < 40; y++)
		{
			char c = ':' + rand() % 4;
			map[x][y] = c;
		}
	}
}
void HandleMovement(MovePacket* packet)
{
	Player* player = NULL;
	std::string pa_name(packet->sender);
	for (int i = 0; i < players.size(); i++)
	{
		std::string pl_name(players[i].name);
		if (pl_name == pa_name)
		{
			player = &players[i];
		}
	}
	if (player == NULL)
	{
		return;
	}
	switch (packet->message)
	{
	case 'w':
		player->y -= 1;
		break;
	case 'a':
		player->x -= 1;
		break;
	case 's':
		player->y += 1;
		break;
	case 'd':
		player->x += 1;
		break;
	default:
		break;
	}
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

void ServerThread(int id, ENetHost* server, bool* running)
{
	ENetEvent event;
	int num_of_connected_clients = 0;
	std::string stop_message = "/stop";
	//std::vector<ENetPeer*> connected_peers;
	std::set<ENetPeer*> connected_peers;

	Packet* pack;
	MessagePacket* msg_pack;
	MovePacket* move_pack;
	JoinPacket* join_pack;

	while (running)
	{

		PrintMap(map);
		PrintPlayers(players);
		UpdateWindows();

		int enet_int = enet_host_service(server, &event, 0);
		std::string name = "Client:" + std::to_string(num_of_connected_clients);
		/* Wait up to 0 milliseconds for an event. */
		if (enet_int > 0)
		{
			switch (event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				wprintw(win_system, "A new client connected from %x:%u.",
					event.peer->address.host,
					event.peer->address.port);
				/* Store any relevant client information here. */
				event.peer->data = (void*)name.c_str();
				connected_peers.insert(event.peer);
				num_of_connected_clients++;
				break;
			case ENET_EVENT_TYPE_RECEIVE:

				switch (((Packet*)(event.packet->data))->type)
				{
				case chat:
					msg_pack = (MessagePacket*)event.packet->data;

					for each (auto peer in connected_peers)
					{
						if (peer == event.peer && *running == true)
							continue;
						SendMessageToPeer(peer, msg_pack);
						if (!running)
						{
							DisconnectPeer(peer, server);
						}
					}

					wprintw(win_system, "[%s]", msg_pack->message);
					break;
				case movement:
					move_pack = (MovePacket*)event.packet->data;
					HandleMovement(move_pack);
					wprintw(win_system, "[%c]", move_pack->message);
					break;
				case command:
					break;
				case join:
					join_pack = (JoinPacket*)event.packet->data;
					players.push_back(Player(join_pack->sender, rand() % 10, rand() % 10));

					break;
				default:
					break;
				}

				pack = (Packet*)event.packet->data;

				wprintw(win_system, "A packet of length %u was received from %s on channel %u.",
					(unsigned int)event.packet->dataLength,
					pack->sender,
					event.channelID);

				/* Clean up the packet now that we're done using it. */
				enet_packet_destroy(event.packet);

				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				wprintw(win_system, "%s disconnected.", (char*)event.peer->data);
				/* Reset the peer's client information. */
				event.peer->data = NULL;
				connected_peers.erase(event.peer);
			}
		}
		//Sleep(10);
	}
}