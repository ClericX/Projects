#pragma once

#include "Includes.h"

class Direct3D
{
public:
	Direct3D(void);
	~Direct3D(void);

	LPDIRECT3D9 D3DObject;
	LPDIRECT3DDEVICE9 D3DDevice;
	LPD3DXFONT FrostFont;

	//Capabilities of graphics adapter
	D3DCAPS9 d3dCaps;

	//Vertex buffer for drawing quads
	IDirect3DVertexBuffer9* vertexBuffer;

	//Presentation parameters for the device
	D3DPRESENT_PARAMETERS PresentParameters;

	//Sprite interface pointer
	LPD3DXSPRITE Sprite;

	void DrawScene();
	void Initialize(bool Windowed);
	void InitializeFont();
	void InitScene();
	void UpdateCamera();
	void Text( LPD3DXFONT Font, int X, int Y, D3DCOLOR Color, char *String );
	void TextWorld( LPD3DXFONT Font, int X, int Y, D3DCOLOR Color, char *String );

	float AspectRatio, FieldOfView, NearClippingPane, FarClippingPane;

	D3DFORMAT ColourFormat, DepthStencilFormat;
	D3DMULTISAMPLE_TYPE MultiSampling;

	DWORD Anisotropy;

	//Draw a textured quad
	void BlitD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR vertexColour = 0xFFFFFFFF, float fRotate = 0);

	//Draw a textured quad with full colour modulation
	void BlitExD3D (IDirect3DTexture9 *texture, RECT *rDest, D3DCOLOR *vertexColours /* -> D3DCOLOR[4] */, float rotate = 0);

	IDirect3DTexture9 *LoadTexture(LOADEDTEXTURE* lt, char *fileName);

	UINT GetTexWidth(LPDIRECT3DTEXTURE9 Texture);
	UINT GetTexHeight(LPDIRECT3DTEXTURE9 Texture);

	//Draw texture with limited colour modulation
	void Blit (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR vertexColour = 0xFFFFFFFF, float rotate = 0);

	void ScreenBlit (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR vertexColour = 0xFFFFFFFF, float rotate = 0);
	void ScreenBlit (LOADEDTEXTURE* lt, int X, int Y, int Width, int Height, D3DCOLOR vertexColour = 0xFFFFFFFF, float rotate = 0);
	void WorldBlit (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR vertexColour = 0xFFFFFFFF, float rotate = 0);

	void Draw(LPDIRECT3DTEXTURE9 Texture, long X, long Y, RECT *pSrcRect, D3DCOLOR Color, bool WorldDraw);

	//Draw texture with full colour modulation
	void BlitEx (LOADEDTEXTURE* lt, int X, int Y, D3DCOLOR* vertexColours, float rotate);

	static const int index = 0;

	long CamX;
	long CamY;
};
