#pragma once

#include "DLL.h"

#include "String.h"
#include "IPAddress.h"
#include "Packet.h"

#include <Windows.h>

namespace Frost
{
	namespace Net
	{
		class FROSTAPI Socket
		{
		public:
			Socket(void);
			~Socket(void);

			void Connect(IPAddress &remoteAddress, unsigned short remotePort);
			void RawSend(char *buf, unsigned int size);
			void Send(Packet *sndPack);
			void RawRecieve(char *buf, unsigned int size);
			unsigned char RawRecieve();
			void Recieve(Packet *rcvPack);
			void Bind(unsigned short port);
			void Listen();
			void Accept(Socket *sock);
			void startRecieveThread();
			void stopRecieveThread();

			IPAddress GetRemoteAddress();
			unsigned short GetLocalPort();
			unsigned short GetRemotePort();

			SOCKET connection;
			HANDLE recvThread;
			bool ContinueRecieving;

			IPAddress connectAddr;
			unsigned short LocalPort;
			unsigned short RemotePort;

			void (*recvCallbackFunc)(Socket *, Packet *);

		private:
			void create();
			void close();

			static void alwaysRecvFunc(Socket *sck);
			static void InitSystem();
			static void CleanupSystem();

			static bool SystemInitialized;
			static unsigned int AppSocketCount;
			static unsigned int MaxAllowedSockets;
		};
	};
};