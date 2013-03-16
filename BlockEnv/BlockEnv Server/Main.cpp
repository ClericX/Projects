#include <Windows.h>
//#include <Winsock2.h>
#include <stdio.h>
#include <iostream>
#include "PacketParser.h"
#include "../Frost/List.h"
#include "../Frost/String.h"
#include "../Frost/FrostConfig.h"
#include "../FrostGame/Block.h"
#include "../Moonlight Frost/PacketStructures.h"
#include "../Frost/FrostConfig.h"

using namespace std;

char *IPAddress = "127.0.0.1";

SOCKET ServerSocket;
struct sockaddr_in ServerSin;
int length;

unsigned int CurrentID = 0;

List<SocketStruct *> ServerClients;

SOCKET GetSocket(unsigned int ID)
{
	for (unsigned int i = 0; i < ServerClients.count(); i++)
	{
		if (ServerClients[i]->ID == ID)
		{
			return ServerClients[i]->Socket;
		}
	}

	return 0;
}

unsigned int GetIDFromSocket(SOCKET s)
{
	for (unsigned int i = 0; i < ServerClients.count(); i++)
	{
		if (ServerClients[i]->Socket == s)
		{
			return ServerClients[i]->ID;
		}
	}

	return 0;
}

unsigned int GetClientIndex(SocketStruct *theClient)
{
	for (unsigned int i = 0; i < ServerClients.count(); i++)
	{
		if (ServerClients[i]->ID == theClient->ID)
		{
			return i;
		}
	}

	return (unsigned int)-1;
}

void LoadBlocks()
{
	// Load the world from map.xml
	XmlDocument *map = new XmlDocument();
	map->Load("map.xml");

	for (unsigned int i = 0; i < map->DocumentElement->SubElements.count(); i++)
	{
		XmlElement *blockElem = map->DocumentElement->SubElements[i];
		Block::Instance *block = new Block::Instance();
		block->Position = Vector3f((float)blockElem->getAttribute("x")->Value.toInt(), (float)blockElem->getAttribute("y")->Value.toInt(), (float)blockElem->getAttribute("z")->Value.toInt());
		block->Type = Block::GetTypeInfoByID(blockElem->getAttribute("id")->Value.toInt());
		Block::Add(block);
	}

	Log::Msg("Loaded map block information from \"map.xml\"", Log::Info);
}

void SaveBlocks()
{
	// Save the world to map.xml
	XmlDocument *map = new XmlDocument();

	for (unsigned int i = 0; i < Block::WorldBlockList.count(); i++)
	{
		Block::Instance *theBlock = Block::WorldBlockList[i];
		XmlElement *blockElem = new XmlElement();
		blockElem->Name = "block";
		blockElem->setAttribute("x", String((int)theBlock->Position.x));
		blockElem->setAttribute("y", String((int)theBlock->Position.y));
		blockElem->setAttribute("z", String((int)theBlock->Position.z));
		blockElem->setAttribute("id", String((int)theBlock->Type->ID));
		map->DocumentElement->addNode(blockElem);
	}

	map->Save("map.xml");

	Log::Msg("Saved map block information to \"map.xml\"", Log::Info);
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
		Log::Msg("Failed to initialize WinSock.", Log::FatalError);
	}

	/* check for correct version */
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
		 HIBYTE( wsaData.wVersion ) != 0 )
	{
		/* incorrect WinSock version */
		WSACleanup();
	}

	/* WinSock has been initialized */
	Log::Msg("Successfully initialized WinSock.", Log::Info);
}

void ServerCreateSocket()
{
	ServerSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if (ServerSocket == SOCKET_ERROR)
	{
		Log::Msg("Creating server socket failed.", Log::FatalError);
	}
	else
	{
		Log::Msg("Creating server socket succeeded.", Log::Info);
	}
}

int ServerStartServer()
{
	memset( &ServerSin, 0, sizeof ServerSin );

	ServerSin.sin_family = AF_INET;
	ServerSin.sin_addr.s_addr = inet_addr(IPAddress); //INADDR_ANY;
	ServerSin.sin_port = htons( 9337 );

	if ( bind( ServerSocket, (const sockaddr *)&ServerSin, sizeof ServerSin ) == SOCKET_ERROR )
	{
		/* could not start server */
		Log::Msg("Failed to bind port. Cannot start server.", Log::FatalError);
		return FALSE;
	}

	String out;
	out << "Succeeded in binding port " << String((long)ServerSin.sin_port) << ". Server starting...";
	Log::Msg(out, Log::Info);
	return TRUE;
}

void ServerListenForClient()
{
	if ( listen( ServerSocket, SOMAXCONN ) == SOCKET_ERROR )
	{
		String theError;
		theError << "Failed in listening for a client. Error Code: " << WSAGetLastError();
		Log::Msg(theError, Log::FatalError);
	}
	else
	{
		Log::Msg("Listening for a client.", Log::Info);
	}
}

void ServerAcceptConnection(SOCKET& TargetClientSocket)
{
	length = sizeof(ServerSin);
	TargetClientSocket = accept( ServerSocket, (sockaddr *)&ServerSin, &length );
	String out;
	out << "Server accepted connection. Currently connected clients: " << (long)ServerClients.count() + 1;
	Log::Msg(out, Log::Info);
}

void BroadcastPacket(String thePacket)
{
	for (unsigned int i = 0; i < ServerClients.count(); i++)
	{
		SendPacket(ServerClients[i]->Socket, thePacket);
	}
}

void SendPacket(SOCKET& s, String thePacket)
{
	unsigned int sendSize = thePacket.length();
	thePacket.setChar(sendSize, '\r');
	send(s, thePacket, sendSize + 1, 0);
}

int ServerRecieveData(SocketStruct *ss)
{
	int ReturnValue = -1;
	char recvChar[1] = { 0 };
	String fullPacket;
	bool packetDone = false;

	while (!packetDone)
	{
		memset(recvChar, 0, 1);

		// Receive data one byte at a time; this loop terminates when a packet-break char (\r) occurs.
		int bytesReceived = recv(ss->Socket, &recvChar[0], 1, 0);

		if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)  // connection closed
		{
			String errorStr;
			errorStr << "The connection with the client has been terminated. ID: " << (long)ss->ID;
			Log::Msg(errorStr, Log::Info);
			ReturnValue = FALSE;
			packetDone = true;
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
			crashError << "Recieved a strange amount of data from the client somehow, " << bytesReceived << " bytes.";
			Log::Msg(crashError, Log::FatalError);
		}
	}

	//Epic success, recieved data.
	//Log::Msg(bytesReceived << " B | ";
	if (ReturnValue != FALSE)
	{
		PacketParser(ss->Socket, &fullPacket);
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
				ServerClients.Remove( GetClientIndex(ss) );
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
	SetWindowTextA(GetConsoleWindow(), "BlockEnv Server");

	Block::LoadAllBlockTypeInfo();
	LoadBlocks();

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
		ServerClients.Add( ss );

		SaveBlocks();
	}

	closesocket( ServerSocket );
	WSACleanup();

	Log::Msg("Press any key and then enter to exit.", Log::Info);
	system("PAUSE");
}