#include "AutoBan.h"
#include "Main.h"

CAutoBan::CAutoBan(void)
{
}

CAutoBan::~CAutoBan(void)
{
}

bool CAutoBan::NoHPGodMode()
{
	if (Player.HP <= 0)
	{
		if (!Player.Dead)
		{
			return true;
		}
	}

	return false;
}