#include "Gravity.h"
#include "Main.h"

CGravity::CGravity(void)
{
	GravityEnabled = true;
}

CGravity::~CGravity(void)
{
}

bool CGravity::IsStoppingGravity(CMapObject *Ground, CMapObject *Object, int FallingVelocity)
{
	Rect FloorRect = Ground->GetRect();
	Rect AffectedRect = Object->GetRect();

	if (AffectedRect.right >= FloorRect.left) //Inside the left of the ground.
	{
		if (AffectedRect.left < FloorRect.right) //Inside the right of the ground.
		{
			/*
			if ( ( AffectedRect.bottom == FloorRect.top ) || ( AffectedRect.bottom >= ( FloorRect.top - (FallingVelocity - 1) ) && AffectedRect.bottom < FloorRect.top ) )
			{
				return true;
			}
			*/

			D3DLOCKED_RECT pt;
			Textures.GetTexturePixelColor(Ground, 0, 0, FROST_READPIXEL_LOCK, &pt);
			PixelColor At = Textures.GetTexturePixelColor(Ground, AffectedRect.left + (AffectedRect.width() / 2), AffectedRect.bottom - 1, FROST_READPIXEL_READ | FROST_READPIXEL_SCREENCOORDS | FROST_READPIXEL_CHECKBOUNDS, &pt);
			PixelColor Under = Textures.GetTexturePixelColor(Ground, AffectedRect.left + (AffectedRect.width() / 2), AffectedRect.bottom, FROST_READPIXEL_READ | FROST_READPIXEL_SCREENCOORDS | FROST_READPIXEL_CHECKBOUNDS, &pt);
			Textures.GetTexturePixelColor(Ground, 0, 0, FROST_READPIXEL_UNLOCK, 0);

			if (Under.A != 0 && At.A == 0)
			{
				return true;
			}
			else if (Under.A != 0 && At.A != 0)
			{
				Object->Move(MOVE_UP, 1);
				return true;
			}
			else
			{
				return false;
			}
		}

		return false;
	}

	else
	{
		//The floor isn't being touched, don't stop
		return false;
	}
}

void CGravity::IsBesidePlatform(CMapObject *Platform, CMapObject *Object)
{
	//Right touching check
	if ( Object->X <= Platform->Right() && abs( (long)(Platform->Right() - Object->X) ) < 6 && abs( (long)(Platform->Y - Object->Y) ) < 30 && !CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET) )
	{
		SetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET);

		if (!CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT))
		{
			SetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT);
		}
	}
	else
	{
		if (!CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET))
		{
			if (CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT))
			{
				UnsetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT);
			}
		}
	}

	//Left touching check
	if ( Object->Right() >= Platform->X && abs( (long)(Platform->X - Object->Right()) ) < 6 && abs( (long)(Platform->Y - Object->Y) ) < 30 && !CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET) )
	{
		SetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET);

		if (!CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT))
		{
			SetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT);
		}
	}
	else
	{
		if (!CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET))
		{
			if (CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT))
			{
				UnsetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_LEFT);
			}
		}
	}

	//Top touching check
	if ( Object->X >= Platform->X && Object->Right() <= Platform->Right() && Object->Y <= Platform->Bottom() && abs( (long)(Platform->Bottom() - Object->Y) ) < 6 && !CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP_SET) )
	{
		SetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP_SET);

		if (!CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP))
		{
			SetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP);
		}
	}
	else
	{
		if (!CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP_SET))
		{
			if (CheckFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP))
			{
				UnsetFlag(Object->MovementFlags, FROST_MOVEMENTFLAGS_UP);
			}
		}
	}

	//Disable bottom movement
	//Not useful in this context/implemented? lol
}

void CGravity::PlayerGravityCheck(CPlayer &Player)
{
	if (GravityEnabled)
	{
		list<CPlatforms*>::iterator start, it, end;

		start = Platforms.begin();
		end = Platforms.end();

		for (it = start; it != end; it++)
		{
			//IsBesidePlatform( (*it), &Player );

			if ( IsStoppingGravity( (*it), &Player, Player.GravityVars.FallingVelocity ) == true )
			{
				Player.GravityVars.GravityPulling = false;
				Player.GravityVars.FallingVelocity = 1;
				Player.GravityVars.NumOfVelocityIncreases = 1;
				Player.GravityVars.FallingTime = 0;
				Player.GravityVars.FoundSomething = true;

				Player.OnPlatform = (*it);
			}
		}

		if (CheckFlag(Player.MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET))
		{
			UnsetFlag(Player.MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET);
		}
		if (CheckFlag(Player.MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET))
		{
			UnsetFlag(Player.MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET);
		}
		if (CheckFlag(Player.MovementFlags, FROST_MOVEMENTFLAGS_UP_SET))
		{
			UnsetFlag(Player.MovementFlags, FROST_MOVEMENTFLAGS_UP_SET);
		}

		if (Player.GravityVars.GravityPulling)
		{
			Player.Y += (1 * Player.GravityVars.FallingVelocity); //fall moar
			Player.GravityVars.FallingTime++;
			if (Player.GravityVars.FallingTime > (5 * Player.GravityVars.NumOfVelocityIncreases) && Player.GravityVars.NumOfVelocityIncreases <= 10 )
			{
				Player.GravityVars.FallingVelocity++;
				Player.GravityVars.NumOfVelocityIncreases++;
			}
		}
		else if (!Player.GravityVars.GravityPulling && !Player.GravityVars.FoundSomething)
		{
			Player.GravityVars.GravityPulling = true;
		}

		if (Player.GravityVars.FoundSomething)
		{
			Player.GravityVars.FoundSomething = false;
		}
	}
}

void CGravity::MobGravityCheck(CMobs *Mob)
{
	if (GravityEnabled)
	{
		list<CPlatforms*>::iterator start, it, end;

		start = Platforms.begin();
		end = Platforms.end();

		for (it = start; it != end; it++)
		{
			//IsBesidePlatform( (*it), Mob );

			if ( IsStoppingGravity( (*it), Mob, Mob->GravityVars.FallingVelocity ) == true )
			{
				Mob->GravityVars.GravityPulling = false;
				Mob->GravityVars.FallingVelocity = 1;
				Mob->GravityVars.NumOfVelocityIncreases = 1;
				Mob->GravityVars.FallingTime = 0;
				Mob->GravityVars.FoundSomething = true;

				Mob->OnPlatform = (*it);
			}
		}

		if (CheckFlag(Mob->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET))
		{
			UnsetFlag(Mob->MovementFlags, FROST_MOVEMENTFLAGS_RIGHT_SET);
		}
		if (CheckFlag(Mob->MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET))
		{
			UnsetFlag(Mob->MovementFlags, FROST_MOVEMENTFLAGS_LEFT_SET);
		}
		if (CheckFlag(Mob->MovementFlags, FROST_MOVEMENTFLAGS_UP_SET))
		{
			UnsetFlag(Mob->MovementFlags, FROST_MOVEMENTFLAGS_UP_SET);
		}

		if (Mob->GravityVars.GravityPulling)
		{
			Mob->Y += (1 * Mob->GravityVars.FallingVelocity); //fall moar
			Mob->GravityVars.FallingTime++;
			if (Mob->GravityVars.FallingTime > (5 * Mob->GravityVars.NumOfVelocityIncreases) && Mob->GravityVars.NumOfVelocityIncreases <= 10 )
			{
				Mob->GravityVars.FallingVelocity++;
				Mob->GravityVars.NumOfVelocityIncreases++;
			}
		}
		else if (!Mob->GravityVars.GravityPulling && !Mob->GravityVars.FoundSomething)
		{
			Mob->GravityVars.GravityPulling = true;
		}

		if (Mob->GravityVars.FoundSomething)
		{
			Mob->GravityVars.FoundSomething = false;
		}
	}
}