#pragma once

#include "DLL.h"

#include <Windows.h>
#include "Rect.h"
#include "Point.h"

namespace Frost
{
	namespace Collision
	{
		bool Overlaps(int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3)
		{
			return ( (! ( ((x0)<(x2) && (x1)<(x2)) || ((x0)>(x3) && (x1)>(x3)) || ((y0)<(y2) && (y1)<(y2)) || ((y0)>(y3) && (y1)>(y3)) ) ) );
		}

		// Object-to-object bounding-box collision detector
		bool RectToRect(Rect one, Rect two)
		{
			bool Check1 = ( one.Bottom < two.Top );
			bool Check2 = ( one.Top > two.Bottom );
			bool Check3 = ( one.Right < two.Left );
			bool Check4 = ( one.Left > two.Right );

			// Trivial rejections
			if ( Check1 || Check2 || Check3 || Check4 )
			{
				return false;
			}

			return true;
		}

		bool PointToRect(Point point, Rect rect)
		{
			if (point.X >= rect.Left && point.X <= rect.Right && point.Y >= rect.Top && point.Y <= rect.Bottom)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};
};

	/*
	bool PixelToPoint(CMapObject *MapObj, long X, long Y)
	{
		return PixelToPoint(MapObj, X, Y, 0);
	}

	// Object-to-Point, pixel-level collision detector
	bool PixelToPoint(CMapObject *MapObj, long X, long Y, D3DLOCKED_RECT* lpLockedRect)
	{
		Rect one = MapObj->GetRect();

		int Check1 = ( X < one.left );
		int Check2 = ( X > one.right );
		int Check3 = ( Y < one.top );
		int Check4 = ( Y > one.bottom );

		if ( Check1 || Check2 || Check3 || Check4 )
		{
			return false;
		}

		int XOffset = abs(X - one.left);
		int YOffset = abs(Y - one.top);

		PixelColor px;

		if (lpLockedRect == NULL)
		{
			px = Textures.GetTexturePixelColor(MapObj, XOffset, YOffset, FROST_READPIXEL_LOCK | FROST_READPIXEL_READ | FROST_READPIXEL_UNLOCK, 0);
		}
		else if (lpLockedRect != NULL)
		{
			px = Textures.GetTexturePixelColor(MapObj, XOffset, YOffset, FROST_READPIXEL_READ, lpLockedRect);
		}

		if ( px.A == 0 )
		{
			return false;
		}

		return true;
	}

	// Full object-to-object pixel-level collision detector
	bool TextureToTexture(CMapObject *ObjOne, CMapObject *ObjTwo)
	{
		Rect one = ObjOne->GetRect();
		Rect two = ObjTwo->GetRect();

		POINT pxscn1;
		POINT pxscn2;

		Rect over;

		bool Check1 = ( one.bottom < two.top );
		bool Check2 = ( one.top > two.bottom );
		bool Check3 = ( one.right < two.left );
		bool Check4 = ( one.left > two.right );

		// Trivial rejections
		if ( Check1 || Check2 || Check3 || Check4 )
		{
			return false;
		}

		// Compute the rectangle of overlap
		// and compute starting offsets into both objects' bitmaps

		// Lefts
		if ( two.left < one.left )
		{
			over.left = one.left;
			pxscn1.x = 0;
			pxscn2.x = ( two.width() - ( two.right - one.left ) );
		}
		else if ( two.left > one.left )
		{
			over.left = two.left;
			pxscn1.x = ( one.width() - ( one.right - two.left ) );
			pxscn2.x = 0;
		}
		else
		{
			over.left = 0;
			pxscn1.x = 0;
			pxscn2.x = 0;
		}

		// Tops
		if ( two.top < one.top )
		{
			over.top = one.top;
			pxscn1.y = 0;
			pxscn2.y = ( two.height() - ( two.bottom - one.top ) );
		}
		else if ( two.top > one.top )
		{
			over.top = two.top;
			pxscn1.y = ( one.height() - ( one.bottom - two.top ) );
			pxscn2.y = 0;
		}
		// start a comment here
		else
		{
			over.top = 0;
			pxscn1.y = 0;
			pxscn2.y = 0;
		}
		// end a comment here

		// Rights
		if ( two.right < one.right )
		{
			over.right = two.right;
		}
		else if ( two.right > one.right )
		{
			over.right = one.right;
		}
		else
		{
			over.right = 0;
		}

		// Bottoms
		if ( two.bottom < one.bottom )
		{
			over.bottom = two.bottom;
		}
		else if ( two.bottom > one.bottom )
		{
			over.bottom = one.bottom;
		}
		else
		{
			over.bottom = 0;
		}

		// Now start scanning the whole rectangle of overlap,
		// checking the corresponding pixel of each object's
		// bitmap to see if they're both non-zero

		D3DLOCKED_RECT tex1lr;
		D3DLOCKED_RECT tex2lr;

		Textures.GetTexturePixelColor(ObjOne, 0, 0, FROST_READPIXEL_LOCK, &tex1lr);
		Textures.GetTexturePixelColor(ObjTwo, 0, 0, FROST_READPIXEL_LOCK, &tex2lr);

		for (long h = 0; h < over.height(); h++)
		{
			for (long w = 0; w < over.width(); w++)
			{
				if ( ( Textures.GetTexturePixelColor(ObjOne, (pxscn1.x + w), (pxscn1.y + h), FROST_READPIXEL_READ, &tex1lr).A > 0 ) && ( Textures.GetTexturePixelColor(ObjTwo, (pxscn2.x + w), (pxscn2.y + h), FROST_READPIXEL_READ, &tex2lr).A > 0 ) )
				{
					Textures.GetTexturePixelColor(ObjOne, 0, 0, FROST_READPIXEL_UNLOCK, 0);
					Textures.GetTexturePixelColor(ObjTwo, 0, 0, FROST_READPIXEL_UNLOCK, 0);
					return true;
				}
			}
		}

		// Worst case!  We scanned through the whole darn rectangle of overlap 
		// and couldn't find a single colliding pixel!

		Textures.GetTexturePixelColor(ObjOne, 0, 0, FROST_READPIXEL_UNLOCK, 0);
		Textures.GetTexturePixelColor(ObjTwo, 0, 0, FROST_READPIXEL_UNLOCK, 0);

		return false;
	}
	*/