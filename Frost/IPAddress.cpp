#define FROST_EXPORT
#include "IPAddress.h"
#include "StringList.h"

namespace Frost
{
	namespace Net
	{
		IPAddress::IPAddress(void)
		{
		}

		IPAddress::~IPAddress(void)
		{
		}

		IPAddress::IPAddress(String Addy)
		{
			StringList nums = Addy.split(".");
			
			b1 = atoi(nums[0]);
			b2 = atoi(nums[1]);
			b3 = atoi(nums[2]);
			b4 = atoi(nums[3]);
		}

		IPAddress::IPAddress(unsigned char byte1, unsigned char byte2, unsigned char byte3, unsigned char byte4)
		{
			b1 = byte1;
			b2 = byte2;
			b3 = byte3;
			b4 = byte4;
		}

		String IPAddress::toString()
		{
			return (String(b1) + "." + b2 + "." + b3 + "." + b4);
		}

		IPAddress IPAddress::ResolveHost(String hostname)
		{
			struct hostent *inf = gethostbyname(hostname);

			if (inf->h_addr_list[0] != 0)
			{
				return IPAddress(inf->h_addr_list[0][0], inf->h_addr_list[0][1], inf->h_addr_list[0][2], inf->h_addr_list[0][3]);
			}
			else
			{
				Log::Msg(String("Unable to resolve hostname \"") + hostname + "\"", Log::FatalError);
			}

			return IPAddress();
		}

		IPAddress IPAddress::GetLocalAddress()
		{
			char acquiredHostName[256];
			gethostname(acquiredHostName, 256);
			return ResolveHost(String(acquiredHostName));
		}
	};
};