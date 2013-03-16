#pragma once

#include <Windows.h>
#include "../Frost/String.h"
#include "../Frost/List.h"

void PacketParser(SOCKET s, String *buffer);
void BroadcastPacket(String thePacket);
void SendPacket(SOCKET& s, String thePacket);

// MOVE THIS INTO A GENERAL NETWORK CLASS LATER
struct SocketStruct
{
	unsigned int ID;
	SOCKET Socket;
};

extern "C" List<SocketStruct *> ServerClients;

unsigned int GetIDFromSocket(SOCKET s);