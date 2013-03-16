#include "Application.h"
#include "Main.h"

Application::Application(void)
{
}

Application::~Application(void)
{
}

void Application::Initialize(LPCSTR tTitle, float fVer, DWORD dBuild, int tWidth, int tHeight)
{
	//Get initial screen environment metrics
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	BorderWidth = GetSystemMetrics(SM_CXFIXEDFRAME);
	BorderHeight = GetSystemMetrics(SM_CYFIXEDFRAME);
	CaptionHeight = GetSystemMetrics(SM_CYCAPTION);

	//Declare a temporary buffer to store version number for VersionStr use
	char TempBuf[10];

	//Clear all the char buffers we're gunna use, so there is no garbage data causing weird string errors
	ZeroMemory(&WndClassName, sizeof(WndClassName));
	ZeroMemory(&WndTitle, sizeof(WndTitle));
	ZeroMemory(&DbgWndClassName, sizeof(DbgWndClassName));
	ZeroMemory(&DbgWndTitle, sizeof(DbgWndTitle));
	ZeroMemory(&VersionStr, sizeof(VersionStr));
	ZeroMemory(&TempBuf, sizeof(TempBuf));

	//Fill these variables with the parameters given
	strcpy_s(WndTitle, tTitle);
	Version = fVer;
	BuildNumber = dBuild;
	ClientWidth = tWidth;
	ClientHeight = tHeight;

	//Declare window class names/titles
	strcpy(WndClassName, "FrostClass");
	strcpy(DbgWndClassName, "DbgFrostClass");
	strcpy(DbgWndTitle, "Debug");

	//Fill version into VersionStr
	sprintf(TempBuf, "%.2f", Version);
	strcpy(VersionStr, "v");
	strcat(VersionStr, TempBuf);
	strcpy(BuildStr, "Build ");
	strcat(BuildStr, _ultoa(BuildNumber, TempBuf, 10));

	//Add version number to the window title of main game & debug window
	strcat(WndTitle, " ");
	strcat(WndTitle, VersionStr);
	strcat(WndTitle, " ");
	strcat(WndTitle, BuildStr);
	strcat(DbgWndTitle, " ");
	strcat(DbgWndTitle, VersionStr);

	//Apply the parameter again to here
	Width = ClientWidth;
	Height = ClientHeight;

	//Make necessary screen calculations to center the window
	Width += (BorderWidth * 2);
	Height += ( (BorderHeight * 2) + CaptionHeight );
	WndX = (ScreenWidth / 2) - (Width / 2);
	WndY = (ScreenHeight / 2) - (Height / 2);

	//Load all the cursors we're gunna need for the game
	DefaultCursor = LoadCursorA(NULL, IDC_ARROW);
	NoCursor = LoadCursorA(hInstance, NULL);
	InGameCursor = LoadCursorA(NULL, IDC_CROSS); //Temporary, crosshair as in-game cursor.

	//Seed the random-number generator with current time so that the numbers will be different every time we run.
	srand( (unsigned)time( NULL ) );

	CurrentDirectory = GetCurrentPath(0);
	CurrentDirectory += "\\";

	//Attempt to initialize the HiResTimer class instance.
	if ( !Timer.Init() )
	{
		MessageBox(NULL, "Your system does not support the high-resolution timer, which is required for the game.\nPlease get a new computer. Kthxbai.", "Epic Fail", MB_OK | MB_ICONEXCLAMATION);
	}
	else
	{
		//The game are good to goez, all things werks fien.
		Running = true;
	}
}

void Application::InitializeWindow()
{
	WNDCLASSEX wc;

	wc.cbClsExtra = 0;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.cbWndExtra = 0;
	wc.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wc.hCursor = 0;
	wc.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wc.hInstance = hInstance; //GetModuleHandle(NULL);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = WndClassName;
	wc.lpszMenuName = 0;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	hWnd = CreateWindowExA(NULL, WndClassName, WndTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, WndX, WndY, Width, Height, NULL, NULL, hInstance, NULL);
	ShowWindow(hWnd, SW_SHOW);
}

void Application::InitializeDebugWindow()
{
	WNDCLASSEX wcd;

	wcd.cbClsExtra = 0;
	wcd.cbSize = sizeof(WNDCLASSEX);
	wcd.cbWndExtra = 0;
	wcd.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
	wcd.hCursor = DefaultCursor;
	wcd.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wcd.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
	wcd.hInstance = hDebugInstance; //GetModuleHandle(NULL);
	wcd.lpfnWndProc = WndProc;
	wcd.lpszClassName = DbgWndClassName;
	wcd.lpszMenuName = 0;
	wcd.style = CS_GLOBALCLASS;

	RegisterClassEx(&wcd);

	DbghWnd = CreateWindowExA(NULL, DbgWndClassName, DbgWndTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, WndX + Width, WndY, 200, 400, NULL, NULL, hDebugInstance, NULL);
	ShowWindow(DbghWnd, SW_SHOW);
}

void Application::Unload()
{
	UnregisterClass(App.WndClassName, App.hInstance);
	UnregisterClass(App.DbgWndClassName, App.hDebugInstance);
	D3D.D3DDevice->Release();
	D3D.D3DObject->Release();
	D3D.FrostFont->Release();
	DI.InputObject->Release();
	DI.KeyboardDevice->Release();
	closesocket( ClientSocket );
	WSACleanup();
}

int Application::GetRand(int low, int high)
{
	return (rand() % ( (high + 1) - low ) ) + low;
}

void Application::InitializeWinsock()
{
	ServerIP = "74.12.102.4";

	WSADATA wsaData;
	WORD version;
	int error;

	version = MAKEWORD( 2, 0 );

	error = WSAStartup( version, &wsaData );

	/* check for error */
	if ( error != 0 )
	{
		/* error occured */
	}

	/* check for correct version */
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 0 )
	{
		/* incorrect WinSock version */
		WSACleanup();
	}

	/* WinSock has been initialized */
}

void Application::CreateClientSocket()
{
	ClientSocket = socket( AF_INET, SOCK_STREAM, 0 );
	/*
	if (ClientSocket == SOCKET_ERROR)
	{
		cout << "Creating client socket failed." << endl;
	}
	else
	{
		cout << "Creating client socket succeeded." << endl;
	}
	*/
}

void Application::ResolveHost()
{
	HostEntity = gethostbyname( ServerIP.c_str() );

	/*
	if (HostEntity != NULL)
	{
		cout << "Successfully found host." << endl;
	}
	else
	{
		cout << "Failed to find host." << endl;
	}
	*/
}

void Application::ConnectToServer()
{
	memset( &ClientSin, 0, sizeof ClientSin );

	ClientSin.sin_family = AF_INET;
	ClientSin.sin_addr.s_addr = inet_addr( ServerIP.c_str() ); //((struct in_addr *)host.h_addr_list[0])->s_addr; // inet_addr("192.168.0.5"); for a specific IP address.
	ClientSin.sin_port = htons( 21 );

	if ( connect( ClientSocket, (const sockaddr *)&ClientSin, sizeof ClientSin ) == SOCKET_ERROR )
	{
		/* could not connect to server */
	}

	/* Successfully connected to the server. */
}

void RecievezData()
{
	while( true )
	{
		// Receive data
		int bytesReceived = recv(App.ClientSocket, App.PacketBuffer, sizeof(App.PacketBuffer), 0);

		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)  // connection closed
		{
			//MessageBox(App.hWnd, "The connection with the server has been terminated.", "Server DC", MB_OK | MB_ICONERROR);
		}
		else
		{
			//Epic success.
			PacketParser(&App.PacketBuffer[0]);
		}
	}
}

void Application::StartRecievingData()
{
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&RecievezData, NULL, NULL, NULL);
}

// function to return the current working directory
// this is generally the application path
char *Application::GetCurrentPath(char* buffer)
{
	return _getcwd(buffer, _MAX_PATH);
}