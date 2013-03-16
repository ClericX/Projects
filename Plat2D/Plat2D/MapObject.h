#pragma once

#include "Includes.h"

class CMapObject
{
public:
	CMapObject(void);
	~CMapObject(void);

	long X;
	long Y;
	long Right()
	{
		return X + LT->ImgWidth;
	}
	long Bottom()
	{
		return X + LT->ImgHeight;
	}
	Rect GetRect();
	void ChangeColor(D3DCOLOR Clr);
	void DefaultColor();
	void ChangeSrcRect(RECT Src);
	void NullSrcRect();
	void Draw(bool WorldDraw);
	void SetLT(LOADEDTEXTURE* lt);
	void SetPos(long CX, long CY);
	void Move(int Direction, long Amount);
	DWORD MovementFlags;
	int JumpSpeed;
	int MoveSpeed;
	bool Dead;
	LOADEDTEXTURE* LT;
	bool Visible;
	void Hide() {	Visible = false;	}
	void Show() {	Visible = true;		}
	D3DCOLOR Color;
	RECT *SrcRect;
	RECT RealSrcRect;
};