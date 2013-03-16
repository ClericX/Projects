#include "Application.h"
#include "../Frost/String.h"
#include "../Frost/StringList.h"
#include "PacketStructures.h"
#include "../FrostGame/Block.h"
#include "Block.h"
#include "../FrostGame/Player.h"

void Application::PacketParser(String *buffer)
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
					Block::Instance *block = new Block::Instance();
					block->Position = recvBlock.BlockLocation;
					block->Type = Block::GetTypeInfoByID(recvBlock.TypeID);
					Block::Add(block);
				}
				break;

			case BlockRelatedPacket::Update:
				{
					//getblockatpos set it
					// unsupported
				}
				break;

			case BlockRelatedPacket::Remove:
				{
					Block::Remove(recvBlock.BlockLocation);
				}
				break;
			}
		}
		break;

	case PlayerRelatedPacket::Header:
		{
			PlayerRelatedPacket recvPlayer = PlayerRelatedPacket::fromPacket(&splitBuffer);

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
		}
		break;

	default:
		{
			String out = "Client Recieved Unrecognized Packet: ";
			out << *buffer;
			Log::Msg(out, Log::Debug);
		}
		break;
	}
}