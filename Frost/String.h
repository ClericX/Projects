#pragma once

#include "DLL.h"

namespace Frost
{
	class FROSTAPI String
	{
	public:

		char *Text;
		unsigned int Size;

		String(void);
		String(char *pText);
		String(String &pText);
		String(char aChar);
		String(int aNumber);
		String(unsigned int aNumber);
		String(float aFloat, int NumDecimals);
		~String(void);

		void Init();
		void resize(unsigned int NewSize);
		void setText(char *NewText);
		void setText(String &pText);
		void setText(char aChar);
		void setText(int aNumber);
		void setText(unsigned int aNumber);
		void setText(float aFloat, int NumDecimals);
		void addText(char *pText);
		void addText(String &pText);
		void addText(char aChar);
		void addText(int aNumber);
		void addText(unsigned int aNumber);
		void setChar(unsigned int index, char val);
		void Trim(unsigned int Amount);
		void Trim(unsigned int StartPos, unsigned int EndPos);
		void addLine(char *pText);
		void addLine(String &pText);
		unsigned int length();
		char &charAt(unsigned int Pos);
		String substring(unsigned int Begin, unsigned int End);
		char *c_str();
		unsigned int size();
		bool equals(char *pText);
		bool equals(String &pText);
		class StringList split(String delimiters);
		unsigned int count(char *what);
		unsigned int CountLines();
		bool empty();
		void clear();
		void replace(unsigned int Begin, unsigned int End, String Replacement);
		void replaceAll(String needle, String replacement);
		void insert(unsigned int atIndex, String Content);
		unsigned int find(String needle, unsigned int startIndex);
		unsigned int find(String needle);


		char &operator [] (const int nIndex);
		String &operator = (char *pText);
		String &operator = (String &pText);
		String &operator = (char aChar);
		String &operator = (int aNumber);
		String &operator = (unsigned int aNumber);
		String &operator += (char *pText);
		String &operator += (String &pText);
		String &operator += (char aChar);
		String &operator += (int aNumber);
		String &operator += (unsigned int aNumber);
		String &operator + (char *pText);
		String &operator + (String &pText);
		String &operator + (char aChar);
		String &operator + (int aNumber);
		String &operator + (unsigned int aNumber);
		String &operator << (char *pText);
		String &operator << (String &pText);
		String &operator << (char aChar);
		String &operator << (int aNumber);
		String &operator << (unsigned int aNumber);
		bool operator == (char *pText);
		bool operator == (String &pText);
		bool operator != (char *pText);
		bool operator != (String &pText);
		operator char *();

		int toInt();
		unsigned long hexToInt();
		double toDouble();
		float toFloat();
	};
};