#pragma once

#include "Includes.h"
#include "Platforms.h"
#include "Mobs.h"
#include "Maps.h"
#include "Backgrounds.h"
#include "MapObject.h"
#include "Portals.h"
#include "Player.h"
#include "Items.h"

///////////////////////////////////////////////////////////////
//Holds the functions for managing loaded textures and lists.//
///////////////////////////////////////////////////////////////

class CTextures
{
public:
	CTextures(void);
	~CTextures(void);

    //Load texture from file
	LOADEDTEXTURE* InitCore(string Filename);
	void InitPlatform(unsigned int ID, int Instance, int X, int Y);
	void InitMob(unsigned int ID, int Instance, int X, int Y);
    void InitLT(list<LOADEDTEXTURE*> &lt, string Filename);

	//Iterate through a list and find a loaded texture pointer
	CPlatforms* GetPlatform(unsigned int PlatformID, int Instance);
	CMobs* GetMob(unsigned int ID, int Instance);
	LOADEDTEXTURE* GetLT(list<LOADEDTEXTURE*> lt, string Filename);

	void DrawAllBackgrounds();
	void DrawAllPlatforms();
	void DrawAllMobs();
	void DrawAllPortals();
	void DrawAllOtherPlayers();
	void DrawAllItems();
	void GravityAllMobs();
	void AutoMoveAllMobs();
	void DetectAttacksOnAllMobs(CMapObject *Obj);
	void DrawAllMobStats();
	void CheckMobsForDamage();
	bool CheckForPortalEntrance();
	CPlayer *GetPlayerByID(unsigned int ID);

	PixelColor GetTexturePixelColor(CMapObject *Obj, long X, long Y, DWORD Flags, D3DLOCKED_RECT *lr);

	void LoadMob(unsigned int ID);
	void UnloadMob(unsigned int ID);
	CMobs *GetLoadedMob(unsigned int ID);

	void LoadMap(unsigned int ID);
	void UnloadMap(unsigned int ID);
	CMaps *GetLoadedMap(unsigned int ID);
	void SetMap(unsigned int ID);

	void LoadPlatform(unsigned int MapID, unsigned int PlatformID);
	void UnloadPlatform(unsigned int MapID, unsigned int PlatformID);
	CPlatforms *GetLoadedPlatform(unsigned int MapID, unsigned int PlatformID);

	void LoadBackground(unsigned int MapID, unsigned int BackgroundID);
	void UnloadBackground(unsigned int MapID, unsigned int BackgroundID);
	CBackgrounds *GetLoadedBackground(unsigned int MapID, unsigned int BackgroundID);

	void LoadPortal(unsigned int ID);
	void UnloadPortal(unsigned int ID);
	CPortals *GetLoadedPortal(unsigned int ID);

	void LoadItem(unsigned int ID);
	void UnloadItem(unsigned int ID);
	CItems *GetLoadedItem(unsigned int ID);

	void ClearMap();
	void ChangeMap(unsigned int ID);

    //Unload a texture
    void Close(LOADEDTEXTURE* lt);

    //Release all unreferenced textures
    void GarbageCollect(list<LOADEDTEXTURE*> lt);

    //Release all unreferenced textures
    void CleanupTextures(list<LOADEDTEXTURE*> lt);
};