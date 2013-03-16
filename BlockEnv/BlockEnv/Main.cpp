#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/gl.h>
#include <math.h>

//#include "vector3f.h"
//#include "matrix4x4f.h"
#include "HiResTimer.h"
#include "String.h"

#include "Application.h"
#include "GUIWindow.h"
#include "Flags.h"
#include "Input.h"

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
int main()
{
	SetWindowTextA(GetConsoleWindow(), "BlockEnv Debug");

	// Initialize everything we need.
	Application::Initialize("BlockEnv", 0.01f, 1, 800, 600);
	Application::InitializeWindow();
	Application::InitializeOpenGL();
	Input::Initialize();

	Application::InitializeWinsock();
	Application::CreateClientSocket();
	Application::ResolveHost();
	Application::ServerIP = "127.0.0.1";
	Application::ConnectToServer();
	Application::StartRecievingData();

	// Load the default font, probably change this up later.
	Application::TextTexture.Load("res/Font.png");

	GUI::Chatbox::Init();
	GUI::GetDesktop()->Hide();
	Block::LoadAllBlockTypeInfo();
	Block::LoadAllBlockTypeTextures();

	// Run the program and handle it's termination appropriately.
	Application::WindowMessageLoop();
	Application::ReleaseOpenGL();
	Application::Unload();

	// Return the exit code for the process.
	return Application::Msg.wParam;
}