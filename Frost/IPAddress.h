#pragma once

#include "DLL.h"

#include "String.h"

namespace Frost
{
	namespace Net
	{
		class FROSTAPI IPAddress
		{
		public:
			IPAddress(void);
			~IPAddress(void);

			IPAddress(String Addy);
			IPAddress(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4);

			unsigned char b1;
			unsigned char b2;
			unsigned char b3;
			unsigned char b4;

			String toString();

			static IPAddress ResolveHost(String hostname);

			static IPAddress GetLocalAddress();
			static IPAddress GetPublicAddress(unsigned int Timeout = 0);

			static IPAddress None;
			static IPAddress LocalHost;
		};
	};
};