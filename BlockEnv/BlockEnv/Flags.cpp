#include "Flags.h"

namespace Flags
{
	// Functions related to flag handling.
	void SetFlag(DWORD& Target, DWORD Flag)
	{
		Target = Target | Flag;
	}

	void UnsetFlag(DWORD& Target, DWORD Flag)
	{
		Target = Target & ~Flag;
	}

	bool CheckFlag(DWORD& Target, DWORD Flag)
	{
		return ( (Target & Flag) == Flag );
	}

	void InvertFlag(DWORD& Target)
	{
		Target = ~Target;
	}
};