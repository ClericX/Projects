#pragma once

#include "DLL.h"
#include "../Frost/Vector3f.h"
#include "../Frost/List.h"

namespace Player
{
	class FROSTAPI Instance
	{
	public:
		Instance(void);
		~Instance(void);

		unsigned int ID;
		Vector3f Location;
		float Pitch;
		float Yaw;
	};

	extern "C" FROSTAPI List<Instance *> Players;

	void FROSTAPI Add(Instance *thePlayer);
	void FROSTAPI Remove(unsigned int ID);
	Instance FROSTAPI *Get(unsigned int ID);
};