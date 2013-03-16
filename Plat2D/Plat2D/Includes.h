#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <time.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <vector>
#include <fstream>
#include <atlstr.h>
#include <direct.h> // for getcwd

using namespace std;

#define DIRECTINPUT_VERSION 0x0800

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>

////////////////////

static const int FACING_LEFT = 0;
static const int FACING_RIGHT = 1;

static const int DRAW_WORLD = 1;
static const int DRAW_SCREEN = 0;

static const int MOVE_LEFT = 0;
static const int MOVE_RIGHT = 1;
static const int MOVE_UP = 2;
static const int MOVE_DOWN = 3;

static const DWORD FROST_MOVEMENTFLAGS_RIGHT = 0x00000001;
static const DWORD FROST_MOVEMENTFLAGS_LEFT = 0x00000010;
static const DWORD FROST_MOVEMENTFLAGS_UP = 0x00000100;
static const DWORD FROST_MOVEMENTFLAGS_DOWN = 0x00001000;

static const DWORD FROST_MOVEMENTFLAGS_RIGHT_SET = 0x00010000;
static const DWORD FROST_MOVEMENTFLAGS_LEFT_SET = 0x00100000;
static const DWORD FROST_MOVEMENTFLAGS_UP_SET = 0x01000000;
static const DWORD FROST_MOVEMENTFLAGS_DOWN_SET = 0x10000000;

static const int STATUS_LEAVING = -1;
static const int STATUS_NEUTRAL = 0;
static const int STATUS_ENTERING = 1;

typedef struct _Colors
{
	static const DWORD Pink = D3DCOLOR_ARGB(255, 255, 0, 128);
	static const DWORD White = D3DCOLOR_ARGB(255, 255, 255, 255);
	static const DWORD BlinkGrey = D3DCOLOR_ARGB(255, 200, 200, 200);
	static const DWORD Black = D3DCOLOR_ARGB(255, 0, 0, 0);
	static const DWORD Transparent = D3DCOLOR_ARGB(0, 0, 0, 0);
} Colors;

//Custom vertex format
const DWORD D3DFVF_TLVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

/* Vertices:

    v0               v1
    |---------------|
    |\              |
    |  \            |
    |    \          |
    |      \        |
    |        \      |
    |          \    |
    |            \  |
    |              \|
    |---------------|
    v3               v2

    z = 0
    rhw = 1
*/

//Custom vertex
struct TLVERTEX
{
    float x;
    float y;
    float z;
    float rhw;
    D3DCOLOR colour;
    float u;
    float v;
};

//Loaded texture struct
struct LOADEDTEXTURE
{
	int referenceCount;             //Amount of CTexture instances containing this texture
	IDirect3DTexture9* Texture;     //The texture
	string Filename;               //The filename of the texture
	int Width;                      //Width of the texture
	int Height;                     //Height of the texture
	int ImgWidth;
	int ImgHeight;
};

//Variables for Gravity checks || For player & mobs
struct GRAVITYVARS
{
	bool GravityPulling;
	bool FoundSomething;
	int FallingTime;
	int FallingVelocity;
	int NumOfVelocityIncreases;
};

static const DWORD FROST_READPIXEL_LOCK = 0x00001;
static const DWORD FROST_READPIXEL_READ = 0x00010;
static const DWORD FROST_READPIXEL_UNLOCK = 0x00100;
static const DWORD FROST_READPIXEL_SCREENCOORDS = 0x01000;
static const DWORD FROST_READPIXEL_CHECKBOUNDS = 0x10000;

struct PixelColor
{
	BYTE B;
	BYTE G;
	BYTE R;
	BYTE A;
};

struct Rect
{
	long left;
	long top;
	long right;
	long bottom;
	long width()
	{
		return right - left;
	}
	long height()
	{
		return bottom - top;
	}
	long cX()
	{
		return left + (width() / 2);
	}
	long cY()
	{
		return top + (height() / 2);
	}
	long hW()
	{
		return width() / 2;
	}
	long hH()
	{
		return height() / 2;
	}
	POINT Center()
	{
		POINT ret = { left + (width() / 2) , top + (height() / 2) };
		return ret;
	}
};

struct LOADEDFRAME
{
	LOADEDTEXTURE* LT;
	unsigned int Delay;
};

struct MOBFRAMESTRUCT
{
	vector<LOADEDFRAME*> Die;
	vector<LOADEDFRAME*> Hit;
	vector<LOADEDFRAME*> Move;
	vector<LOADEDFRAME*> Stand;
};

static const LPCSTR ImgDir = "img/";
static const LPCSTR MobDir = "img/Mob/";
static const LPCSTR MapDir = "img/Map/";
static const LPCSTR ItemDir = "img/Item/";
static const LPCSTR PNGExtentionStr = ".png";
static const LPCSTR INIExtentionStr = ".ini";

struct MAPOBJECT
{
	unsigned int ID;
	long X;
	long Y;
	unsigned int TargetMapID;
};