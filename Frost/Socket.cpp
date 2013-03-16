#define FROST_EXPORT
#include "Socket.h"

#include "Log.h"

namespace Frost
{
	namespace Net
	{
		Socket::Socket(void)
		{
			// Init class variables.
			connection = 0;
			recvThread = 0;
			ContinueRecieving = false;
			recvCallbackFunc = 0;

			// Check for Winsock initialization.
			if (!SystemInitialized)
			{
				InitSystem();
			}

			// Create the native socket.
			if (AppSocketCount < MaxAllowedSockets)
			{
				create();
			}
			else
			{
				Log::Msg(String("The maximum allowed number of sockets is ") + MaxAllowedSockets + " and sockets currently in use by this app are " + AppSocketCount, Log::FatalError);
			}

			AppSocketCount++;
		}

		Socket::~Socket(void)
		{
			close();

			if (AppSocketCount == 0)
			{
				CleanupSystem();
			}
		}

		void Socket::Connect(IPAddress &remoteAddress, unsigned short remotePort)
		{
			struct sockaddr_in sockin;

			// Fill the struct.
			sockin.sin_family = AF_INET;
			sockin.sin_addr.s_addr = inet_addr( remoteAddress.toString() );
			sockin.sin_port = htons( remotePort );

			// Fill in internal storage variables.
			connectAddr = remoteAddress;
			RemotePort = remotePort;

			// Attempt a connection.
			if ( connect( connection, (const sockaddr *)&sockin, sizeof(sockin) ) == SOCKET_ERROR )
			{
				Log::Msg("Could not connect to the server.", Log::FatalError);
			}
			else
			{
				Log::Msg("Successfully connected to the server.", Log::Info);
			}
		}

		void Socket::RawSend(char *buf, unsigned int size)
		{
			send(connection, (const char *)buf, size, 0);
		}

		void Socket::Send(Packet *sndPack)
		{
			// Send size
			unsigned short sz = (unsigned short)sndPack->size();
			RawSend((char *)&sz, 2);
			// Send data
			for (unsigned int i = 0; i < sndPack->size(); i++)
			{
				RawSend((char *)&sndPack->Data[i], 1);
			}
		}

		void Socket::RawRecieve(char *buf, unsigned int size)
		{
			int bytesReceived = recv(connection, buf, size, 0);

			if (bytesReceived == SOCKET_ERROR || bytesReceived == 0)  // connection closed
			{
				Log::Msg("The connection with the server has been terminated.", Log::FatalError);
			}
		}

		unsigned char Socket::RawRecieve()
		{
			unsigned char ret = 0;

			RawRecieve((char *)&ret, 1);

			return ret;
		}

		void Socket::Recieve(Packet *rcvPack)
		{
			// Recieve size
			unsigned short sz = 0;
			unsigned char *psz = (unsigned char *)&sz;
			*psz = RawRecieve();
			*(psz + 1) = RawRecieve();
			// Recieve data
			for (unsigned short i = 0; i < sz; i++)
			{
				rcvPack->addByte(RawRecieve());
			}
		}

		void Socket::Bind(unsigned short port)
		{
			struct sockaddr_in sockin;

			sockin.sin_family = AF_INET;
			sockin.sin_addr.s_addr = inet_addr( "127.0.0.1" );
			sockin.sin_port = htons( port );

			if ( bind( connection, (const sockaddr *)&sockin, sizeof(sockin) ) == SOCKET_ERROR )
			{
				Log::Msg(String("Could not bind to port ") + port + ".", Log::FatalError);
			}
			else
			{
				Log::Msg(String("Successfully bound to port ") + port + ".", Log::Info);
			}
		}

		void Socket::Listen()
		{
			if ( listen( connection, SOMAXCONN ) == SOCKET_ERROR )
			{
				Log::Msg(String("Failed to listen on server. Error Code: ") + WSAGetLastError(), Log::FatalError);
			}
			else
			{
				Log::Msg("Listening for a connection.", Log::Info);
			}
		}

		void Socket::Accept(Socket *sock)
		{
			// Init a struct to hold the ip data.
			struct sockaddr_in sockin;
			int addrlen = sizeof(sockin);

			// Accept the connection.
			sock->connection = accept(connection, (sockaddr *)&sockin, &addrlen);

			// Fill in internal variables accordingly.
			sock->connectAddr.b1 = sockin.sin_addr.S_un.S_un_b.s_b1;
			sock->connectAddr.b2 = sockin.sin_addr.S_un.S_un_b.s_b2;
			sock->connectAddr.b3 = sockin.sin_addr.S_un.S_un_b.s_b3;
			sock->connectAddr.b4 = sockin.sin_addr.S_un.S_un_b.s_b4;
			sock->LocalPort = sockin.sin_port;

			// Output notification of accepting.
			Log::Msg(String("Server accepted connection: ") + GetRemoteAddress().toString() + ":" + GetLocalPort() + " | Currently connected clients: " + 1, Log::Info);
		}

		void Socket::startRecieveThread()
		{
			ContinueRecieving = true;
			recvThread = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&Socket::alwaysRecvFunc, this, 0, 0);
		}

		void Socket::stopRecieveThread()
		{
			ContinueRecieving = false;
			recvThread = 0;
		}

		IPAddress Socket::GetRemoteAddress()
		{
			return connectAddr;
		}

		unsigned short Socket::GetLocalPort()
		{
			return LocalPort;
		}

		unsigned short Socket::GetRemotePort()
		{
			return RemotePort;
		}

		void Socket::create()
		{
			connection = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

			if (connection == SOCKET_ERROR)
			{
				Log::Msg("Creating a socket failed.", Log::FatalError);
			}
			else
			{
				Log::Msg("Created a socket.", Log::Info);
			}
		}

		void Socket::close()
		{
			if (recvThread != 0)
			{
				TerminateThread(recvThread, 0);
			}

			closesocket(connection);
		}

		void Socket::alwaysRecvFunc(Socket *sck)
		{
			while (sck->ContinueRecieving)
			{
				Packet *pck = new Packet();
				sck->Recieve(pck);
				sck->recvCallbackFunc(sck, pck);
				delete pck;
			}
		}

		void Socket::InitSystem()
		{
			WSADATA wsaData;
			int error;

			error = WSAStartup(MAKEWORD(2, 0), &wsaData);

			if (error != 0)
			{
				Log::Msg(String("Failed to initialize the socket library. Error code: ") + error, Log::FatalError);
			}

			if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
			{
				WSACleanup();
				Log::Msg(String("Failed to acquire necessary socket library version. Recieved v") + LOBYTE(wsaData.wVersion) + "." + HIBYTE(wsaData.wVersion), Log::FatalError);
			}

			//MaxAllowedSockets = wsaData.iMaxSockets;
			Log::Msg("Successfully initialized the socket library.", Log::Info);
			SystemInitialized = true;
		}

		void Socket::CleanupSystem()
		{
			WSACleanup();
			SystemInitialized = false;
		}

		bool Socket::SystemInitialized = false;
		unsigned int Socket::AppSocketCount = 0;
		unsigned int Socket::MaxAllowedSockets = 4000000000;
	};
};