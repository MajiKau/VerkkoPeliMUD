#pragma once

enum message_type
{
	movement,
	chat,
	command,
	join
};

struct Packet
{
	message_type type;
	char sender[20];
};

struct MessagePacket :Packet
{
	char message[1003];
};

struct MovePacket :Packet
{
	char message;
};

struct JoinPacket :Packet
{
}; 

struct CommandPacket :Packet
{
	char message[100];
};
JoinPacket JoinP(std::string name)
{
	if (name.size() > 19)
	{
		name.resize(19);
	}
	JoinPacket pack;
	pack.type = join;

	memcpy(pack.sender, name.c_str(), 20);
	return pack;
}
MovePacket MovementP(std::string name, char message)
{

	if (name.size() > 19)
	{
		name.resize(19);
	}
	MovePacket pack;
	pack.type = movement;

	memcpy(pack.sender, name.c_str(), 20);
	pack.message = message;
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
	pack.type = chat;
	
	memcpy(pack.sender, name.c_str(), 20);
	memcpy(pack.message, message.c_str(), 1004);
	return pack;
}
void SendMessageToPeer(ENetPeer* peer, Packet* package)
{
	if (package->type == chat)
	{
		char* message[sizeof(MessagePacket)];
		memcpy(message, package, sizeof(MessagePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else if (package->type == movement)
	{
		char* message[sizeof(MovePacket)];
		memcpy(message, package, sizeof(MovePacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}
	else if (package->type == join)
	{
		char* message[sizeof(JoinPacket)];
		memcpy(message, package, sizeof(JoinPacket));

		ENetPacket * packet = enet_packet_create(message,
			sizeof(message),
			ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(peer, 0, packet);
	}

}


void _SendPackageToPeer(ENetPeer* peer, std::string message = "Test Package")
{
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

	ENetPacket * packet = enet_packet_create(message.c_str(),
		sizeof(message),
		ENET_PACKET_FLAG_RELIABLE);
	enet_peer_send(peer, 0, packet);


	//DO THINGS HERE
	//DisconnectPeer(peer);

	/* One could just use enet_host_service() instead. */
	//enet_host_flush(host);
}