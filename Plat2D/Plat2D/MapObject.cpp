#include "MapObject.h"
#include "Main.h"

CMapObject::CMapObject(void)
{
	Color = Colors.White;
	SrcRect = 0;
	Visible = true;
	Dead = false;
	MoveSpeed = 1;
	JumpSpeed = 1;
	MovementFlags = 0;
}

CMapObject::~CMapObject(void)
{
}

Rect CMapObject::GetRect()
{
	Rect ply;
	ZeroMemory(&ply, sizeof(Rect));
	ply.left = X;
	ply.right = ply.left + LT->ImgWidth;
	ply.top = Y;
	ply.bottom = ply.top + LT->ImgHeight;
	return ply;
}

void CMapObject::ChangeColor(D3DCOLOR Clr)
{
	Color = Clr;
}

void CMapObject::DefaultColor()
{
	Color = Colors.White;
}

void CMapObject::ChangeSrcRect(RECT Src)
{
	RealSrcRect = Src;
	SrcRect = &RealSrcRect;
}

void CMapObject::NullSrcRect()
{
	SrcRect = 0;
}

void CMapObject::Draw(bool WorldDraw)
{
	if (Visible)
	{
		D3D.Draw(LT->Texture, X, Y, SrcRect, Color, WorldDraw);
	}
}

void CMapObject::SetLT(LOADEDTEXTURE* lt)
{
	LT = lt;
}

void CMapObject::SetPos(long CX, long CY)
{
	X = CX;
	Y = CY;
}

void CMapObject::Move(int Direction, long Amount)
{
	if (!Dead)
	{
		if (Direction == MOVE_LEFT && !CheckFlag(MovementFlags, FROST_MOVEMENTFLAGS_LEFT))
		{
			X -= (Amount * MoveSpeed);
		}
		else if (Direction == MOVE_RIGHT && !CheckFlag(MovementFlags, FROST_MOVEMENTFLAGS_RIGHT))
		{
			X += (Amount * MoveSpeed);
		}
		else if (Direction == MOVE_UP && !CheckFlag(MovementFlags, FROST_MOVEMENTFLAGS_UP))
		{
			Y -= (Amount * JumpSpeed);
		}
		else if (Direction == MOVE_DOWN && !CheckFlag(MovementFlags, FROST_MOVEMENTFLAGS_DOWN))
		{
			Y += (Amount * JumpSpeed);
		}
	}
}