#include "MobMove.h"
#include "Main.h"

CMobMove::CMobMove(void)
{
}

CMobMove::~CMobMove(void)
{
}

void CMobMove::GenerateMovement(CMobs *Mob)
{
	Mob->TargetMovement.Direction.Value = App.GetRand(0, 5);
	Mob->TargetMovement.Direction.Done = false;
	Mob->TargetMovement.Direction.Progress = 0;
	Mob->TargetMovement.Direction.Completion = 1;

	Mob->TargetMovement.Movement.Value = App.GetRand(100, 200);
	Mob->TargetMovement.Movement.Done = false;
	Mob->TargetMovement.Movement.Progress = 0;
	Mob->TargetMovement.Movement.Completion = Mob->TargetMovement.Movement.Value;

	Mob->TargetMovement.Jump.Value = App.GetRand(0, 10);
	Mob->TargetMovement.Jump.Done = false;
	Mob->TargetMovement.Jump.Progress = 0;
	Mob->TargetMovement.Jump.Completion = 60; //Amount of pixels to go up.

	Mob->TargetMovement.Wait.Value = App.GetRand(0, 3);
	Mob->TargetMovement.Wait.Done = false;
	Mob->TargetMovement.Wait.Progress = 0;
	Mob->TargetMovement.Wait.Completion = ( Mob->TargetMovement.Wait.Value * 60 );
	//This MIGHT need to be more accurately timed, I used 60 'cuz it's usually 60 fps.

	Mob->TargetMovement.Done = false;
}

void CMobMove::DoDirection(CMobs *Mob)
{
	if (Mob->TargetMovement.Direction.Progress != Mob->TargetMovement.Direction.Completion)
	{
		if (Mob->TargetMovement.Direction.Value < 3)
		{
			Mob->Facing = FACING_LEFT;
		}

		else
		{
			Mob->Facing = FACING_RIGHT;
		}

		Mob->TargetMovement.Direction.Progress++;
	}

	else
	{
		Mob->TargetMovement.Direction.Done = true;
	}
}

void CMobMove::DoMovement(CMobs *Mob)
{
	if (Mob->TargetMovement.Movement.Progress < Mob->TargetMovement.Movement.Completion)
	{
		if (Mob->Facing == FACING_LEFT)
		{
			if ( Mob->CanFallOffPlatforms == false && (Mob->X - 1) > Mob->OnPlatform->X )
			{
				Mob->Move(MOVE_LEFT, 1);
			}
			else if (Mob->CanFallOffPlatforms == TRUE)
			{
				Mob->Move(MOVE_LEFT, 1);
			}
			else
			{
				Mob->Facing = FACING_RIGHT;
			}
		}

		else if (Mob->Facing == FACING_RIGHT)
		{
			if ( Mob->CanFallOffPlatforms == false && (Mob->GetRect().right + 1) < Mob->OnPlatform->GetRect().right )
			{
				Mob->Move(MOVE_RIGHT, 1);
			}
			else if (Mob->CanFallOffPlatforms == TRUE)
			{
				Mob->Move(MOVE_RIGHT, 1);
			}
			else
			{
				Mob->Facing = FACING_LEFT;
			}
		}

		Mob->TargetMovement.Movement.Progress++;
	}

	else
	{
		Mob->TargetMovement.Movement.Done = true;
	}
}

void CMobMove::DoJump(CMobs *Mob)
{
	if (Mob->TargetMovement.Jump.Progress < Mob->TargetMovement.Jump.Completion && Mob->TargetMovement.Jump.Value > 7)
	{
		Mob->GravityVars.FallingVelocity = 0; //So gravity doesn't pull it down instantly.
		Mob->Y--; //Bump up 1 pixel.
		Mob->TargetMovement.Jump.Progress++;
	}

	else
	{
		Mob->GravityVars.FallingVelocity = 1; //After the jump has been launched, we want gravity to suck it down again.
		Mob->TargetMovement.Jump.Done = true;
	}
}

void CMobMove::DoWait(CMobs *Mob)
{
	if (Mob->TargetMovement.Wait.Progress < Mob->TargetMovement.Wait.Completion)
	{
		Mob->TargetMovement.Wait.Progress++;
	}

	else
	{
		Mob->TargetMovement.Wait.Done = true;
		Mob->TargetMovement.Done = true;
	}
}

void CMobMove::ApplyMovements(CMobs *Mob)
{
	//Monsters need to have touched a platform since they have spawned in order to be able to use random movement.
	if (Mob->OnPlatform != 0)
	{
		if (!Mob->Dead && Mob->Visible)
		{
			if (Mob->TargetMovement.Done)
			{
				GenerateMovement(Mob);
			}

			else if (!Mob->TargetMovement.Direction.Done)
			{
				DoDirection(Mob);
			}

			else if (!Mob->TargetMovement.Movement.Done)
			{
				DoMovement(Mob);
			}

			else if (!Mob->TargetMovement.Jump.Done)
			{
				DoJump(Mob);
			}

			else if (!Mob->TargetMovement.Wait.Done)
			{
				DoWait(Mob);
			}
		}
		else
		{
			if (Mob->TargetMovement.Done == false)
			{
				Mob->TargetMovement.Done = true;
			}
		}
	}
}