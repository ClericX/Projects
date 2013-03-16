#include "PacketParser.h"
#include "Main.h"

void PacketParser(const char *Packet)
{
	switch (Packet[0])
	{
	// Movement packet.
	case PacketHeaders.MovementPacket:
		{
			MOVEMENTPACKET *mp = (MOVEMENTPACKET*)Packet;

			if (mp->ID != Player.ID)
			{
				CPlayer *ply = Textures.GetPlayerByID(mp->ID);

				if (ply == 0) //Character has not been loaded.
				{
					ply = new CPlayer();
					ply->ID = mp->ID;
					ply->CurrentMapID = mp->MapID;
					ply->SetLT( Player.LT );
					OtherPlayers.push_back( ply );
				}
				else if (ply != 0 && ply->CurrentMapID == Player.CurrentMapID)
				{
					ply->X = mp->XPos;
					ply->Y = mp->YPos;
				}
				else if (ply->CurrentMapID != Player.CurrentMapID)
				{
					OtherPlayers.remove( ply );
					delete ply;
				}
			}
		}
		break;

	// Chat packet
	case PacketHeaders.ChatPacket:
		{
			CHATPACKET *cp = (CHATPACKET*)Packet;
			cout << cp->ID << " :: " << cp->Text << endl;
		}
		break;

	// Login packet
	case PacketHeaders.LoginPacket:
		{
			LOGINPACKET *lp = (LOGINPACKET*)Packet;
			if (lp->UserID != 0)
			{
				Player.ID = lp->UserID;
				//LoginSuccess = 1;
			}
			else
			{
				MessageBox(NULL, "Login failed.", "Epic fail has occurred.", MB_OK | MB_ICONEXCLAMATION);
				//LoginSuccess = -1;
			}
		}
		break;

	default:
		{
			printf("WARNING: Recieved packet structure not recognized. Header: %s \n", (int)((char)Packet[0]));
		}
		break;
	}
}