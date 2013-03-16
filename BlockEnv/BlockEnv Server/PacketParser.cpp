#include "PacketParser.h"
#include "../Frost/Log.h"
#include "../Moonlight Frost/PacketStructures.h"
#include "../Frost/String.h"
#include "../FrostGame/Block.h"
#include "../FrostGame/Player.h"

void PacketParser(SOCKET s, String *buffer)
{
	StringList splitBuffer = buffer->split("|");
	char header = (char)splitBuffer[0].toInt();

	switch (header)
	{
	case BlockRelatedPacket::Header:
		{
			BlockRelatedPacket recvBlock = BlockRelatedPacket::fromPacket(&splitBuffer);

			switch (recvBlock.ActionType)
			{
			case BlockRelatedPacket::Create:
				{
					Block::Instance *newBlock = new Block::Instance();
					newBlock->Position = recvBlock.BlockLocation;
					newBlock->Type = Block::GetTypeInfoByID(recvBlock.TypeID);
					Block::Add(newBlock);
				}
				break;

			case BlockRelatedPacket::Update:
				{
					for (unsigned int i = 0; i < Block::WorldBlockList.count(); i++)
					{
						BlockRelatedPacket p;
						p.ActionType = BlockRelatedPacket::Create;
						p.BlockLocation = Block::WorldBlockList[i]->Position;
						p.TypeID = Block::WorldBlockList[i]->Type->ID;
						SendPacket(s, p.toPacket());
					}
				}
				break;

			case BlockRelatedPacket::Remove:
				{
					Block::Remove(recvBlock.BlockLocation);
				}
				break;
			}

			if (recvBlock.ActionType != BlockRelatedPacket::Update)
			{
				BroadcastPacket(recvBlock.toPacket());
			}
		}
		break;

	case PlayerRelatedPacket::Header:
		{
			PlayerRelatedPacket recvPlayer = PlayerRelatedPacket::fromPacket(&splitBuffer);

			recvPlayer.ID = GetIDFromSocket(s);
			Player::Instance *thePlayer = Player::Get(recvPlayer.ID);

			if (thePlayer == 0)
			{
				// Create the player in memory and add it to the player stack.
				thePlayer = new Player::Instance();
				Player::Players.Add(thePlayer);
			}

			thePlayer->ID = recvPlayer.ID;
			thePlayer->Location = recvPlayer.Location;
			thePlayer->Pitch = recvPlayer.Pitch;
			thePlayer->Yaw = recvPlayer.Yaw;

			BroadcastPacket(recvPlayer.toPacket());
		}
		break;

	default:
		{
			String out = "Server Recieved Unrecognized Packet: ";
			out << *buffer;
			Log::Msg(out, Log::Debug);
		}
		break;
	}
}