#include "Includes.h"
#include "Main.h"

Application App; //Controls many native Windows-related functionality or other core functionality in the application.
Direct3D D3D; //Handles many areas of the application related to Direct3D.
DirectInput DI; //Handles user input using calls involving DirectInput.
CPlayer Player; //Handles many player variables.
CTextures Textures; //Holds the functions for managing loaded textures and lists.
CGravity Gravity; //Manages gravity-related variables and functions.
CMobMove MobMove; //Handles functionality of automatically moving monsters.
CUI UI; //Manages UI functionality.
CCollision Collision; //Handles collision-related functionality.
CAttacking Attacking; //Handles attacking-related logic and functionality.
HiResTimer Timer; //The high-precision timer manager class.

list <CPlatforms*> Platforms;
list <CMobs*> Mobs;
list <CBackgrounds*> Backgrounds;
list <CPortals*> Portals;
list <CPlatforms*> LoadedPlatforms;
list <CMobs*> LoadedMobs;
list <CBackgrounds*> LoadedBackgrounds;
list <CMaps*> LoadedMaps;
list <CPortals*> LoadedPortals;
list <CPlayer*> OtherPlayers;
list <CItems*> LoadedItems;
list <CItems*> Items;

LOADEDTEXTURE* GUIImg;

/////TEMPORARILY HERE
void InitializeGame()
{
	GUIImg = Textures.InitCore("img/GUIDesktop.png");
	LOADEDTEXTURE *GUIWindow = Textures.InitCore("img/GUIWindow.png");
	GetDesktop()->Background = GUIImg;
	GetDesktop()->Width = App.ClientWidth;
	GetDesktop()->Height = App.ClientHeight;
	CGUIWindow *lul = new CGUIWindow();
	lul->Background = GUIWindow;
	lul->X = 100;
	lul->Y = 100;
	lul->Width = 100;
	lul->Height = 100;
	GetDesktop()->AddChild(lul);

	Player.SetLT(Textures.InitCore ("img/0.png")); //Init temp player
	Textures.LoadItem(9001); // MONEYZ
	Textures.LoadPortal(1);
	Textures.LoadMap(1);
	Textures.LoadMap(2);
	Textures.SetMap(1);

	Player.ID = 1;
	Player.Level = 1;
	Player.SetPos(500, 100);

	Player.HP = 500;
	Player.MP = 100;
	Player.EXP = 100;
	Player.MaxHP = 10000;
	Player.MaxMP = 10000;
	Player.EXPNeeded = 105;

	UI.LoadBarBG();
	UI.LoadHPBar();
	UI.LoadMPBar();
	UI.LoadEXPBar();
	UI.LoadLevelBG();

	Attacking.Skills.StarSmash.Load();
}
/////TEMPORARILY HERE

//Entry point for the application.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	App.Initialize("Frost", 0.01f, 47, 1024, 768); // Calculate/set initial application values.
	App.InitializeWindow(); // Use gathered values to generate the main game window.
	//App.InitializeDebugWindow(); // Initialize the debug window, NOT FOR RELEASE USE!

	D3D.Initialize(true); // Creates an instance of D3D and initializes the device.
	D3D.InitializeFont(); // Only needed if using D3DXFont, creates the usable font.
	D3D.InitScene(); // Sets some render states' values and the projection settings.
	//D3D.UpdateCamera(); // Sets up viewpoint variable information. Needs to be called every time the view changes. Only for 3D, not needed.
	DI.Initialize(); // Currently only acquires the keyboard, that's all I need from DirectInput.

	/////TEMPORARILY HERE
	InitializeGame(); // Loads textures I'm using into lists.
	/////TEMPORARILY HERE

	App.InitializeWinsock();
	App.CreateClientSocket();
	App.ResolveHost();
	App.ConnectToServer();
	App.StartRecievingData();

	while (App.Running) // Run this loop while the game is still working fine.
	{
		if ( PeekMessage(&App.Msg, NULL, 0, 0, PM_REMOVE) ) // If there are native Windows messages.
		{
			TranslateMessage(&App.Msg); // Change WM_KEYDOWN messages to WM_CHAR and output.
			DispatchMessage(&App.Msg); // Send out any other messages.
		}

		// Update the global variable that holds the time passed this frame.
		App.TimeElapsedThisFrame = Timer.GetElapsedSeconds();
		// Update all the keyboard key checks for DirectInput.
		DI.ProcessInput();
		// Updates the camera view. 3D only, don't need.
		//D3D.UpdateCamera();
		// Draws the current frame.
		D3D.DrawScene();
	}

	// When the application is being unloaded, release memory resources in use.
	App.Unload();

	return EXIT_SUCCESS; //Exit with a exit code indicating success (0).
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Vital function to make a point out of two ints.
POINT MakePoint(int X, int Y)
{
	POINT Point;
	Point.x = X;
	Point.y = Y;
	return Point;
}

//Function to make a rect out of four ints.
RECT MakeRect(int Left, int Top, int Right, int Bottom)
{
	RECT Rect;
	Rect.top = Top;
	Rect.bottom = Bottom;
	Rect.left = Left;
	Rect.right = Right;
	return Rect;
}

//USE FOR SQL INJECTION CHECKS!
int strpos(char *haystack, char *needle)
{
   char *p = strstr(haystack, needle);
   if (p)
      return p - haystack;
   return -1;   // Not found = -1.
}

void HandleError(LPCSTR errorInstance)
{
	MessageBox(App.hWnd, errorInstance, "FAIL !!", MB_OK);
	TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
}

long GetPrivateProfileLong(__in  LPCTSTR lpAppName, __in  LPCTSTR lpKeyName, __in  long nDefault, __in  LPCTSTR lpFileName)
{
	char DefaultBuf[10];
	char ResultBuf[10];
	_ltoa(nDefault, DefaultBuf, 10);
	GetPrivateProfileStringA(lpAppName, lpKeyName, DefaultBuf, ResultBuf, sizeof(ResultBuf), lpFileName);

	if (strcmp(ResultBuf, DefaultBuf) == 0) //If the default value was returned, we have to handle this.
	{
		return nDefault;
	}
	else //Epic success, let's return what we wanted.
	{
		return atol(ResultBuf);
	}
}

//Default message handling, doesn't function as needed for DX apps though.
/*
while(GetMessage(&App.Msg, NULL, 0, 0) > 0)
{
	TranslateMessage(&App.Msg);
	DispatchMessage(&App.Msg);
}
*/

//Functions related to flag handling.
void SetFlag(DWORD& Target, DWORD Flag)
{
	Target = Target | Flag;
}

void UnsetFlag(DWORD& Target, DWORD Flag)
{
	Target = Target & ~Flag;
}

bool CheckFlag(DWORD& Target, DWORD Flag)
{
	return ( (Target & Flag) == Flag );
}

void InvertFlag(DWORD& Target)
{
	Target = ~Target;
}

//////////////////////////////////////////////////////////////

CGUIDesktop *GetDesktop()
{
	return UI.Desktop;
}

/*
char user[100];
char pass[100];
GetWindowText(userfield, user, 100);
GetWindowText(passfield, pass, 100);
LOGINPACKET *lp = new LOGINPACKET();
strcpy(lp->Username, user);
strcpy(lp->Password, pass);
send(App.ClientSocket, (char *)lp, sizeof(LOGINPACKET), 0);
while (LoginSuccess == 0)
{
	Sleep(500);
}
if (LoginSuccess == 1)
{
	MessageBox(NULL, "Login successful! Close this box by pressing the X button at the top.", "lol temp login", MB_OK);
}
*/