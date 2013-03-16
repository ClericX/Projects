#include "Block.h"
#include "PacketStructures.h"
#include "Application.h"
#include "../Frost/String.h"

namespace Block
{
	void RequestAdd(Instance *theBlock)
	{
		BlockRelatedPacket p;
		p.ActionType = BlockRelatedPacket::Create;
		p.BlockLocation = theBlock->Position;
		p.TypeID = theBlock->Type->ID;
		Application::SendPacket(p.toPacket());
	}

	void RequestRemove(Instance *theBlock)
	{
		BlockRelatedPacket p;
		p.ActionType = BlockRelatedPacket::Remove;
		p.BlockLocation = theBlock->Position;
		Application::SendPacket(p.toPacket());
	}
};