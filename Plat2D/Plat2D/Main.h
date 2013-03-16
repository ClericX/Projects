#pragma once

#include "Includes.h"

#include "Application.h"
#include "Direct3D.h"
#include "DirectInput.h"
#include "Player.h"
#include "Textures.h"
#include "Gravity.h"
#include "KeyActions.h"
#include "Platforms.h"
#include "Mobs.h"
#include "MobMove.h"
#include "UI.h"
#include "Collision.h"
#include "Attacking.h"
#include "Portals.h"
#include "PacketParser.h"
#include "HiResTimer.h"
#include "Items.h"

extern "C" Application App;
extern "C" Direct3D D3D;
extern "C" DirectInput DI;
extern "C" CPlayer Player;
extern "C" CTextures Textures;
extern "C" CGravity Gravity;
extern "C" CMobMove MobMove;
extern "C" CUI UI;
extern "C" CCollision Collision;
extern "C" CAttacking Attacking;
extern "C" HiResTimer Timer;

long GetPrivateProfileLong(__in  LPCTSTR lpAppName, __in  LPCTSTR lpKeyName, __in  long nDefault, __in  LPCTSTR lpFileName);
void HandleError(LPCSTR errorInstance);
int strpos(char *haystack, char *needle);
POINT MakePoint(int X, int Y);
RECT MakeRect(int Left, int Top, int Right, int Bottom);

//Linked lists of all loaded textures
extern "C" list <CPlatforms*> Platforms;
extern "C" list <CMobs*> Mobs;
extern "C" list <CBackgrounds*> Backgrounds;
extern "C" list <CPortals*> Portals;
extern "C" list <CPlatforms*> LoadedPlatforms;
extern "C" list <CMobs*> LoadedMobs;
extern "C" list <CBackgrounds*> LoadedBackgrounds;
extern "C" list <CMaps*> LoadedMaps;
extern "C" list <CPortals*> LoadedPortals;
extern "C" list <CPlayer*> OtherPlayers;
extern "C" list <CItems*> LoadedItems;
extern "C" list <CItems*> Items;

//Functions related to flag handling.
void SetFlag(DWORD& Target, DWORD Flag);
void UnsetFlag(DWORD& Target, DWORD Flag);
bool CheckFlag(DWORD& Target, DWORD Flag);
void InvertFlag(DWORD& Target);

static DWORD _GetPrivateProfileString(LPCTSTR lpSection, LPCTSTR lpKey, 
       LPTSTR lpDefault, LPTSTR lpBuffer, DWORD dwBufSize, LPCTSTR szPathAndFile) 
{
   fstream clFileOp(szPathAndFile,ios::in);  // File
   string clline;                            // line buffer
   CStringA clkey( lpKey);                   // key in ansii

   if (clFileOp.is_open() == FALSE ) 
   {
      return 1;
   }
   while(!clFileOp.eof()) 
   {
      getline ( clFileOp, clline );
      if ( clline.find( clkey ) != -1 )
      {
         int nEquals = clline.find( "=" );
         if ( nEquals == -1 )
         {
            continue;
         }
         string clres = clline.substr( nEquals +1, clline.length() - nEquals );
         CString clret (  clres.c_str() );
         _tcscpy ( lpBuffer,clret );
         clFileOp.close();
         return 1;
      }
   }

   clFileOp.close();
   return 0;

}

CGUIDesktop *GetDesktop();

extern LOADEDTEXTURE* GUIImg;