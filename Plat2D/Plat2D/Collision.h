#pragma once

#include "Includes.h"
#include "MapObject.h"

class CCollision
{
public:
	CCollision(void);
	~CCollision(void);

	bool Overlaps(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3);
	bool RectToRect(Rect one, Rect two);
	bool PixelToPoint(CMapObject *MapObj, long X, long Y);
	bool PixelToPoint(CMapObject *MapObj, long X, long Y, D3DLOCKED_RECT* lpLockedRect);
	bool TextureToTexture(CMapObject *ObjOne, CMapObject *ObjTwo);
	bool PointToRect(POINT point, Rect rect);
};