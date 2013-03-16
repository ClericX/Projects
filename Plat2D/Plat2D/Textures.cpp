#include "Textures.h"
#include "Main.h"

CTextures::CTextures(void)
{
}

CTextures::~CTextures(void)
{
}

LOADEDTEXTURE* CTextures::InitCore(string Filename)
{
    D3DSURFACE_DESC surfaceDesc;
    LOADEDTEXTURE* newTexture;

    //Convert filename to lowercase letters
    //Filename = _strlwr((char *)Filename.c_str ());

    //Texture was not in the list, make a new texture
    newTexture = new LOADEDTEXTURE;

    //Load texture from file
    newTexture->Texture = D3D.LoadTexture (newTexture, (char*)Filename.c_str());

    //Make sure texture was loaded
    if (!newTexture->Texture)
	{
		string MessageContent;
		MessageContent = "Error loading texture into struct!\nTarget File: ";
		MessageContent += Filename;
		MessageBox(App.hWnd, MessageContent.c_str(), "Frost - Error", MB_OK);
		return NULL;
	}

    //Get texture dimensions
    newTexture->Texture->GetLevelDesc(0, &surfaceDesc);

    //Set new texture parameters
    newTexture->referenceCount = 1;
    newTexture->Filename = Filename;
    newTexture->Width = surfaceDesc.Width;
    newTexture->Height = surfaceDesc.Height;

	return newTexture;
}

void CTextures::InitPlatform(unsigned int ID, int Instance, int X, int Y)
{
	//Create and push new texture onto list of platforms specified.

	CPlatforms* Plat = new CPlatforms();
	Plat->Instance = Instance;
	Plat->X = X;
	Plat->Y = Y;
	//Plat->LT = InitCore(Filename);
	//Push back the item into the Platforms list.
	Platforms.push_back( Plat );
}

void CTextures::InitMob(unsigned int ID, int Instance, int X, int Y)
{
	//Create and push new texture onto list of mobs specified.
	CMobs* Mob = new CMobs();
	(*Mob) = (*GetLoadedMob(ID));
	Mob->Instance = Instance;
	Mob->X = X;
	Mob->Y = Y;
	//Push back the item into the Platforms list.
	Mobs.push_back( Mob );
}

//Load texture from file || THIS FUNCTION IS NOT REALLY USED AS THERE USUALLY ISNT A DIRECT LIST TO LOADEDTEXTURE *'s SPECIFIED. LEAVE AS GUIDELINE.
void CTextures::InitLT (list<LOADEDTEXTURE*> &lt, string Filename)
{
    D3DSURFACE_DESC surfaceDesc;
    LOADEDTEXTURE* newTexture;

    //Convert filename to lowercase letters
    Filename = _strlwr((char *)Filename.c_str ());

    //Texture was not in the list, make a new texture
    newTexture = new LOADEDTEXTURE;

    //Load texture from file
    newTexture->Texture = D3D.LoadTexture (newTexture, (char*)Filename.c_str());

    //Make sure texture was loaded
    if (!newTexture->Texture)
	{
		MessageBox(App.hWnd, "Error loading texture into struct!", "Frost - Error", MB_OK);
	}

    //Get texture dimensions
    newTexture->Texture->GetLevelDesc(0, &surfaceDesc);

    //Set new texture parameters
    newTexture->referenceCount = 1;
    newTexture->Filename = Filename;
    newTexture->Width = surfaceDesc.Width;
    newTexture->Height = surfaceDesc.Height;

    //Push new texture onto list
    lt.push_back (newTexture);

    //Setup current texture instance
	//The way I use this function, I don't use this line.
    //texture = lt.back();
}

CPlatforms* CTextures::GetPlatform(unsigned int PlatformID, int Instance)
{
    list<CPlatforms*>::iterator itTextures;

    //Check if texture is in the loaded list
    for (itTextures = Platforms.begin (); itTextures != Platforms.end (); itTextures++)
	{
        if ((*itTextures)->PlatformID == PlatformID && (*itTextures)->Instance == Instance)
        {   
            //Get CPlatforms object
            return (*itTextures);
        }
	}

	return NULL;
}

CMobs* CTextures::GetMob(unsigned int ID, int Instance)
{
    list<CMobs*>::iterator itTextures;

    //Check if texture is in the loaded list
    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
        if ((*itTextures)->MobID == ID && (*itTextures)->Instance == Instance)
        {   
            //Get CMobs object
            return (*itTextures);
        }
	}

	return NULL;
}

//Iterate through a list and find a loaded texture pointer
LOADEDTEXTURE* CTextures::GetLT(list<LOADEDTEXTURE*> lt, string Filename)
{
    list<LOADEDTEXTURE*>::iterator itTextures;

    //Convert filename to lowercase letters
    Filename = _strlwr((char *)Filename.c_str ());

    //Check if texture is in the loaded list
    for (itTextures = lt.begin (); itTextures != lt.end (); itTextures++)
	{
        if ((*itTextures)->Filename == Filename)
        {   
            //Get LOADEDTEXTURE object
            return (*itTextures);
        }
	}

	return NULL;
}

//Unload a texture
void CTextures::Close(LOADEDTEXTURE* lt)
{    
    //Decrement reference counter and nullify pointer
    lt->referenceCount--;
    lt = NULL;
}

//Release all unreferenced textures
void CTextures::GarbageCollect(list<LOADEDTEXTURE*> lt)
{
    list<LOADEDTEXTURE*>::iterator it;
    list<LOADEDTEXTURE*>::iterator itNext;

    //Go through loaded texture list
    for (it = lt.begin(); it != lt.end ();)   
        if ((*it)->referenceCount <= 0)
        {
            //Get next iterator
            itNext = it;
            itNext++;

            //Release texture
            if ((*it)->Texture)
                (*it)->Texture->Release();
            (*it)->Texture = NULL;

            //Delete LOADEDTEXTURE object
            delete (*it);
            lt.erase (it);

            //Move to next element
            it = itNext;
        } else it++; //Increment iterator

    //Successfully released unreferenced textures
}

//Release all textures
void CTextures::CleanupTextures(list<LOADEDTEXTURE*> lt)
{
    list<LOADEDTEXTURE*>::iterator it;
 
     //Go through loaded texture list
    for (it = lt.begin(); it != lt.end (); it++)
    {
        //Release texture
        if ((*it)->Texture)
            (*it)->Texture->Release();
        (*it)->Texture = NULL;
        
         //Delete LOADEDTEXTURE object
        delete (*it);
    }

    //Clear list
    lt.clear ();

    //Successfully released all textures
}

void CTextures::DrawAllBackgrounds()
{
    list<CBackgrounds*>::iterator itTextures;

    for (itTextures = Backgrounds.begin (); itTextures != Backgrounds.end (); itTextures++)
	{
		(*itTextures)->Draw(DRAW_WORLD);
	}
}

void CTextures::DrawAllPlatforms()
{
    list<CPlatforms*>::iterator itTextures;

    for (itTextures = Platforms.begin (); itTextures != Platforms.end (); itTextures++)
	{
		(*itTextures)->Draw(DRAW_WORLD);
	}
}

void CTextures::DrawAllMobs()
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
		(*itTextures)->Draw(DRAW_WORLD);
	}
}

void CTextures::DrawAllPortals()
{
	list<CPortals*>::iterator it;

	for (it = Portals.begin(); it != Portals.end(); it++)
	{
		(*it)->Draw(DRAW_WORLD);
	}
}

void CTextures::DrawAllOtherPlayers()
{
	list <CPlayer*>::iterator it;

	for (it = OtherPlayers.begin(); it != OtherPlayers.end(); it++)
	{
		(*it)->Draw(DRAW_WORLD);
	}
}

void CTextures::DrawAllItems()
{
	list <CItems*>::iterator it;

	for (it = Items.begin(); it != Items.end(); it++)
	{
		(*it)->Draw(DRAW_WORLD);
	}
}

void CTextures::GravityAllMobs()
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
		Gravity.MobGravityCheck( (*itTextures) );
	}
}

void CTextures::AutoMoveAllMobs()
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
		MobMove.ApplyMovements( (*itTextures) );
	}
}

void CTextures::DetectAttacksOnAllMobs(CMapObject *Obj)
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
		(*itTextures)->Attacked(Obj);
	}
}

void CTextures::DrawAllMobStats()
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
		(*itTextures)->DrawStats();
	}
}

void CTextures::CheckMobsForDamage()
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = Mobs.begin (); itTextures != Mobs.end (); itTextures++)
	{
		if ( (*itTextures)->Dead == false )
		{
			//if (Collision.PixelToPixel(Player.LT->Texture, (*itTextures)->LT->Texture, Player.GetRect(), (*itTextures)->GetRect()))
			if (Collision.RectToRect(Player.GetRect(), (*itTextures)->GetRect()))
			{
				Player.ChangeHP(2, true, false);
			}
		}
	}
}

bool CTextures::CheckForPortalEntrance()
{
	list <CPortals*>::iterator it;

	for (it = Portals.begin(); it != Portals.end(); it++)
	{
		if (Collision.RectToRect((*it)->GetRect(), Player.GetRect()))
		{
			ChangeMap((*it)->TargetMapID);
			return true;
		}
	}

	return false;
}

CPlayer *CTextures::GetPlayerByID(unsigned int ID)
{
	list <CPlayer*>::iterator it;

	for (it = OtherPlayers.begin(); it != OtherPlayers.end(); it++)
	{
		if ( (*it)->ID = ID )
		{
			return (*it);
		}
	}

	return 0;
}

PixelColor CTextures::GetTexturePixelColor(CMapObject *Obj, long X, long Y, DWORD Flags, D3DLOCKED_RECT *lr)
{
	D3DLOCKED_RECT templr;
	PixelColor pixel;
	PixelColor* surface;
	int pitch;
	long x = X;
	long y = Y;

	if (CheckFlag(Flags, FROST_READPIXEL_SCREENCOORDS))
	{
		x -= Obj->X;
		y -= Obj->Y;
	}

	if (CheckFlag(Flags, FROST_READPIXEL_LOCK))
	{
		if (lr == NULL)
		{
			Obj->LT->Texture->LockRect(0, &templr, 0, D3DLOCK_READONLY);
		}
		else
		{
			Obj->LT->Texture->LockRect(0, lr, 0, D3DLOCK_READONLY);
		}
	}

	if (CheckFlag(Flags, FROST_READPIXEL_READ))
	{
		//Acquire the pointer to the start of surface memory.
		//The number of DWORDS per line should be bytecount/4.
		if (lr == NULL)
		{
			surface = (PixelColor *)templr.pBits;
			pitch = templr.Pitch / sizeof(DWORD);
		}
		else
		{
			surface = (PixelColor *)lr->pBits;
			pitch = lr->Pitch / sizeof(DWORD);
		}

		//Get the precise selected pixel's color information.
		if (CheckFlag(Flags, FROST_READPIXEL_CHECKBOUNDS))
		{
			if ( X < Obj->X || Y < Obj->Y || X > (Obj->X + Obj->GetRect().width()) || Y > (Obj->Y + Obj->GetRect().height()) )
			{
				PixelColor tmp;
				tmp.A = 0;
				tmp.R = 0;
				tmp.G = 0;
				tmp.B = 0;
				return tmp;
			}
			else
			{
				pixel = surface[ ( y * pitch ) + x ];
			}
		}
		else
		{
			pixel = surface[ ( y * pitch ) + x ];
		}
		//pixel = *( surface + ( ( y * pitch ) + x ) );

		/*
		BYTE *bytePointer=(BYTE*)lr->pBits;
		DWORD index=(x*4+(y*(lr->Pitch)));

		// Blue
		BYTE b=bytePointer[index];

		// Green
		BYTE g=bytePointer[index+1];

		// Red
		BYTE r=bytePointer[index+2];

		// Alpha
		BYTE a=bytePointer[index+3];

		pixel.A = a;
		pixel.R = r;
		pixel.G = g;
		pixel.B = b;
		*/
	}

	if (CheckFlag(Flags, FROST_READPIXEL_UNLOCK))
	{
		Obj->LT->Texture->UnlockRect(0);
	}

	return pixel;
}

void CTextures::LoadMob(unsigned int ID)
{
	CMobs *Mob = new CMobs();
	
	char MobNameBuffer[50];
	char SettingsPath[100];
	char IDString[10];
	_itoa(ID, IDString, 10);
	strcpy(SettingsPath, App.CurrentDirectory.c_str());
	strcat(SettingsPath, MobDir);
	strcat(SettingsPath, IDString);
	strcat(SettingsPath, "/");
	strcat(SettingsPath, IDString);
	strcat(SettingsPath, INIExtentionStr);
	
	GetPrivateProfileStringA("General", "MobName", "NULL", MobNameBuffer, sizeof(MobNameBuffer), SettingsPath);

	//Specify monster stats
	Mob->MobName = MobNameBuffer;
	Mob->MobID = GetPrivateProfileIntA(MobNameBuffer, "MobID", 0, SettingsPath);

	Mob->Accuracy = GetPrivateProfileIntA(MobNameBuffer, "Accuracy", 0, SettingsPath);
	Mob->MaxHP = GetPrivateProfileIntA(MobNameBuffer, "MaxHP", 0, SettingsPath);
	Mob->MaxMP = GetPrivateProfileIntA(MobNameBuffer, "MaxMP", 0, SettingsPath);
	Mob->EXPGiven = GetPrivateProfileIntA(MobNameBuffer, "EXPGiven", 0, SettingsPath);
	Mob->CanFallOffPlatforms = GetPrivateProfileIntA(MobNameBuffer, "CanFallOffPlatforms", 0, SettingsPath);
	
	Mob->HP = Mob->MaxHP;
	Mob->MP = Mob->MaxMP;
	
	//Indicate number of frames per category
	int DieFrames = 0;
	int HitFrames = 0;
	int MoveFrames = 0;
	int StandFrames = 0;
	
	vector<unsigned int> DieDelays;
	vector<unsigned int> HitDelays;
	vector<unsigned int> MoveDelays;
	vector<unsigned int> StandDelays;

	Mob->MobFrames = new MOBFRAMESTRUCT();

	//Specify delays between frames (and also indirectly number of frames)
	//Load Die Delays
	for (int Die = 0; Die >= -1; Die++)
	{
		char ITOABuf[10];
		char DelayNameBuffer[20];
		strcpy(DelayNameBuffer, "Delay");
		strcat(DelayNameBuffer, _itoa(Die, ITOABuf, 10));
		UINT DelayValue = GetPrivateProfileIntA("DieDelays", DelayNameBuffer, 9001, SettingsPath);
		
		if (DelayValue == 9001)
		{
			Die = -5;
			DieFrames--;
		}
		else
		{
			DieFrames++;
			DieDelays.push_back(DelayValue);
		}
	}

	//Load Hit Delays
	for (int Hit = 0; Hit >= -1; Hit++)
	{
		char ITOABuf[10];
		char DelayNameBuffer[20];
		strcpy(DelayNameBuffer, "Delay");
		strcat(DelayNameBuffer, _itoa(Hit, ITOABuf, 10));
		UINT DelayValue = GetPrivateProfileIntA("HitDelays", DelayNameBuffer, 9001, SettingsPath);
		
		if (DelayValue == 9001)
		{
			Hit = -5;
		}
		else
		{
			HitFrames++;
			HitDelays.push_back(DelayValue);
		}
	}

	//Load Move Delays
	for (int Move = 0; Move >= -1; Move++)
	{
		char ITOABuf[10];
		char DelayNameBuffer[20];
		strcpy(DelayNameBuffer, "Delay");
		strcat(DelayNameBuffer, _itoa(Move, ITOABuf, 10));
		UINT DelayValue = GetPrivateProfileIntA("MoveDelays", DelayNameBuffer, 9001, SettingsPath);
		
		if (DelayValue == 9001)
		{
			Move = -5;
		}
		else
		{
			MoveFrames++;
			MoveDelays.push_back(DelayValue);
		}
	}

	//Load Stand Delays
	for (int Stand = 0; Stand >= -1; Stand++)
	{
		char ITOABuf[10];
		char DelayNameBuffer[20];
		strcpy(DelayNameBuffer, "Delay");
		strcat(DelayNameBuffer, _itoa(Stand, ITOABuf, 10));
		UINT DelayValue = GetPrivateProfileIntA("StandDelays", DelayNameBuffer, 9001, SettingsPath);
		
		if (DelayValue == 9001)
		{
			Stand = -5;
		}
		else
		{
			StandFrames++;
			StandDelays.push_back(DelayValue);
		}
	}

	//Make the full strings for folder paths
	string DieDir;
	string HitDir;
	string MoveDir;
	string StandDir;
	string File;
	
	//Insert the base dir into each sub dir
	DieDir += MobDir;
	DieDir += IDString;
	DieDir += "/";
	HitDir += MobDir;
	HitDir += IDString;
	HitDir += "/";
	MoveDir += MobDir;
	MoveDir += IDString;
	MoveDir += "/";
	StandDir += MobDir;
	StandDir += IDString;
	StandDir += "/";
	
	//Add sub dir ending for each sub dir
	DieDir += "Die/";
	HitDir += "Hit/";
	MoveDir += "Move/";
	StandDir += "Stand/";
	
	char buf[10];
	
	//Load each frame in the die animation
	for (int i = 0; i <= DieFrames; i++)
	{
		File = DieDir;

		//Create a new instance of the LOADEDFRAME struct
		LOADEDFRAME* newFrame = new LOADEDFRAME;
		
		//Get the filename and complete the path
		File += _itoa(i, buf, 10);
		File += PNGExtentionStr;
		
		//Load the texture
		newFrame->LT = Textures.InitCore( File.c_str() );
		//Load the appropriate delay
		newFrame->Delay = DieDelays[i];
		
		Mob->MobFrames->Die.push_back(newFrame);
	}
	
	//Load each frame in the hit animation
	for (int i = 0; i < HitFrames; i++)
	{
		File = HitDir;

		//Create a new instance of the LOADEDFRAME struct
		LOADEDFRAME* newFrame = new LOADEDFRAME;
		
		//Get the filename and complete the path
		File += _itoa(i, buf, 10);
		File += PNGExtentionStr;
		
		//Load the texture
		newFrame->LT = Textures.InitCore( File.c_str() );
		//Load the appropriate delay
		newFrame->Delay = HitDelays[i];
		
		Mob->MobFrames->Hit.push_back(newFrame);
	}
	
	//Load each frame in the move animation
	for (int i = 0; i < MoveFrames; i++)
	{
		File = MoveDir;

		//Create a new instance of the LOADEDFRAME struct
		LOADEDFRAME* newFrame = new LOADEDFRAME;
		
		//Get the filename and complete the path
		File += _itoa(i, buf, 10);
		File += PNGExtentionStr;
		
		//Load the texture
		newFrame->LT = Textures.InitCore( File.c_str() );
		//Load the appropriate delay
		newFrame->Delay = MoveDelays[i];
		
		Mob->MobFrames->Move.push_back(newFrame);
	}
	
	//Load each frame in the stand animation
	for (int i = 0; i < StandFrames; i++)
	{
		File = StandDir;

		//Create a new instance of the LOADEDFRAME struct
		LOADEDFRAME* newFrame = new LOADEDFRAME;
		
		//Get the filename and complete the path
		File += _itoa(i, buf, 10);
		File += PNGExtentionStr;
		
		//Load the texture
		newFrame->LT = Textures.InitCore( File.c_str() );
		//Load the appropriate delay
		newFrame->Delay = StandDelays[i];
		
		Mob->MobFrames->Stand.push_back(newFrame);
	}

	Mob->LT = Mob->MobFrames->Stand[0]->LT;
	//Mob->Color = 0x11111111;
	
	LoadedMobs.push_back( Mob );
}

CMobs *CTextures::GetLoadedMob(unsigned int ID)
{
    list<CMobs*>::iterator itTextures;

    for (itTextures = LoadedMobs.begin (); itTextures != LoadedMobs.end (); itTextures++)
	{
		if ( (*itTextures)->MobID == ID )
		{
			return (*itTextures);
		}
	}

	return NULL;
}

void CTextures::LoadMap(unsigned int ID)
{
	CMaps *Map = new CMaps();
	
	char MapNameBuffer[50];
	char BackgroundSettingsPath[150];
	char PlatformSettingsPath[150];
	char MobSettingsPath[150];
	char PortalSettingsPath[150];
	char IDString[10];
	_itoa(ID, IDString, 10);
	strcpy(BackgroundSettingsPath, App.CurrentDirectory.c_str());
	strcat(BackgroundSettingsPath, MapDir);
	strcat(BackgroundSettingsPath, IDString);
	strcat(BackgroundSettingsPath, "/");
	strcpy(PlatformSettingsPath, BackgroundSettingsPath);
	strcpy(MobSettingsPath, BackgroundSettingsPath);
	strcpy(PortalSettingsPath, BackgroundSettingsPath);
	strcat(BackgroundSettingsPath, "Background");
	strcat(BackgroundSettingsPath, INIExtentionStr);
	strcat(PlatformSettingsPath, "Platform");
	strcat(PlatformSettingsPath, INIExtentionStr);
	strcat(MobSettingsPath, "Mob");
	strcat(MobSettingsPath, INIExtentionStr);
	strcat(PortalSettingsPath, "Portal");
	strcat(PortalSettingsPath, INIExtentionStr);
	
	GetPrivateProfileStringA("General", "MapName", "NULL", MapNameBuffer, sizeof(MapNameBuffer), PlatformSettingsPath);

	Map->MapName = MapNameBuffer;
	Map->MapID = ID;

	Map->Bounds.top = GetPrivateProfileLong("Bounds", "Top", 9001, PlatformSettingsPath);
	Map->Bounds.bottom = GetPrivateProfileLong("Bounds", "Bottom", 9001, PlatformSettingsPath);
	Map->Bounds.left = GetPrivateProfileLong("Bounds", "Left", 9001, PlatformSettingsPath);
	Map->Bounds.right = GetPrivateProfileLong("Bounds", "Right", 9001, PlatformSettingsPath);

	char PlatformNameBuffer[15];
	char BackgroundNameBuffer[15];
	char MobNameBuffer[15];
	char PortalNameBuffer[15];
	char ITOABuf[10];
	unsigned int PlatformVerificationValue = 0;
	unsigned int BackgroundVerificationValue = 0;
	unsigned int MobVerificationValue = 0;
	unsigned int PortalVerificationValue = 0;

	//Get the number of platforms.
	while (PlatformVerificationValue != 9001)
	{
		strcpy(PlatformNameBuffer, "Platform");
		strcat(PlatformNameBuffer, _itoa(Map->NumberOfPlatforms, ITOABuf, 10));
		PlatformVerificationValue = GetPrivateProfileInt(PlatformNameBuffer, "PosX", 9001, PlatformSettingsPath);

		if (PlatformVerificationValue != 9001)
		{
			Map->NumberOfPlatforms++;
		}
	}

	//Get the number of backgrounds.
	while (BackgroundVerificationValue != 9001)
	{
		strcpy(BackgroundNameBuffer, "Background");
		strcat(BackgroundNameBuffer, _itoa(Map->NumberOfBackgrounds, ITOABuf, 10));
		BackgroundVerificationValue = GetPrivateProfileInt(BackgroundNameBuffer, "PosX", 9001, BackgroundSettingsPath);

		if (BackgroundVerificationValue != 9001)
		{
			Map->NumberOfBackgrounds++;
		}
	}

	//Get the number of mobs.
	while (MobVerificationValue != 9001)
	{
		strcpy(MobNameBuffer, "Mob");
		strcat(MobNameBuffer, _itoa(Map->NumberOfMobs, ITOABuf, 10));
		MobVerificationValue = GetPrivateProfileInt(MobNameBuffer, "PosX", 9001, MobSettingsPath);

		if (MobVerificationValue != 9001)
		{
			Map->NumberOfMobs++;
		}
	}

	//Get the number of portals.
	while (PortalVerificationValue != 9001)
	{
		strcpy(PortalNameBuffer, "Portal");
		strcat(PortalNameBuffer, _itoa(Map->NumberOfPortals, ITOABuf, 10));
		PortalVerificationValue = GetPrivateProfileInt(PortalNameBuffer, "PosX", 9001, PortalSettingsPath);

		if (PortalVerificationValue != 9001)
		{
			Map->NumberOfPortals++;
		}
	}

	for (unsigned int i = 0; i < Map->NumberOfPlatforms; i++)
	{
		strcpy(PlatformNameBuffer, "Platform");
		strcat(PlatformNameBuffer, _itoa(i, ITOABuf, 10));
		unsigned int CurrentPlatformID = GetPrivateProfileInt(PlatformNameBuffer, "PlatformID", 9001, PlatformSettingsPath);
		long CurrentPosX = GetPrivateProfileLong(PlatformNameBuffer, "PosX", 9001, PlatformSettingsPath);
		long CurrentPosY = GetPrivateProfileLong(PlatformNameBuffer, "PosY", 9001, PlatformSettingsPath);

		MAPOBJECT *NewPlatform = new MAPOBJECT;

		NewPlatform->ID = CurrentPlatformID;
		NewPlatform->X = CurrentPosX;
		NewPlatform->Y = CurrentPosY;

		Map->Platforms.push_back ( NewPlatform );
	}

	for (unsigned int i = 0; i < Map->NumberOfBackgrounds; i++)
	{
		strcpy(BackgroundNameBuffer, "Background");
		strcat(BackgroundNameBuffer, _itoa(i, ITOABuf, 10));
		unsigned int CurrentBackgroundID = GetPrivateProfileInt(BackgroundNameBuffer, "BackgroundID", 9001, BackgroundSettingsPath);
		long CurrentPosX = GetPrivateProfileLong(BackgroundNameBuffer, "PosX", 9001, BackgroundSettingsPath);
		long CurrentPosY = GetPrivateProfileLong(BackgroundNameBuffer, "PosY", 9001, BackgroundSettingsPath);

		MAPOBJECT *NewBackground = new MAPOBJECT;

		NewBackground->ID = CurrentBackgroundID;
		NewBackground->X = CurrentPosX;
		NewBackground->Y = CurrentPosY;

		Map->Backgrounds.push_back ( NewBackground );
	}

	for (unsigned int i = 0; i < Map->NumberOfMobs; i++)
	{
		strcpy(MobNameBuffer, "Mob");
		strcat(MobNameBuffer, _itoa(i, ITOABuf, 10));
		unsigned int CurrentMobID = GetPrivateProfileInt(MobNameBuffer, "MobID", 9001, MobSettingsPath);
		long CurrentPosX = GetPrivateProfileLong(MobNameBuffer, "PosX", 9001, MobSettingsPath);
		long CurrentPosY = GetPrivateProfileLong(MobNameBuffer, "PosY", 9001, MobSettingsPath);

		MAPOBJECT *NewMob = new MAPOBJECT;

		NewMob->ID = CurrentMobID;
		NewMob->X = CurrentPosX;
		NewMob->Y = CurrentPosY;

		Map->Mobs.push_back ( NewMob );
	}

	for (unsigned int i = 0; i < Map->NumberOfPortals; i++)
	{
		strcpy(PortalNameBuffer, "Portal");
		strcat(PortalNameBuffer, _itoa(i, ITOABuf, 10));
		unsigned int CurrentPortalID = GetPrivateProfileInt(PortalNameBuffer, "PortalID", 9001, PortalSettingsPath);
		long CurrentPosX = GetPrivateProfileLong(PortalNameBuffer, "PosX", 9001, PortalSettingsPath);
		long CurrentPosY = GetPrivateProfileLong(PortalNameBuffer, "PosY", 9001, PortalSettingsPath);
		unsigned int TargetMapID = GetPrivateProfileInt(PortalNameBuffer, "TargetMapID", 9001, PortalSettingsPath);

		MAPOBJECT *NewPortal = new MAPOBJECT;

		NewPortal->ID = CurrentPortalID;
		NewPortal->X = CurrentPosX;
		NewPortal->Y = CurrentPosY;
		NewPortal->TargetMapID = TargetMapID;

		Map->Portals.push_back ( NewPortal );
	}
	
	LoadedMaps.push_back( Map );
}

CMaps *CTextures::GetLoadedMap(unsigned int ID)
{
    list<CMaps*>::iterator itTextures;

    for (itTextures = LoadedMaps.begin (); itTextures != LoadedMaps.end (); itTextures++)
	{
		if ( (*itTextures)->MapID == ID )
		{
			return (*itTextures);
		}
	}

	return NULL;
}

void CTextures::SetMap(unsigned int ID)
{
	CMaps *Map = GetLoadedMap(ID);

	for (unsigned int i = 0; i < Map->NumberOfPlatforms; i++)
	{
		CPlatforms *CurrentPlatform = GetLoadedPlatform(ID, Map->Platforms[i]->ID);

		if (CurrentPlatform == NULL)
		{
			LoadPlatform(ID, Map->Platforms[i]->ID);
			CurrentPlatform = GetLoadedPlatform(ID, Map->Platforms[i]->ID);
		}

		CPlatforms *NewPlatform = new CPlatforms();

		(*NewPlatform) = (*CurrentPlatform);
		NewPlatform->X = Map->Platforms[i]->X;
		NewPlatform->Y = Map->Platforms[i]->Y;

		Platforms.push_back( NewPlatform );
	}

	for (unsigned int i = 0; i < Map->NumberOfBackgrounds; i++)
	{
		CBackgrounds *CurrentBackground = GetLoadedBackground(ID, Map->Backgrounds[i]->ID);

		if (CurrentBackground == NULL)
		{
			LoadBackground(ID, Map->Backgrounds[i]->ID);
			CurrentBackground = GetLoadedBackground(ID, Map->Backgrounds[i]->ID);
		}

		CBackgrounds *NewBackground = new CBackgrounds();

		(*NewBackground) = (*CurrentBackground);
		NewBackground->X = Map->Backgrounds[i]->X;
		NewBackground->Y = Map->Backgrounds[i]->Y;

		Backgrounds.push_back( NewBackground );
	}

	for (unsigned int i = 0; i < Map->NumberOfMobs; i++)
	{
		CMobs *CurrentMob = GetLoadedMob(Map->Mobs[i]->ID);

		if (CurrentMob == NULL)
		{
			LoadMob(Map->Mobs[i]->ID);
			CurrentMob = GetLoadedMob(Map->Mobs[i]->ID);
		}

		CMobs *NewMob = new CMobs();

		(*NewMob) = (*CurrentMob);
		NewMob->X = Map->Mobs[i]->X;
		NewMob->Y = Map->Mobs[i]->Y;

		NewMob->Instance = i; //Temporary, might be changed. Doesn't look like it needs to be though.

		Mobs.push_back( NewMob );
	}

	for (unsigned int i = 0; i < Map->NumberOfPortals; i++)
	{
		CPortals *CurrentPortal = GetLoadedPortal(Map->Portals[i]->ID);

		if (CurrentPortal == NULL)
		{
			LoadPortal(Map->Portals[i]->ID);
			CurrentPortal = GetLoadedPortal(Map->Portals[i]->ID);
		}

		CPortals *NewPortal = new CPortals();

		(*NewPortal) = (*CurrentPortal);
		NewPortal->PortalID = Map->Portals[i]->ID;
		NewPortal->X = Map->Portals[i]->X;
		NewPortal->Y = Map->Portals[i]->Y;
		NewPortal->TargetMapID = Map->Portals[i]->TargetMapID;

		Portals.push_back( NewPortal );
	}

	Player.CurrentMapID = ID;
}

void CTextures::LoadPlatform(unsigned int MapID, unsigned int PlatformID)
{
	CPlatforms *Platform = new CPlatforms();

	char MapIDString[10];
	char PlatformIDString[10];
	_itoa(MapID, MapIDString, 10);
	_itoa(PlatformID, PlatformIDString, 10);

	string Path;

	Path = MapDir;
	Path += MapIDString;
	Path += "/";
	Path += "Platform";
	Path += "/";
	Path += PlatformIDString;
	Path += PNGExtentionStr;

	Platform->MapID = MapID;
	Platform->PlatformID = PlatformID;

	Platform->SetLT( InitCore( Path ) );
	
	LoadedPlatforms.push_back( Platform );
}

CPlatforms *CTextures::GetLoadedPlatform(unsigned int MapID, unsigned int PlatformID)
{
    list<CPlatforms*>::iterator itTextures;

    for (itTextures = LoadedPlatforms.begin (); itTextures != LoadedPlatforms.end (); itTextures++)
	{
		if ( (*itTextures)->MapID == MapID && (*itTextures)->PlatformID == PlatformID )
		{
			return (*itTextures);
		}
	}

	return NULL;
}

void CTextures::LoadBackground(unsigned int MapID, unsigned int BackgroundID)
{
	CBackgrounds *Background = new CBackgrounds();

	char MapIDString[10];
	char BackgroundIDString[10];
	_itoa(MapID, MapIDString, 10);
	_itoa(BackgroundID, BackgroundIDString, 10);

	string Path;

	Path = MapDir;
	Path += MapIDString;
	Path += "/";
	Path += "Background";
	Path += "/";
	Path += BackgroundIDString;
	Path += PNGExtentionStr;

	Background->MapID = MapID;
	Background->BackgroundID = BackgroundID;

	Background->SetLT( InitCore( Path ) );
	
	LoadedBackgrounds.push_back( Background );
}

CBackgrounds *CTextures::GetLoadedBackground(unsigned int MapID, unsigned int BackgroundID)
{
    list<CBackgrounds*>::iterator itTextures;

    for (itTextures = LoadedBackgrounds.begin (); itTextures != LoadedBackgrounds.end (); itTextures++)
	{
		if ( (*itTextures)->MapID == MapID && (*itTextures)->BackgroundID == BackgroundID )
		{
			return (*itTextures);
		}
	}

	return NULL;
}

void CTextures::LoadPortal(unsigned int ID)
{
	CPortals *Portal = new CPortals();

	char PortalIDString[10];
	_itoa(ID, PortalIDString, 10);

	string Path;

	Path = MapDir;
	Path += "Portal/";
	Path += PortalIDString;
	Path += "/";
	Path += "0";
	Path += PNGExtentionStr;

	Portal->PortalID = ID;
	Portal->SetLT( InitCore( Path ) );
	
	LoadedPortals.push_back( Portal );
}

CPortals *CTextures::GetLoadedPortal(unsigned int ID)
{
    list<CPortals*>::iterator it;

    for (it = LoadedPortals.begin (); it != LoadedPortals.end (); it++)
	{
		if ( (*it)->PortalID == ID )
		{
			return (*it);
		}
	}

	return NULL;
}

void CTextures::LoadItem(unsigned int ID)
{
	CItems *Item = new CItems();

	char ItemIDString[10];
	_itoa(ID, ItemIDString, 10);

	string Path;

	Path = App.CurrentDirectory;
	Path += ItemDir;
	Path += ItemIDString;
	Path += "/";
	Path += "0";
	Path += PNGExtentionStr;

	Item->ItemID = ID;
	Item->SetLT( InitCore( Path ) );
	
	LoadedItems.push_back( Item );
}

CItems *CTextures::GetLoadedItem(unsigned int ID)
{
    list<CItems*>::iterator it;

    for (it = LoadedItems.begin (); it != LoadedItems.end (); it++)
	{
		if ( (*it)->ItemID == ID )
		{
			return (*it);
		}
	}

	return NULL;
}

void CTextures::ClearMap()
{
	Platforms.clear();
	Mobs.clear();
	Backgrounds.clear();
	Portals.clear();
	Items.clear();
}

void CTextures::ChangeMap(unsigned int ID)
{
	ClearMap();
	SetMap(ID);
}