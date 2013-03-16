#include "KeyActions.h"
#include "Main.h"
#include "PacketStructures.h"

void KeyActions::CharMovement::MoveLeft()
{
	Player.Move(MOVE_LEFT, 5);
}

void KeyActions::CharMovement::MoveRight()
{
	Player.Move(MOVE_RIGHT, 5);
}

void KeyActions::CharMovement::Jump()
{
	if (!Textures.CheckForPortalEntrance())
	{
		if (Player.Haste)
		{
			Player.Y -= 200;
		}
		else
		{
			Player.Y -= 100;
		}
	}
}

void KeyActions::CameraMovement::CamRight()
{
	Cam("Right", 5);
}

void KeyActions::CameraMovement::CamLeft()
{
	Cam("Left", 5);
}

void KeyActions::CameraMovement::CamDown()
{
	Cam("Down", 5);
}

void KeyActions::CameraMovement::CamUp()
{
	Cam("Up", 5);
}

void KeyActions::CameraMovement::Cam(string Direction, int Offset)
{
	if (Direction == "Up")
	{
		D3D.CamY -= Offset;
	}
	else if (Direction == "Down")
	{
		D3D.CamY += Offset;
	}
	else if (Direction == "Left")
	{
		D3D.CamX -= Offset;
	}
	else if (Direction == "Right")
	{
		D3D.CamX += Offset;
	}
}

void KeyActions::Testing::HealHP()
{
	Player.ChangeHP(500, false, false);
}

void KeyActions::Testing::HealMP()
{
	Player.ChangeMP(500, false, false);
}

void KeyActions::Testing::Haste()
{
	if (Player.Haste)
	{
		Player.Haste = false;
		Player.MoveSpeed = 1;
		Player.JumpSpeed = 1;
	}
	else if (!Player.Haste)
	{
		Player.Haste = true;
		Player.MoveSpeed = 2;
		Player.JumpSpeed = 2;
	}
}

void KeyActions::Testing::Attack()
{
	if (Player.ChangeMP(100, true, true))
	{
		Attacking.Skills.StarSmash.StartAttack = true;
	}
}

void KeyActions::Testing::ReviveSnail()
{
	Textures.GetMob(1, 0)->Respawn();
}

void KeyActions::Testing::SwitchMap()
{
	if (Player.CurrentMapID == 1)
	{
		Textures.ChangeMap(2);
	}
	else if (Player.CurrentMapID == 2)
	{
		Textures.ChangeMap(1);
	}
}

void KeyActions::Testing::Looting()
{
	list <CItems*>::iterator it;
	it = Items.begin();

	while (it != Items.end())
	{
		if (Collision.RectToRect((*it)->GetRect(), Player.GetRect()))
		{
			Player.Money += (*it)->Amount;
            delete (*it);
            it = Items.erase (it);
			break;
		}
		else
		{
			it++;
		}
	}
}