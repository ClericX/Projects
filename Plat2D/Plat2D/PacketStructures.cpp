#include "PacketStructures.h"

int SendMovementPacket(SOCKET TargetSocket, unsigned long ID, unsigned long MapID, long XPos, long YPos)
{
	MOVEMENTPACKET *mp = new MOVEMENTPACKET();
	mp->ID = ID;
	mp->MapID = MapID;
	mp->XPos = XPos;
	mp->YPos = YPos;
	int SentBytes = send(TargetSocket, (char *)mp, sizeof(MOVEMENTPACKET), 0);
	delete mp;
	return SentBytes;
}

int SendChatPacket(SOCKET TargetSocket, unsigned long ID, string Text)
{
	CHATPACKET *cp = new CHATPACKET();
	cp->ID = ID;
	strcpy(cp->Text, Text.c_str());
	cp->Length = sizeof(CHATPACKET) - 256 + Text.size();
	if (cp->Length > sizeof(CHATPACKET))
	{
		cp->Length = sizeof(CHATPACKET);
	}
	int SentBytes = send(TargetSocket, (char *)cp, cp->Length, 0);
	delete cp;
	return SentBytes;
}