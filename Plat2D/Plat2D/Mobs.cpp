#include "Mobs.h"
#include "Main.h"

CMobs::CMobs(void)
{
	GravityVars.GravityPulling = true;
	GravityVars.FallingVelocity = 1;
	GravityVars.NumOfVelocityIncreases = 1;
	GravityVars.FallingTime = 0;
	GravityVars.FoundSomething = false;

	OnPlatform = 0;
	MobID = 0;
}

CMobs::~CMobs(void)
{
}

bool CMobs::Attacked(CMapObject *Obj)
{
	if (Player.Attacking)
	{
		if (Collision.TextureToTexture(Obj, this))
		{
			ChangeHP(1, true, false);
			return true;
		}
		return false;
	}
	return false;
}

/*
This function handles gaining / losing HP.
It handles various different scenarios, and should be able to handle every possible one.
*/
bool CMobs::ChangeHP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked)
{
	// Handle if we have a negative number.
	if ( Decrease )
	{
		// If we have enough HP left to take away.
		if (HP >= ByWhat)
		{
			HP -= ByWhat;
		}

		// Else if we don't, just set the HP to 0.
		else if (HP < ByWhat)
		{
			// If this is not a check of some sort, kill the player (Like if this is monster damage).
			if (!ThisIsChecked)
			{
				HP = 0;
			}

			// If this is a check, just return false so we know the player doesn't have enough
			// HP to perform the operation they are trying to do (like use a skill).
			else if (ThisIsChecked)
			{
				return false;
			}
		}

		// This would mean the monster has died recently.
		if (Dead == false && HP == 0)
		{
			Player.ChangeEXP(EXPGiven, false);
			CItems *NewItem = new CItems(9001, 10, this->GetRect());
			Items.push_back( NewItem );
		}

		// If the player has 0 HP, they are dead. Set this value accordingly.
		if (HP == 0)
		{
			Dead = true;
		}
	}

	// Handle if we have a positive number.
	else if ( !Decrease )
	{
		// If we are trying to gain more hp than the max.
		if ( (HP + ByWhat) > MaxHP )
		{
			// Just heal to full.
			HP = MaxHP;
		}

		// Otherwise, we're just trying a regular heal, so just increase HP normally.
		else
		{
			HP += ByWhat;
		}

		// If the player has the dead flag set, we just healed HP meaning they can't have 0 anymore. Set this value accordingly.
		// This function might be called with a value of 0 for whatever reason, and since in this case we don't want a major exploit where
		// you could be alive with 0 HP, we will always check if they are dead AND have greater than 0 hp before setting them to alive.
		if (Dead == true && HP > 0)
		{
			Dead = false;
		}
	}

	DeathCheck();

	return true;
}

bool CMobs::ChangeMP(unsigned long ByWhat, bool Decrease, bool ThisIsChecked)
{
	// Handle if we have a negative number.
	if ( Decrease )
	{
		// If we have enough MP left to take away.
		if (MP >= ByWhat)
		{
			MP -= ByWhat;
		}

		// Else if we don't, just set the MP to 0.
		else if (MP < ByWhat)
		{
			// If this is not a check of some sort, drain remaining MP (Like if this is monster damage to MP).
			if (!ThisIsChecked)
			{
				MP = 0;
			}

			// If this is a check, just return false so we know the player doesn't have enough
			// MP to perform the operation they are trying to do (like use a skill).
			else if (ThisIsChecked)
			{
				return false;
			}
		}
	}

	// Handle if we have a positive number.
	else if ( !Decrease )
	{
		// If we are trying to gain more mp than the max.
		if ( (MP + ByWhat) > MaxMP )
		{
			// Just heal to full.
			MP = MaxMP;
		}

		// Otherwise, we're just trying a regular heal, so just increase MP normally.
		else
		{
			MP += ByWhat;
		}
	}

	return true;
}

void CMobs::DeathCheck()
{
	if (Dead == true)
	{
		Visible = false;
	}
	else if (Dead == false)
	{
		Visible = true;
	}
}

void CMobs::DrawStats()
{
	if (Visible)
	{
		char buf[100];
		char ultoabuf[10];
		strcpy(buf, MobName.c_str());
		strcat(buf, "\n");
		strcat(buf, "HP: ");
		strcat(buf, _ultoa(HP, ultoabuf, 10));
		strcat(buf, " / ");
		strcat(buf, _ultoa(MaxHP, ultoabuf, 10));
		strcat(buf, "\n");
		strcat(buf, "MP: ");
		strcat(buf, _ultoa(MP, ultoabuf, 10));
		strcat(buf, " / ");
		strcat(buf, _ultoa(MaxMP, ultoabuf, 10));
		strcat(buf, "\n");
		strcat(buf, "EXP: ");
		strcat(buf, _ultoa(EXPGiven, ultoabuf, 10));
		D3D.TextWorld(D3D.FrostFont, X - 10, Y - (20*4), D3DCOLOR_XRGB( 0xFF, 0xFF, 0xFF ), buf);
	}
}

void CMobs::Respawn()
{
	ChangeHP(MaxHP, false, false);
	ChangeMP(MaxMP, false, false);
}