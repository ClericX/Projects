#pragma once

#include "Includes.h"

static struct _PacketHeaders
{
	static const int MovementPacket = 1;
	static const int ChatPacket = 2;
	static const int LoginPacket = 3;
} PacketHeaders;

struct MOVEMENTPACKET
{
	BYTE Header;
	unsigned long ID;
	unsigned long MapID;
	long XPos;
	long YPos;

	MOVEMENTPACKET()
	{
		Header = PacketHeaders.MovementPacket;
	}
	~MOVEMENTPACKET()
	{
	}
};

struct CHATPACKET
{
	BYTE Header;
	unsigned long ID;
	unsigned short Length;
	char Text[256];

	CHATPACKET()
	{
		Header = PacketHeaders.ChatPacket;
	}
	~CHATPACKET()
	{
	}
};

struct LOGINPACKET
{
	BYTE Header;
	unsigned long UserID;
	char Username[30];
	char Password[30];

	LOGINPACKET()
	{
		Header = PacketHeaders.LoginPacket;
	}
	~LOGINPACKET()
	{
	}
};

int SendMovementPacket(SOCKET TargetSocket, unsigned long ID, unsigned long MapID, long XPos, long YPos);
int SendChatPacket(SOCKET TargetSocket, unsigned long ID, string Text);