#pragma once

#include "Includes.h"

class Application
{
public:
	Application(void);
	~Application(void);

	//Functions
	void Initialize(LPCSTR tTitle, float fVer, DWORD dBuild, int tWidth, int tHeight);
	void InitializeWindow();
	void InitializeDebugWindow();
	void Unload();

	//Winsock functions
	void InitializeWinsock();
	void CreateClientSocket();
	void ResolveHost();
	void ConnectToServer();
	void RecieveData();
	void StartRecievingData();

	//Winsock variables
	char PacketBuffer[300];
	SOCKET ClientSocket;
	string ServerIP;
	struct hostent *HostEntity;
	struct sockaddr_in ClientSin;

	//Window procedure stuff
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	MSG Msg;

	//Screen/window variables
	int ScreenWidth;
	int ScreenHeight;
	int BorderWidth;
	int BorderHeight;
	int CaptionHeight;
	int Width;
	int Height;
	int ClientWidth;
	int ClientHeight;
	int WndX;
	int WndY;

	//Version variables
	float Version;
	char VersionStr[20];
	DWORD BuildNumber;
	char BuildStr[20];

	//App instances
	HINSTANCE hInstance;
	HINSTANCE hDebugInstance;

	//Game window variables
	HWND hWnd;
	char WndClassName[100];
	char WndTitle[100];

	//Debug window variables
	HWND DbghWnd;
	char DbgWndClassName[100];
	char DbgWndTitle[100];

	//Cursor variables
	HCURSOR DefaultCursor;
	HCURSOR NoCursor;
	HCURSOR InGameCursor;

	//Client window variables
	RECT ClientRect;
	POINT WndTopLeft;
	POINT WndBottomRight;

	//Cursor positions
	POINT CursorPos;
	POINT ClientCursorPos;

	//Timer variables
	float TimeElapsedThisFrame;

	//Other variables
	bool WndActive;
	bool Running;
	string CurrentDirectory;

	//Functions with other usefulness
	int GetRand(int low, int high);
	char *GetCurrentPath(char* buffer);
};