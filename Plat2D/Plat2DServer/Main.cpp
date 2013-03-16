#include "Includes.h"
#include "PacketStructures.h"
#include "PacketParser.h"

char *IPAddress = "74.12.102.4";

SOCKET ServerSocket;
struct sockaddr_in ServerSin;
int length;

unsigned int CurrentID = 1;

struct SocketStruct
{
	unsigned int ID;
	SOCKET Socket;
};

list<SocketStruct*> ServerClients;

SOCKET FindSocket(unsigned int ID)
{
	list<SocketStruct*>::iterator it;

	for (it = ServerClients.begin(); it != ServerClients.end(); it++)
	{
		if ( (*it)->ID == ID)
		{
			return (*it)->Socket;
		}
	}

	return 0;
}

void InitWinsock()
{
	WSADATA wsaData;
	WORD version;
	int error;

	version = MAKEWORD( 2, 0 );

	error = WSAStartup( version, &wsaData );

	/* check for error */
	if ( error != 0 )
	{
		/* error occured */
		cout << "Failed to initialize WinSock." << endl;
	}

	/* check for correct version */
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 0 )
	{
		/* incorrect WinSock version */
		WSACleanup();
	}

	/* WinSock has been initialized */
	cout << "Successfully initialized WinSock." << endl;
}

void ServerCreateSocket()
{
	ServerSocket = socket( AF_INET, SOCK_STREAM, 0 );
	if (ServerSocket == SOCKET_ERROR)
	{
		cout << "Creating server socket failed." << endl;
	}
	else
	{
		cout << "Creating server socket succeeded." << endl;
	}
}

int ServerStartServer()
{
	memset( &ServerSin, 0, sizeof ServerSin );

	ServerSin.sin_family = AF_INET;
	ServerSin.sin_addr.s_addr = inet_addr(IPAddress); //INADDR_ANY;
	ServerSin.sin_port = htons( 21 );

	if ( bind( ServerSocket, (const sockaddr *)&ServerSin, sizeof ServerSin ) == SOCKET_ERROR )
	{
		/* could not start server */
		cout << "Failed to bind port. Cannot start server." << endl;
		return FALSE;
	}

	cout << "Succeeded in binding port " << ServerSin.sin_port << ". Server starting..." << endl;
	return TRUE;
}

void ServerListenForClient()
{
	if ( listen( ServerSocket, SOMAXCONN ) == SOCKET_ERROR )
	{
		cout << "Failed in listening for a client." << endl;
		char Error[100];
		sprintf(Error, "%d", WSAGetLastError());
		MessageBox(NULL, Error, Error, MB_OK);
	}
	else
	{
		cout << "Listening for a client." << endl;
	}
}

void ServerAcceptConnection(SOCKET& TargetClientSocket)
{
	length = sizeof(ServerSin);
	TargetClientSocket = accept( ServerSocket, (sockaddr *)&ServerSin, &length );
	cout << "Server accepted connection. Currently connected clients: " << ServerClients.size()+1 << endl;
}

void BroadcastPacket(const char *buffer, int length)
{
	list<SocketStruct*>::iterator it;

	for (it = ServerClients.begin(); it != ServerClients.end(); it++)
	{
		send( (*it)->Socket, buffer, length, 0);
	}
}

int ServerRecieveData(SocketStruct *ss)
{
	int ReturnValue = 0;
	char buffer[300];

	// Receive data
	int bytesReceived = recv(ss->Socket, buffer, sizeof(buffer), 0);

	if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)  // connection closed
	{
		cout << "The connection with the client has been terminated. ID: " << ss->ID << endl;
		ReturnValue = FALSE;
	}
	else
	{
		//Epic success.
		cout << bytesReceived << " B | ";
		PacketParser(ss->Socket, &buffer[0]);
		ReturnValue = TRUE;
	}

	// Send received data back
	//send(ss->Socket, buffer, bytesReceived, 0);
	//Broadcast test
	//BroadcastPacket(&buffer[0], bytesReceived);

	return ReturnValue;
}

void ServerAlwaysRecieve(SocketStruct *ss)
{
	while (true)
	{
		if (ss->Socket != INVALID_SOCKET)
		{
			if (!ServerRecieveData( ss ))
			{
				ServerClients.remove( ss );
				delete ss;
				break;
			}
		}
	}
}

//Entry point for the application.
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
int main()
{
	InitWinsock();

	ServerCreateSocket();
	ServerStartServer();

	while ( true )
	{
		ServerListenForClient();
		SocketStruct *ss = new SocketStruct();
		ss->ID = CurrentID;
		ss->Socket = 0;
		CurrentID++;

		ServerAcceptConnection( ss->Socket );
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ServerAlwaysRecieve, ss, NULL, NULL);
		ServerClients.push_back( ss );
	}

	closesocket( ServerSocket );
	WSACleanup();

	cout << "Press any key and then enter to exit." << endl;
	system("PAUSE");
}