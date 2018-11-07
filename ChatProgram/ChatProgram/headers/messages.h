#pragma once

enum MessageType
{
	MOVEMENT,
	LOOK,
	CHAT,
	COMMAND,
	JOIN,
	MAP,
	PLAYERS
};

enum Direction
{
	NORTH,
	EAST,
	WEST,
	SOUTH,
	BELOW,
	ABOVE
};

struct Packet
{
	MessageType type;
	char sender[20];
};

struct MessagePacket :Packet
{
	char message[1003];
};

struct MovePacket :Packet
{
	Direction dir;
};

struct LookPacket :Packet
{
	Direction dir;
};

struct JoinPacket :Packet
{
}; 

struct CommandPacket :Packet
{
	char message[100];
};

struct MapPacket :Packet
{
	MapPacket()
	{
		map = new Tile[MAP_SIZE_X*MAP_SIZE_Y];
		//map = std::vector<std::vector<Tile>>(MAP_SIZE_X, std::vector<Tile>(MAP_SIZE_Y, Tile()));
	}
	//std::vector<std::vector<Tile>> map;
	//Tile map[MAP_SIZE_X][MAP_SIZE_Y];
	Tile* map;
};

struct PlayersPacket :Packet
{
	Player players[MAX_PLAYERS];
};

JoinPacket JoinP(std::string name)
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	JoinPacket pack;
	pack.type = JOIN;

	memcpy(pack.sender, name.c_str(), 20);
	return pack;
}

MovePacket MovementP(std::string name, Direction direction)
{

	if (name.size() > 19)
	{
		name.resize(19);
	}
	MovePacket pack;
	pack.type = MOVEMENT;

	memcpy(pack.sender, name.c_str(), 20);
	pack.dir = direction;
	return pack;
}

LookPacket LookP(std::string name, Direction direction)
{

	if (name.size() > 19)
	{
		name.resize(19);
	}
	LookPacket pack;
	pack.type = LOOK;

	memcpy(pack.sender, name.c_str(), 20);
	pack.dir = direction;
	return pack;
}

MessagePacket MessageP(std::string name, std::string message)
{
	if (message.size() > 1003)
		message.resize(1003);
	if (name.size() > 19)
	{
		name.resize(19);
	}
	MessagePacket pack;
	pack.type = CHAT;
	
	memcpy(pack.sender, name.c_str(), 20);
	memcpy(pack.message, message.c_str(), 1004);
	return pack;
}

MapPacket MapP(std::string name, std::vector<std::vector<Tile>> map)
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	MapPacket pack;
	pack.type = MAP;

	

	memcpy(pack.sender, name.c_str(), 20);
	for (int x = 0; x < MAP_SIZE_X; x++)
	{
		for (int y = 0; y < MAP_SIZE_Y; y++)
		{
			pack.map[x + y*MAP_SIZE_X] = map[x][y];
		}
	}
	//pack.map = map;
	return pack;
}

MapPacket MapP(std::string name, Tile** map)
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	MapPacket pack;
	pack.type = MAP;

	memcpy(pack.sender, name.c_str(), 20);
	for (int x = 0; x < MAP_SIZE_X; x++)
	{
		for (int y = 0; y < MAP_SIZE_Y; y++)
		{
			pack.map[x + y*MAP_SIZE_X] = map[x][y];
			//pack.map[x][y] = map[x][y];
		}
	}
	//pack.map = map;
	return pack;
}

/*MapPacket MapP(std::string name, Tile map[MAP_SIZE_X][MAP_SIZE_Y] )
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	MapPacket pack;
	pack.type = MAP;

	memcpy(pack.sender, name.c_str(), 20);
	for (int x = 0; x < MAP_SIZE_X; x++)
	{
		for (int y = 0; y < MAP_SIZE_Y; y++)
		{
			pack.map[x][y] = map[x][y];
		}
	}
	//pack.map = map;
	return pack;
}*/

PlayersPacket PlayersP(std::string name, Player players[MAX_PLAYERS])
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	PlayersPacket pack;
	pack.type = PLAYERS;

	memcpy(pack.sender, name.c_str(), 20);
	for (int i = 0; i < MAX_PLAYERS; i++)
	{
		pack.players[i] = players[i];
	}
	//pack.map = map;
	return pack;
}

void SendMessageToPeer(ENetPeer* peer, Packet* package)
{
	if (package->type == CHAT)
	{
		char* message[sizeof(MessagePacket)];
		memcpy(message, package, sizeof(MessagePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else if (package->type == MOVEMENT)
	{
		char* message[sizeof(MovePacket)];
		memcpy(message, package, sizeof(MovePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else if (package->type == LOOK)
	{
		char* message[sizeof(LookPacket)];
		memcpy(message, package, sizeof(LookPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else if (package->type == JOIN)
	{
		char* message[sizeof(JoinPacket)];
		memcpy(message, package, sizeof(JoinPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else if (package->type == MAP)
	{
		char* message = new char[sizeof(MapPacket)+sizeof(Tile)*MAP_SIZE_X*MAP_SIZE_Y];
		memcpy(message, package, sizeof(MapPacket));
		memcpy(message + sizeof(MapPacket), ((MapPacket*)package)->map, sizeof(Tile)*MAP_SIZE_X*MAP_SIZE_Y);

		ENetPacket * packet = enet_packet_create(message,
			sizeof(MapPacket) + sizeof(Tile)*MAP_SIZE_X*MAP_SIZE_Y,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);

		delete[] message;
	}
	else if (package->type == PLAYERS)
	{
		char* message[sizeof(PlayersPacket)];
		memcpy(message, package, sizeof(PlayersPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
}


//void _SendPackageToPeer(ENetPeer* peer, std::string message = "Test Package")
//{
	/* Create a reliable packet of size 7 containing "packet\0" */
	//ENetPacket * packet = enet_packet_create("packet",
	//	strlen("packet") + 1,
	//	ENET_PACKET_FLAG_RELIABLE);
	/* Extend the packet so and append the string "foo", so it now */
	/* contains "packetfoo\0"                                      */
	//enet_packet_resize(packet, strlen("packetfoo") + 1);
	//strcpy(&packet->data[strlen("packet")], "foo");
	/* Send the packet to the peer over channel id 0. */
	/* One could also broadcast the packet by         */
	/* enet_host_broadcast (host, 0, packet);         */

	///ENetPacket * packet = enet_packet_create(message.c_str(),
	///	sizeof(message),
	///	ENET_PACKET_FLAG_RELIABLE);
	///enet_peer_send(peer, 0, packet);


	//DO THINGS HERE
	//DisconnectPeer(peer);

	/* One could just use enet_host_service() instead. */
	//enet_host_flush(host);
//}