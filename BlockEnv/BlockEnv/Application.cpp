#include "Application.h"
#include "HiResTimer.h"
#include "Input.h"
#include <time.h>
#include <math.h>

#include "../Frost/Vector3f.h"
#include "../Frost/Matrix4x4f.h"
#include "Flags.h"
#include "myglu.h"

#include "PacketStructures.h"

namespace Application
{
	// Game window variables
	HWND Window;
	HDC WindowDC;
	HGLRC WindowGLRC;
	WNDCLASSEX WindowClass;
	char WndClassName[100];
	String WndTitle;
	HINSTANCE hInstance;
	MSG Msg;

	// Debug window variables
	WNDCLASSEX DebugWindowClass;
	char DbgWndClassName[100];
	String DbgWndTitle;

	// Cursor variables
	HCURSOR DefaultCursor;
	HCURSOR NoCursor;
	HCURSOR InGameCursor;

	// Client window variables
	RECT ClientRect;
	POINT WndTopLeft;
	POINT WndBottomRight;

	// Version variables
	float Version;
	DWORD BuildNumber;

	bool Running;
	double RunTime;

	double TimeElapsedThisFrame;

	Texture TextTexture;

	// Screen/window variables
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

	int CurrentDrawMode = 0;

	Vector3f CharPos = Vector3f(0, 50, 0);
	Vector3f CamPos;
	float CamPitch = -8.0f;
	float CamYaw = 227.0f;

	//Winsock variables
	char PacketBuffer[300];
	SOCKET ClientSocket;
	String ServerIP;
	struct hostent *HostEntity;
	struct sockaddr_in ClientSin;

	unsigned int PlacementBlockID = 1;

	void GoFullscreen()
	{
		DISPLAY_DEVICE dd;
		DEVMODE devMode;
		DEVMODE tmpDevMode;
		dd.cb = sizeof(DISPLAY_DEVICE);
		devMode.dmSize = sizeof(DEVMODE);
		devMode.dmDriverExtra = 0;
		EnumDisplayDevices(NULL, 0, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
		EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &devMode);
		tmpDevMode = devMode;
		tmpDevMode.dmPelsWidth = 800;
		tmpDevMode.dmPelsHeight = 600;
		long res = ChangeDisplaySettings(&tmpDevMode, CDS_FULLSCREEN);
		if (res != DISP_CHANGE_SUCCESSFUL)
		{
			MessageBoxA(0, "failed to change resolution.", "derp", MB_OK);
		}
		Sleep(5000);
		ChangeDisplaySettings(&devMode, 0);
	}

	void Initialize(char *tTitle, float fVer, DWORD dBuild, int tWidth, int tHeight)
	{
		hInstance = GetModuleHandle(NULL);
		memset(&Msg, 0, sizeof(Msg));

		// Get initial screen environment metrics
		ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
		ScreenHeight = GetSystemMetrics(SM_CYSCREEN);
		BorderWidth = GetSystemMetrics(SM_CXFIXEDFRAME);
		BorderHeight = GetSystemMetrics(SM_CYFIXEDFRAME);
		CaptionHeight = GetSystemMetrics(SM_CYCAPTION);

		// Fill these variables with the parameters given
		WndTitle = tTitle;
		Version = fVer;
		BuildNumber = dBuild;
		ClientWidth = tWidth;
		ClientHeight = tHeight;

		// Declare window class names/titles
		strcpy(WndClassName, "BlockEnvClass");

		// Apply the parameter again to here
		Width = ClientWidth;
		Height = ClientHeight;

		// Make necessary screen calculations to center the window
		Width += (BorderWidth * 2);
		Height += ( (BorderHeight * 2) + CaptionHeight );
		WndX = (ScreenWidth / 2) - (Width / 2);
		WndY = (ScreenHeight / 2) - (Height / 2);

		// Load all the cursors we're gunna need for the game
		DefaultCursor = LoadCursorA(NULL, IDC_ARROW);
		NoCursor = LoadCursorA(hInstance, NULL);
		InGameCursor = LoadCursorA(NULL, IDC_CROSS); //Temporary, crosshair as in-game cursor.

		//Seed the random-number generator with current time so that the numbers will be different every time we run.
		srand( (unsigned)time( NULL ) );

		//Attempt to initialize the HiResTimer.
		if ( !HiResTimer::Initialize() )
		{
			Log::Msg("Your system does not support the high-resolution timer, which is required for the game.\nPlease get a new computer. Kthxbai.", Log::FatalError);
		}
		else
		{
			//The game are good to goez, all things werks fien.
			Running = true;
		}
	}

	void InitializeWindow()
	{
		WindowClass.lpszClassName	= WndClassName;
		WindowClass.cbSize			= sizeof(WNDCLASSEX);
		WindowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		WindowClass.lpfnWndProc		= WndProc;
		WindowClass.hInstance		= hInstance;
		WindowClass.hIcon			= LoadIcon(hInstance, IDI_APPLICATION);
		WindowClass.hIconSm			= LoadIcon(hInstance, IDI_APPLICATION);
		WindowClass.hCursor			= InGameCursor;
		WindowClass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
		WindowClass.lpszMenuName	= NULL;
		WindowClass.cbClsExtra		= 0;
		WindowClass.cbWndExtra		= 0;

		if( !RegisterClassEx(&WindowClass) )
		{
			Log::Msg("Failed to register window class.", Log::FatalError);
		}

		Window = CreateWindowExA(NULL, WndClassName, WndTitle.Text, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE, WndX, WndY, Width, Height, NULL, NULL, hInstance, NULL);

		if (Window == NULL)
		{
			Log::Msg("Failed to create the window.", Log::FatalError);
		}
	}

	void InitializeOpenGL()
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));

		pfd.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion   = 1;
		pfd.dwFlags    = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 24;
		//pfd.cStencilBits = 8;

		WindowDC = GetDC( Window );
		GLuint iPixelFormat = ChoosePixelFormat( WindowDC, &pfd );

		if( iPixelFormat != 0 )
		{
			PIXELFORMATDESCRIPTOR bestMatch_pfd;
			DescribePixelFormat( WindowDC, iPixelFormat, sizeof(pfd), &bestMatch_pfd );

			// TO DO: Double-check  the closet match pfd for anything unacceptable...

			if( bestMatch_pfd.cDepthBits < pfd.cDepthBits )
			{
				// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
				Log::Msg("We need at least a 16-bit z-buffer!", Log::FatalError);
				return;
			}

			if( SetPixelFormat( WindowDC, iPixelFormat, &pfd) == FALSE )
			{
				DWORD dwErrorCode = GetLastError();
				// TO DO: Report cause of failure here...
				Log::Msg("Chose pixel format but failed to set it.", Log::FatalError);
				return;
			}
		}
		else
		{
			DWORD dwErrorCode = GetLastError();
			// TO DO: Report cause of failure here...
			Log::Msg("Failed to choose pixel format entirely.", Log::FatalError);
			return;
		}

		WindowGLRC = wglCreateContext( WindowDC );
		wglMakeCurrent( WindowDC, WindowGLRC );

		glClearColor( 0.35f, 0.53f, 0.7f, 1.0f );
		glClearDepth(1.0f);
		glEnable( GL_TEXTURE_2D );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glShadeModel(GL_SMOOTH);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glViewport(0, 0, ClientWidth, ClientHeight);
		gluPerspective( 45.0f, (float)ClientWidth / (float)ClientHeight, 0.0f, 1000.0f);
	}

	void ReleaseOpenGL()
	{
		if( WindowGLRC != NULL )
		{
			wglMakeCurrent( NULL, NULL );
			wglDeleteContext( WindowGLRC );
			WindowGLRC = NULL;
		}

		if( WindowDC != NULL )
		{
			ReleaseDC( Window, WindowDC );
			WindowDC = NULL;
		}
	}

	void Unload()
	{
		UnregisterClass(WndClassName, hInstance);
		UnregisterClass(DbgWndClassName, hInstance);
		closesocket( ClientSocket );
		WSACleanup();
	}

	void WindowMessageLoop()
	{
		//while( Msg.message != WM_QUIT )
		while ( Running )
		{
			if( PeekMessage( &Msg, NULL, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &Msg ); // Transform certain WM_KEYDOWN messages to WM_CHAR messages.
				DispatchMessage( &Msg ); // Perform actions based on message acquire appropriately.
			}

			HiResTimer::Update();
			TimeElapsedThisFrame = HiResTimer::GetElapsedMilliseconds();
			RunTime += TimeElapsedThisFrame;
			Input::Update();
			//UpdateCamera();
			DrawScene();
		}
	}

	int GetRand(int low, int high)
	{
		return (rand() % ( (high + 1) - low ) ) + low;
	}

	void InitializeWinsock()
	{
		ServerIP = "127.0.0.1";

		WSADATA wsaData;
		WORD version;
		int error;

		version = MAKEWORD( 2, 0 );

		error = WSAStartup( version, &wsaData );

		/* check for error */
		if ( error != 0 )
		{
			/* error occured */
			Log::Msg("Initializing WinSock failed.", Log::FatalError);
		}

		/* check for correct version */
		if ( LOBYTE( wsaData.wVersion ) != 2 ||
			 HIBYTE( wsaData.wVersion ) != 0 )
		{
			/* incorrect WinSock version */
			Log::Msg("WinSock was initialized but would not initialize to requested version.", Log::FatalError);
			WSACleanup();
		}

		/* WinSock has been initialized */
		Log::Msg("WinSock was successfully initialized.", Log::Info);
	}

	void CreateClientSocket()
	{
		ClientSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

		if (ClientSocket == SOCKET_ERROR)
		{
			Log::Msg("Creating client socket failed.", Log::FatalError);
		}
		else
		{
			Log::Msg("Creating client socket succeeded.", Log::Info);
		}

	}

	void ResolveHost()
	{
		HostEntity = gethostbyname( ServerIP );

		if (HostEntity != NULL)
		{
			Log::Msg("Successfully found host.", Log::Info);
		}
		else
		{
			Log::Msg("Failed to find host.", Log::FatalError);
		}
	}

	void ConnectToServer()
	{
		memset( &ClientSin, 0, sizeof ClientSin );

		ClientSin.sin_family = AF_INET;
		ClientSin.sin_addr.s_addr = inet_addr( ServerIP ); //((struct in_addr *)host.h_addr_list[0])->s_addr; // inet_addr("192.168.0.5"); for a specific IP address.
		ClientSin.sin_port = htons( 9337 );

		if ( connect( ClientSocket, (const sockaddr *)&ClientSin, sizeof ClientSin ) == SOCKET_ERROR )
		{
			/* could not connect to server */
			Log::Msg("Could not connect to the server.", Log::FatalError);
		}

		/* Successfully connected to the server. */
		Log::Msg("Successfully connected to the server.", Log::Info);
	}

	void RecieveData()
	{
		while( true )
		{
			String fullPacket;
			char recvChar[1] = { 0 };
			bool packetDone = false;

			while (!packetDone)
			{
				memset(recvChar, 0, 1);

				// Receive data one byte at a time; this loop terminates when a packet-break char (\r) occurs.
				int bytesReceived = recv(ClientSocket, &recvChar[0], 1, 0);

				if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)  // connection closed
				{
					Log::Msg("The connection with the server has been terminated.", Log::FatalError);
				}
				else if (bytesReceived == 1)
				{
					if (recvChar[0] != '\r')
					{
						fullPacket += recvChar[0];
					}
					else
					{
						packetDone = true;
					}
				}
				else
				{
					String crashError;
					crashError << "Recieved a strange amount of data from the server somehow, " << bytesReceived << " bytes.";
					Log::Msg(crashError, Log::FatalError);
				}
			}
			
			PacketParser(&fullPacket);
		}
	}

	void StartRecievingData()
	{
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&RecieveData, NULL, NULL, NULL);
	}

	void PrintText(float X, float Y, String Text)
	{
		glColor4ub(255, 255, 255, 255);
		glBindTexture(GL_TEXTURE_2D, TextTexture.ID);
		
		float tempX = X;
		float tempY = Y;
		
		float FontWidth = 192.0f;
		float FontHeight = 160.0f;
		float LetterWidth = FontWidth / 16.0f;
		float LetterHeight = FontHeight / 8.0f;
		
		for (unsigned int i = 0; i < Text.length(); i++)
		{
			if (Text.charAt(i) == '\n')
			{
				tempY += LetterHeight;
				tempX = X;
				continue;
			}
			
			int letternum = Text.charAt(i)-32;
			
			float LetterY = 0; // Y Position Of Current Character
			while (letternum > 15)
			{
				LetterY += LetterHeight;
				letternum -= 16;
			}
			float LetterX = (letternum*LetterWidth); // X Position Of Current Character
			
			glBegin(GL_QUADS);
				
				glTexCoord2f((LetterX/FontWidth), (LetterY/FontHeight));
				glVertex2f(tempX, tempY);
				
				glTexCoord2f(((LetterX+LetterWidth)/FontWidth), (LetterY/FontHeight));
				glVertex2f(tempX + LetterWidth, tempY);
				
				glTexCoord2f(((LetterX+LetterWidth)/FontWidth), ((LetterY+LetterHeight)/FontHeight));
				glVertex2f(tempX + LetterWidth, tempY + LetterHeight);
				
				glTexCoord2f((LetterX/FontWidth), ((LetterY+LetterHeight)/FontHeight));
				glVertex2f(tempX, tempY + LetterHeight);
				
			glEnd(); // Done Building Our Quad (Character)
			
			tempX += LetterWidth;
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void DrawQuad(float X, float Y, float Width, float Height, Color *clr, Texture *tex)
	{
		if (clr != NULL)
		{
			glColor4ub(clr->R, clr->G, clr->B, clr->A);
		}
		else
		{
			glColor4ub(255, 255, 255, 255);
		}

		if (!tex)
		{
			glBegin(GL_QUADS);
				glVertex2f(X, Y);
				glVertex2f(X + Width, Y);
				glVertex2f(X + Width, Y + Height);
				glVertex2f(X, Y + Height);
			glEnd();
		}
		else
		{
			tex->Bind();
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glVertex2f(X, Y);
				glTexCoord2f(1, 0);
				glVertex2f(X + Width, Y);
				glTexCoord2f(1, 1);
				glVertex2f(X + Width, Y + Height);
				glTexCoord2f(0, 1);
				glVertex2f(X, Y + Height);
			glEnd();
			tex->Unbind();
		}
	}

	void Move3DHorizontally(Vector3f &Location, float Direction, float Distance)
	{
		float rad = (Direction) * PI/180.0f; // convert the degrees into radians
		Location.x -= sin(rad)*Distance; // calculate the new coorinate, if you don't understand, draw a right triangle with the datas, you have
		Location.z -= cos(rad)*Distance; // and try to calculate the new coorinate with trigonometric functions, that should help
	}

	void Move3DVertically(Vector3f &Location, float Direction, float Distance)
	{
		// the the same, only this time we calculate the y coorinate
		float rad = (Direction) * PI/180.0f;
		Location.y += sin(rad)*Distance;
	}

	void MoveCharacter(float dist, float dir)
	{
		Move3DHorizontally(CharPos, CamYaw+dir, dist);
	}

	void MoveCharacterUp(float dist, float dir)
	{
		Move3DVertically(CharPos, CamPitch+dir, dist);
	}

	void MoveCharacterWithCollision(float dist, float dir)
	{
		Vector3f OldPos = CharPos;
		Vector3f OldGridCharPos = Block::AbsCoordToGridCoord(OldPos); // Get the old grid coordinate for relative block checks.

		MoveCharacter(dist, dir);

		Vector3f NewGridCharPos = Block::AbsCoordToGridCoord(CharPos); // Get the new grid coordinate for collision detection.

		// Check all surrounding relevant blocks.
		Block::Instance *FrontTop = Block::Get(Vector3f(OldGridCharPos.x, OldGridCharPos.y + 1, OldGridCharPos.z + 1));
		Block::Instance *FrontBot = Block::Get(Vector3f(OldGridCharPos.x, OldGridCharPos.y, OldGridCharPos.z + 1));
		Block::Instance *BackTop = Block::Get(Vector3f(OldGridCharPos.x, OldGridCharPos.y + 1, OldGridCharPos.z - 1));
		Block::Instance *BackBot = Block::Get(Vector3f(OldGridCharPos.x, OldGridCharPos.y, OldGridCharPos.z - 1));
		Block::Instance *LeftTop = Block::Get(Vector3f(OldGridCharPos.x - 1, OldGridCharPos.y + 1, OldGridCharPos.z));
		Block::Instance *LeftBot = Block::Get(Vector3f(OldGridCharPos.x - 1, OldGridCharPos.y, OldGridCharPos.z));
		Block::Instance *RightTop = Block::Get(Vector3f(OldGridCharPos.x + 1, OldGridCharPos.y + 1, OldGridCharPos.z));
		Block::Instance *RightBot = Block::Get(Vector3f(OldGridCharPos.x + 1, OldGridCharPos.y, OldGridCharPos.z));

		bool FrontTopCollision = false, FrontBotCollision = false, BackTopCollision = false, BackBotCollision = false,
			LeftTopCollision = false, LeftBotCollision = false, RightTopCollision = false, RightBotCollision = false;

		String collide;

		if (FrontTop)
		{
			if (FrontTop->Position == NewGridCharPos.DeriveRelatively(0, 1, 0))
			{
				collide << "FrontTop "; FrontTopCollision = true;
			}
		}

		if (FrontBot)
		{
			if (FrontBot->Position == NewGridCharPos)
			{
				collide << "FrontBot "; FrontBotCollision = true;
			}
		}

		if (BackTop)
		{
			if (BackTop->Position == NewGridCharPos.DeriveRelatively(0, 1, 0))
			{
				collide << "BackTop "; BackTopCollision = true;
			}
		}

		if (BackBot)
		{
			if (BackBot->Position == NewGridCharPos)
			{
				collide << "BackBot "; BackBotCollision = true;
			}
		}

		if (LeftTop)
		{
			if (LeftTop->Position == NewGridCharPos.DeriveRelatively(0, 1, 0))
			{
				collide << "LeftTop "; LeftTopCollision = true;
			}
		}

		if (LeftBot)
		{
			if (LeftBot->Position == NewGridCharPos)
			{
				collide << "LeftBot "; LeftBotCollision = true;
			}
		}

		if (RightTop)
		{
			if (RightTop->Position == NewGridCharPos.DeriveRelatively(0, 1, 0))
			{
				collide << "RightTop "; RightTopCollision = true;
			}
		}

		if (RightBot)
		{
			if (RightBot->Position == NewGridCharPos)
			{
				collide << "RightBot "; RightBotCollision = true;
			}
		}

		if (!collide.empty())
		{
			Log::Msg(collide, Log::Debug);
			CharPos = OldPos;
		}

		// This block handles moving sideways upon a collision with a block on any direction based on location of the collision
		// as well as the direction the person is facing to determine the direction that person should move towards.
		if (FrontTopCollision || FrontBotCollision)
		{
			// Facing away from the front.
			if (CamYaw < 90 || CamYaw > 270)
			{
				if (CamYaw > 90)
				{
					Move3DHorizontally(CharPos, 90, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 270, dist);
				}
			}
			// Facing towards the front.
			else
			{
				if (CamYaw < 180)
				{
					Move3DHorizontally(CharPos, 90, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 270, dist);
				}
			}
		}

		if (BackTopCollision || BackBotCollision)
		{
			// Facing towards the back.
			if (CamYaw < 90 || CamYaw > 270)
			{
				if (CamYaw < 90)
				{
					Move3DHorizontally(CharPos, 90, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 270, dist);
				}
			}
			// Facing away from the back.
			else
			{
				if (CamYaw > 180)
				{
					Move3DHorizontally(CharPos, 90, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 270, dist);
				}
			}
		}

		if (LeftTopCollision || LeftBotCollision)
		{
			// Facing towards the left.
			if (CamYaw < 180 && CamYaw > 0)
			{
				if (CamYaw < 90)
				{
					Move3DHorizontally(CharPos, 0, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 180, dist);
				}
			}
			// Facing away from the left.
			else
			{
				if (CamYaw < 270)
				{
					Move3DHorizontally(CharPos, 0, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 180, dist);
				}
			}
		}

		if (RightTopCollision || RightBotCollision)
		{
			// Facing towards the back.
			if (CamYaw < 360 && CamYaw > 180)
			{
				if (CamYaw > 270)
				{
					Move3DHorizontally(CharPos, 0, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 180, dist);
				}
			}
			// Facing away from the back.
			else
			{
				if (CamYaw > 90)
				{
					Move3DHorizontally(CharPos, 0, dist);
				}
				else
				{
					Move3DHorizontally(CharPos, 180, dist);
				}
			}
		}
	}

	void MoveCharacterUpWithCollision(float dist, float dir)
	{
		Vector3f OldPos = CharPos;

		MoveCharacterUp(dist, dir);

		Vector3f GridCharPos = Block::AbsCoordToGridCoord(CharPos);

		// Check all surrounding relevant blocks.
		Block::Instance *Above = Block::Get(Vector3f(GridCharPos.x, GridCharPos.y + 3, GridCharPos.z));
		Block::Instance *Below = Block::Get(Vector3f(GridCharPos.x, GridCharPos.y - 1, GridCharPos.z));

		if (Above || Below)
		{
			CharPos = OldPos;
		}
	}

	// Function to update the current modelview matrix stack to reflect camera variables (positioning and rotation).
	void UpdateCamera()
	{
		// Move the camera to where the player is and slightly higher than their feet.
		//Move3DVertically(CamPos, CamYaw, 5);
		CamPos.x = CharPos.x;
		CamPos.y = CharPos.y + 15;
		CamPos.z = CharPos.z;

		glRotatef(-CamPitch, 1.0, 0.0, 0.0); //rotate the camera (more precisly move everything in the opposit direction)
		glRotatef(-CamYaw, 0.0, 1.0, 0.0);
	
		glTranslatef(-CamPos.x, -CamPos.y, -CamPos.z); //move the camera
	}

	void ApplyCharacterMovement()
	{
		float movevel=(50.0f * (float)TimeElapsedThisFrame) / 1000.0f, mousevel=0.2f;

		if (Input::IsForGame())
		{
			Point CursorChange = Input::GetCursorChange();

			CamYaw -= mousevel*CursorChange.X; //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
			CamPitch -= mousevel*CursorChange.Y; //this is for X

			//set campitch between -90 and 90 and set camyaw between 0 and 360 degrees
			if(CamPitch > 90)
				CamPitch = 90;
			if(CamPitch < -90)
				CamPitch = -90;
			if(CamYaw < 0.0)
				CamYaw += 360.0;
			if(CamYaw > 360.0)
				CamYaw -= 360;
		
			if(Input::GetKeyByKeyCode(Input::Keys::W)->CurrentlyDown)
			{
				if(CamPitch != 90 && CamPitch != -90)	//if we are facing directly up or down, we don't go forward, it will be commented out, when there will be gravity
					MoveCharacterWithCollision(movevel, 0.0);	//move forward
					//MoveCharacterUp(movevel, 0.0);	//move up/down
			}
			if(Input::GetKeyByKeyCode(Input::Keys::S)->CurrentlyDown)
			{
				//same, just we use 180 degrees, so we move at the different direction (move back)
				if(CamPitch != 90 && CamPitch != -90)
					MoveCharacterWithCollision(movevel, 180.0);
					//MoveCharacterUp(movevel, 180.0);
			}		
			if(Input::GetKeyByKeyCode(Input::Keys::A)->CurrentlyDown)	//move left
				MoveCharacterWithCollision(movevel, 90.0);
			if(Input::GetKeyByKeyCode(Input::Keys::D)->CurrentlyDown)	//move right
				MoveCharacterWithCollision(movevel, 270);
		}
	}

	void setDrawMode(int DrawMode)
	{
		if (CurrentDrawMode != DrawMode)
		{
			switch (DrawMode)
			{
				case DRAWMODE_2D:
				{
					glMatrixMode(GL_PROJECTION);
					glLoadIdentity();
					glOrtho(0, 800, 600, 0, 0, 1);
					glDisable(GL_DEPTH_TEST);
					glMatrixMode(GL_MODELVIEW);
					glLoadIdentity();
					// Displacement trick for exact pixelization
					glTranslatef(0.375f, 0.375f, 0.0f);
					break;
				}
				case DRAWMODE_3D:
				{
					glEnable(GL_DEPTH_TEST);
					glViewport(0, 0, 800, 600);
					glMatrixMode( GL_PROJECTION );
					glLoadIdentity();
					gluPerspective( 45.0f, (float)800 / (float)600, 0.1f, 1000.0f);
					glMatrixMode( GL_MODELVIEW );
					glLoadIdentity();
					break;
				}
				default:
				{
					break;
				}
			}
			CurrentDrawMode = DrawMode;
		}
	}

	void SendPacket(String thePacket)
	{
		unsigned int sendSize = thePacket.length();
		thePacket.setChar(sendSize, '\r');
		send(ClientSocket, thePacket, sendSize + 1, 0);
	}
};