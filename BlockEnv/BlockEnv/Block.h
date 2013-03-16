#pragma once

#include "../Frost/List.h"
#include "../Frost/Vector3f.h"
#include "../FrostGame/Block.h"

namespace Block
{
	void RequestAdd(Instance *theBlock);
	void RequestRemove(Instance *theBlock);
};