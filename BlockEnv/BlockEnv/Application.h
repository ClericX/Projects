#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <GL/gl.h>
#include "../Frost/String.h"
#include "Color.h"
#include "../Frost/Texture.h"
#include "../Frost/List.h"
#include "Block.h"
#include "Point.h"
#include "../Frost/Vector3f.h"

#define PI 3.1415926535f

namespace Application
{
	// Game window variables
	extern "C" HWND Window;
	extern "C" HDC WindowDC;
	extern "C" HGLRC WindowGLRC;
	extern "C" WNDCLASSEX WindowClass;
	extern "C" char WndClassName[100];
	extern "C" String WndTitle;
	extern "C" HINSTANCE hInstance;
	extern "C" MSG Msg;

	// Cursor variables
	extern "C" HCURSOR DefaultCursor;
	extern "C" HCURSOR NoCursor;
	extern "C" HCURSOR InGameCursor;

	// Version variables
	extern "C" float Version;
	extern "C" DWORD BuildNumber;

	extern "C" bool Running;
	extern "C" double RunTime;

	extern "C" double TimeElapsedThisFrame;

	extern "C" Texture TextTexture;

	// Screen/window variables
	extern "C" int ScreenWidth;
	extern "C" int ScreenHeight;
	extern "C" int BorderWidth;
	extern "C" int BorderHeight;
	extern "C" int CaptionHeight;
	extern "C" int Width;
	extern "C" int Height;
	extern "C" int ClientWidth;
	extern "C" int ClientHeight;
	extern "C" int WndX;
	extern "C" int WndY;

	// Client window variables
	extern "C" RECT ClientRect;
	extern "C" POINT WndTopLeft;
	extern "C" POINT WndBottomRight;

	extern "C" int CurrentDrawMode;

	extern "C" Vector3f CharPos;
	extern "C" Vector3f CamPos;
	extern "C" float CamPitch;
	extern "C" float CamYaw;

	//Winsock variables
	extern "C" SOCKET ClientSocket;
	extern "C" String ServerIP;
	extern "C" struct hostent *HostEntity;
	extern "C" struct sockaddr_in ClientSin;

	extern "C" unsigned int PlacementBlockID;

	void GoFullscreen();

	void Initialize(char *tTitle, float fVer, DWORD dBuild, int tWidth, int tHeight);
	void InitializeWindow();
	void InitializeOpenGL();
	void ReleaseOpenGL();
	void Unload();
	void WindowMessageLoop();
	int GetRand(int low, int high);

	//Winsock functions
	void InitializeWinsock();
	void CreateClientSocket();
	void ResolveHost();
	void ConnectToServer();
	void RecieveData();
	void StartRecievingData();

	void DrawScene();
	LRESULT CALLBACK WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void PacketParser(String *buffer);

	void PrintText(float X, float Y, String Text);
	void DrawQuad(float X, float Y, float Width, float Height, Color *clr, Texture *tex);

	void Move3DHorizontally(Vector3f &Location, float Direction, float Distance);
	void Move3DVertically(Vector3f &Location, float Direction, float Distance);
	void MoveCharacter(float dist, float dir);
	void MoveCharacterUp(float dist, float dir);
	void MoveCharacterWithCollision(float dist, float dir);
	void MoveCharacterUpWithCollision(float dist, float dir);
	void UpdateCamera();
	void ApplyCharacterMovement();
	void setDrawMode(int DrawMode);

	void SendPacket(String thePacket);
};