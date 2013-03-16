#pragma once

#include "DLL.h"

#include "Vector.h"
#include "String.h"

namespace Frost
{
	namespace Net
	{
		class FROSTAPI Packet
		{
		public:
			Packet(void);
			~Packet(void);

			void addByte(unsigned char b);
			void addChar(char c);
			void addUShort(unsigned short s);
			void addShort(short s);
			void addUInt(unsigned int i);
			void addInt(int i);
			void addFloat(float f);
			void addDouble(double d);
			void addString(String str);

			unsigned char getByte();
			char getChar();
			unsigned short getUShort();
			short getShort();
			unsigned int getUInt();
			int getInt();
			float getFloat();
			double getDouble();
			String getString();

			unsigned int size();

			Vector<unsigned char> Data;
			unsigned int ReadPos;
		};
	};
};