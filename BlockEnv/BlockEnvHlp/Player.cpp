#define FROST_EXPORT
#include "Player.h"

namespace Player
{
	Instance::Instance(void)
	{
		ID = 0;
		Pitch = 0;
		Yaw = 0;
	}

	Instance::~Instance(void)
	{
	}

	List<Instance *> Players;

	void Add(Instance *thePlayer)
	{
		Players.Add(thePlayer);
	}

	void Remove(unsigned int ID)
	{
		for (unsigned int i = 0; i < Players.count(); i++)
		{
			if (Players[i]->ID == ID)
			{
				Players.Remove(ID);
			}
		}
	}

	Instance *Get(unsigned int ID)
	{
		for (unsigned int i = 0; i < Players.count(); i++)
		{
			if (Players[i]->ID == ID)
			{
				return Players[i];
			}
		}
		return (Instance *)0;
	}
};