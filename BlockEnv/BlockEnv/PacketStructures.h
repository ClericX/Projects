#pragma once

#include "../Frost/Vector3f.h"
#include "../Frost/String.h"
#include "../Frost/StringList.h"

class BlockRelatedPacket
{
public:
	BlockRelatedPacket(void)
	{
	}

	enum ActionTypes
	{
		Create,
		Update,
		Remove
	};

	static const unsigned char Header = 1;
	unsigned char ActionType; //Create/Modify/Remove
	Vector3f BlockLocation;
	unsigned int TypeID;

	String toPacket()
	{
		String ret;

		ret << Header << "|" << ActionType << "|" << (int)BlockLocation.x << "|" << (int)BlockLocation.y << "|" << (int)BlockLocation.z << "|" << (int)TypeID;

		return ret;
	}

	static BlockRelatedPacket fromPacket(StringList *packet)
	{
		BlockRelatedPacket ret;

		ret.ActionType = (unsigned char)packet->operator[](1).toInt();
		
		ret.BlockLocation.x = (float)packet->operator[](2).toInt();
		ret.BlockLocation.y = (float)packet->operator[](3).toInt();
		ret.BlockLocation.z = (float)packet->operator[](4).toInt();

		ret.TypeID = (unsigned int)packet->operator[](5).toInt();

		return ret;
	}
};

class PlayerRelatedPacket
{
public:
	PlayerRelatedPacket(void)
	{
	}

	String toPacket()
	{
		String ret;

		ret << Header << "|" << (int)ID << "|" << (int)Location.x << "|" << (int)Location.y << "|" << (int)Location.z << "|" << (int)Pitch << "|" << (int)Yaw;

		return ret;
	}

	static PlayerRelatedPacket fromPacket(StringList *packet)
	{
		PlayerRelatedPacket ret;

		ret.ID = (unsigned int)packet->operator[](1).toInt();

		ret.Location.x = (float)packet->operator[](2).toInt();
		ret.Location.y = (float)packet->operator[](3).toInt();
		ret.Location.z = (float)packet->operator[](4).toInt();

		ret.Pitch = (float)packet->operator[](5).toInt();
		ret.Yaw = (float)packet->operator[](6).toInt();

		return ret;
	}

	static const unsigned char Header = 2;
	unsigned int ID;
	Vector3f Location;
	float Pitch;
	float Yaw;
};