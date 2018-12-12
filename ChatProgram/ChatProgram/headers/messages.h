#pragma once

enum MessageType
{
	MOVEMENT,
	LOOK,
	CHAT,
	COMMAND,
	JOIN,
	MAP,
	PLAYERS,
	TILE,
	DIG,
	ANIMALS
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
	unsigned int sequence;
};


struct MessagePacket :Packet
{
	char message[1003];
};

struct MovePacket :Packet
{
	Direction direction;
};

struct LookPacket :Packet
{
	Direction direction;
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
	}
	Tile* map;
};

struct PlayersPacket :Packet
{
	Player players[MAX_PLAYERS];
};

struct AnimalsPacket :Packet
{
	Animal animals[MAX_ANIMALS];
};

struct TileUpdatePacket :Packet
{
	int x, y;
	Tile tile;
};

struct DigPacket :Packet
{
	Direction direction;
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
	pack.direction = direction;
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
	pack.direction = direction;
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
		}
	}

	return pack;
}

TileUpdatePacket TileP(std::string name, int x, int y, Tile tile)
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	TileUpdatePacket pack;
	pack.type = TILE;

	memcpy(pack.sender, name.c_str(), 20);

	pack.x = x;
	pack.y = y;
	pack.tile = tile;

	return pack;
}

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
	return pack;
}

AnimalsPacket AnimalsP(std::string name, Animal animals[MAX_ANIMALS])
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	AnimalsPacket pack;
	pack.type = ANIMALS;

	memcpy(pack.sender, name.c_str(), 20);
	for (int i = 0; i < MAX_ANIMALS; i++)
	{
		pack.animals[i] = animals[i];
	}
	return pack;
}

DigPacket DigP(std::string name, Direction direction)
{

	if (name.size() > 19)
	{
		name.resize(19);
	}
	DigPacket pack;
	pack.type = DIG;

	memcpy(pack.sender, name.c_str(), 20);
	pack.direction = direction;
	return pack;
}

void SendMessageToPeer(ENetPeer* peer, Packet* package, unsigned int sequence_number)
{
	package->sequence = sequence_number;

	switch (package->type)
	{
	case CHAT:
	{
		char* message[sizeof(MessagePacket)];
		memcpy(message, package, sizeof(MessagePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}
	case MOVEMENT:
	{
		char* message[sizeof(MovePacket)];
		memcpy(message, package, sizeof(MovePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}
	case LOOK:
	{
		char* message[sizeof(LookPacket)];
		memcpy(message, package, sizeof(LookPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}
	case JOIN:
	{
		char* message[sizeof(JoinPacket)];
		memcpy(message, package, sizeof(JoinPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}
	case MAP:
	{
		char* message = new char[sizeof(MapPacket) + sizeof(Tile)*MAP_SIZE_X*MAP_SIZE_Y];
		memcpy(message, package, sizeof(MapPacket));
		memcpy(message + sizeof(MapPacket), ((MapPacket*)package)->map, sizeof(Tile)*MAP_SIZE_X*MAP_SIZE_Y);

		ENetPacket * packet = enet_packet_create(message,
			sizeof(MapPacket) + sizeof(Tile)*MAP_SIZE_X*MAP_SIZE_Y,
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		delete[] message;
		break;
	}
	case PLAYERS:
	{
		char* message[sizeof(PlayersPacket)];
		memcpy(message, package, sizeof(PlayersPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}
	case ANIMALS:
	{
		char* message[sizeof(AnimalsPacket)];
		memcpy(message, package, sizeof(AnimalsPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}
	case TILE:
	{
		char* message[sizeof(TileUpdatePacket)];
		memcpy(message, package, sizeof(TileUpdatePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}

	case DIG:
	{
		char* message[sizeof(DigPacket)];
		memcpy(message, package, sizeof(DigPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
		break;
	}

	default:
		break;
	}
}