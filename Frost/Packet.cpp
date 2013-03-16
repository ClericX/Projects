#define FROST_EXPORT
#include "Packet.h"

namespace Frost
{
	namespace Net
	{
		Packet::Packet(void)
		{
			ReadPos = 0;
		}

		Packet::~Packet(void)
		{
		}

		void Packet::addByte(unsigned char b)
		{
			Data.Add(b);
		}

		void Packet::addChar(char c)
		{
			addByte((unsigned char)c);
		}

		void Packet::addUShort(unsigned short s)
		{
			for (unsigned char x = 0; x < sizeof(unsigned short); x++)
			{
				addByte(*(((unsigned char *)&s) + x));
			}
		}

		void Packet::addShort(short s)
		{
			addUShort((unsigned short)s);
		}

		void Packet::addUInt(unsigned int i)
		{
			for (unsigned char x = 0; x < sizeof(unsigned int); x++)
			{
				addByte(*(((unsigned char *)&i) + x));
			}
		}

		void Packet::addInt(int i)
		{
			addUInt((unsigned int)i);
		}

		void Packet::addFloat(float f)
		{
			for (unsigned char x = 0; x < sizeof(float); x++)
			{
				addByte(*(((unsigned char *)&f) + x));
			}
		}

		void Packet::addDouble(double d)
		{
			for (unsigned char x = 0; x < sizeof(double); x++)
			{
				addByte(*(((unsigned char *)&d) + x));
			}
		}

		void Packet::addString(String str)
		{
			unsigned short sz = (unsigned short)str.length();

			addUShort(sz);

			for (unsigned short i = 0; i < sz; i++)
			{
				addChar(str.charAt(i));
			}
		}

		unsigned char Packet::getByte()
		{
			unsigned char ret = Data[ReadPos];
			ReadPos++;
			return ret;
		}

		char Packet::getChar()
		{
			return (char)getByte();
		}

		unsigned short Packet::getUShort()
		{
			unsigned short ret = 0;
			unsigned char *pRet = (unsigned char *)&ret;

			for (unsigned char x = 0; x < sizeof(unsigned short); x++)
			{
				*(pRet + x) = getByte();
			}

			return ret;
		}

		short Packet::getShort()
		{
			return (short)getUShort();
		}

		unsigned int Packet::getUInt()
		{
			unsigned int ret = 0;
			unsigned char *pRet = (unsigned char *)&ret;

			for (unsigned char x = 0; x < sizeof(unsigned int); x++)
			{
				*(pRet + x) = getByte();
			}

			return ret;
		}

		int Packet::getInt()
		{
			return (int)getUInt();
		}

		float Packet::getFloat()
		{
			float ret = 0;
			unsigned char *pRet = (unsigned char *)&ret;

			for (unsigned char x = 0; x < sizeof(float); x++)
			{
				*(pRet + x) = getByte();
			}

			return ret;
		}

		double Packet::getDouble()
		{
			double ret = 0;
			unsigned char *pRet = (unsigned char *)&ret;

			for (unsigned char x = 0; x < sizeof(double); x++)
			{
				*(pRet + x) = getByte();
			}

			return ret;
		}

		String Packet::getString()
		{
			String ret;
			unsigned short sz = getUShort();

			for (unsigned short i = 0; i < sz; i++)
			{
				ret += getChar();
			}

			return ret;
		}

		unsigned int Packet::size()
		{
			return Data.count();
		}
	};
};