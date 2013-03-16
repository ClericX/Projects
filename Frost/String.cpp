#define FROST_EXPORT
#include "String.h"
#include <Windows.h>
#include <stdio.h>
#include "StringList.h"

namespace Frost
{
	String::String(void)
	{
		Init();
	}

	String::String(char *pText)
	{
		Init();
		(*this) = pText;
	}

	String::String(String &pText)
	{
		Init();
		(*this) = pText;
	}

	String::String(char aChar)
	{
		Init();
		(*this) = aChar;
	}

	String::String(int aNumber)
	{
		Init();
		(*this) = aNumber;
	}

	String::String(unsigned int aNumber)
	{
		Init();
		(*this) = aNumber;
	}

	String::String(float aFloat, int NumDecimals)
	{
		Init();
		setText(aFloat, NumDecimals);
	}

	String::~String(void)
	{
		if (Text != 0)
		{
			free((void *)Text);
		}
	}

	void String::Init()
	{
		Text = 0;
		resize(1);
		Text[0] = '\0';
	}

	void String::resize(unsigned int NewSize)
	{
		char *NewMem = (char *)realloc((void *)Text, NewSize);
		if (NewMem == NULL)
		{
			String errmsg = "Allocating string object memory failed or NewSize was 0.\n";
			errmsg += "NewSize: ";
			errmsg += (int)NewSize;
			MessageBoxA(0, errmsg.c_str(), "Epic Fail", MB_OK);
		}
		else
		{
			Size = NewSize;
			Text = NewMem;
		}
	}

	void String::setText(char *NewText)
	{
		resize(strlen((const char *)NewText) + 1);
		strcpy_s(Text, size(), (const char *)NewText);
	}

	void String::setText(String &pText)
	{
		setText(pText.Text);
	}

	void String::setText(char aChar)
	{
		resize(2);
		Text[0] = aChar;
		Text[1] = '\0';
	}

	void String::setText(int aNumber)
	{
		resize(20);
		_itoa_s(aNumber, Text, size(), 10);
	}
	void String::setText(unsigned int aNumber)
	{
		resize(20);
		_itoa_s(aNumber, Text, size(), 10);
	}

	void String::setText(float aFloat, int NumDecimals)
	{
		resize(20);
		String Format = String("%.") + String(NumDecimals) + String("f");
		sprintf_s(Text, size(), Format.Text, aFloat);
	}

	void String::addText(char *pText)
	{
		resize(strlen((const char *)Text) + strlen((const char *)pText) + 1);
		strcat_s(Text, size(), (const char *)pText);
	}

	void String::addText(String &pText)
	{
		addText(pText.Text);
	}

	void String::addText(char aChar)
	{
		addText(String(aChar));
	}

	void String::addText(int aNumber)
	{
		addText(String(aNumber));
	}

	void String::addText(unsigned int aNumber)
	{
		addText(String(aNumber));
	}

	void String::setChar(unsigned int index, char val)
	{
		Text[index] = val;
	}

	void String::Trim(unsigned int Amount)
	{
		if (Amount > 0)
		{
			unsigned int NewSize = strlen((const char *)Text)-Amount+1;
			resize(NewSize);
			Text[NewSize-1] = '\0';
		}
	}

	void String::Trim(unsigned int StartPos, unsigned int EndPos)
	{
		//String derp;
		//derp << "Start: " << (int)StartPos << " | End: " << (int)EndPos;
		//MessageBoxA(0, derp.c_str(), derp.c_str(), MB_OK);
		if (StartPos > length() || StartPos < 0 || EndPos > length() || EndPos < 0)
		{
			MessageBoxA(0, "Tried to perform a Trim() that was out of bounds.", "Error!", MB_OK);
		}
		else
		{
			String Temp1 = substring(0, StartPos);
			String Temp2 = substring(EndPos, length());
			(*this) = Temp1;
			(*this) += Temp2;
		}
	}

	void String::addLine(char *pText)
	{
		addText(pText);
		addText("\n");
	}

	void String::addLine(String &pText)
	{
		addLine(pText.Text);
	}

	unsigned int String::length()
	{
		return strlen((const char *)Text);
	}

	char &String::charAt(unsigned int Pos)
	{
		return Text[Pos];
	}

	String String::substring(unsigned int Begin, unsigned int End)
	{
		if (End > length())
		{
			Log::Msg("Attempted a substring with the end bound being greater than the length of the string.", Log::FatalError);
		}

		String ret;

		for (unsigned int i = Begin; i < End; i++)
		{
			ret += Text[i];
		}

		return ret;
	}

	char *String::c_str()
	{
		return Text;
	}

	unsigned int String::size()
	{
		return Size;
	}

	bool String::equals(char *pText)
	{
		return (strcmp(Text, pText) == 0);
	}

	bool String::equals(String &pText)
	{
		return equals(pText.Text);
	}

	StringList String::split(String delimiters)
	{
		StringList ret;
		char *CurrentToken = 0;
		char *NextToken = 0;
		CurrentToken = strtok_s(Text, delimiters.c_str(), &NextToken);
		while (CurrentToken != 0)
		{
			ret.Add(CurrentToken);
			CurrentToken = strtok_s(0, delimiters.c_str(), &NextToken);
		}
		return ret;
	}

	unsigned int String::count(char *what)
	{
		unsigned int cnt = 0;
		for (unsigned int i = 0; i < length(); i++)
		{
			if (Text[i] == what[0])
			{
				cnt++;
			}
		}
		return cnt;
	}

	unsigned int String::CountLines()
	{
		return count("\n");
	}

	bool String::empty()
	{
		return length() == 0;
	}

	void String::clear()
	{
		(*this) = "";
	}

	void String::replace(unsigned int Begin, unsigned int End, String Replacement)
	{
		String before = substring(0, Begin);
		String after = substring(End, length());
		(*this) = before + Replacement + after;
	}

	void String::replaceAll(String needle, String replacement)
	{
		unsigned int curIndex = 0;

		while (curIndex = find(needle, curIndex) != (unsigned int)-1)
		{
			replace(curIndex, curIndex + needle.length(), replacement);
			curIndex -= (needle.length() - replacement.length());
		}
	}

	void String::insert(unsigned int atIndex, String Content)
	{
		replace(atIndex, atIndex, Content);
	}

	unsigned int String::find(String needle, unsigned int startIndex)
	{
		for (unsigned int i = 0; i < length(); i++)
		{
			for (unsigned int f = i; f < length(); f++)
			{
				unsigned int advancedIndex = f - i;
				if (charAt(f) != needle.charAt(advancedIndex))
				{
					break;
				}
				else if (advancedIndex == needle.length() - 1)
				{
					return i;
				}
			}
		}

		return (unsigned int)-1;
	}

	unsigned int String::find(String needle)
	{
		return find(needle, 0);
	}

	char &String::operator [] (const int nIndex)
	{
		return charAt(nIndex);
	}

	String &String::operator = (char *pText)
	{
		setText(pText);
		return (*this);
	}

	String &String::operator = (String &pText)
	{
		setText(pText);
		return (*this);
	}

	String &String::operator = (char aChar)
	{
		setText(aChar);
		return (*this);
	}

	String &String::operator = (int aNumber)
	{
		setText(aNumber);
		return (*this);
	}

	String &String::operator = (unsigned int aNumber)
	{
		setText(aNumber);
		return (*this);
	}

	String &String::operator += (char *pText)
	{
		addText(pText);
		return (*this);
	}

	String &String::operator += (String &pText)
	{
		addText(pText);
		return (*this);
	}

	String &String::operator += (char aChar)
	{
		addText(aChar);
		return (*this);
	}

	String &String::operator += (int aNumber)
	{
		addText(aNumber);
		return (*this);
	}

	String &String::operator += (unsigned int aNumber)
	{
		addText(aNumber);
		return (*this);
	}

	String &String::operator + (char *pText)
	{
		return ((*this) += pText);
	}

	String &String::operator + (String &pText)
	{
		return ((*this) += pText.Text);
	}

	String &String::operator + (char aChar)
	{
		return ((*this) += aChar);
	}

	String &String::operator + (int aNumber)
	{
		return ((*this) += aNumber);
	}

	String &String::operator + (unsigned int aNumber)
	{
		return ((*this) += aNumber);
	}

	String &String::operator << (char *pText)
	{
		return ((*this) += pText);
	}

	String &String::operator << (String &pText)
	{
		return ((*this) += pText.Text);
	}

	String &String::operator << (char aChar)
	{
		return ((*this) += aChar);
	}

	String &String::operator << (int aNumber)
	{
		return ((*this) += aNumber);
	}

	String &String::operator << (unsigned int aNumber)
	{
		return ((*this) += aNumber);
	}

	bool String::operator == (char *pText)
	{
		return equals(pText);
	}

	bool String::operator == (String &pText)
	{
		return equals(pText);
	}

	bool String::operator != (char *pText)
	{
		return (!((*this) == pText));
	}

	bool String::operator != (String &pText)
	{
		return (!((*this) == pText.Text));
	}

	String::operator char *()
	{
		return Text;
	}

	int String::toInt()
	{
		return atoi(Text);
	}

	unsigned long String::hexToInt()
	{
		const char* xs = this->c_str();
		unsigned int retVal = 0;
		unsigned int* result = &retVal;
		int i, xv, fact;

		if (length() > 0)
		{
			// Converting more than 32bit hexadecimal value?
			if (length()>8) return 0; // exit

			// Begin conversion here
			*result = 0;
			fact = 1;

			// Run until no more character to convert
			for(i=length()-1; i>=0 ;i--)
			{
				if (isxdigit(*(xs+i)))
				{
					if (*(xs+i)>=97)
					{
						xv = ( *(xs+i) - 97) + 10;
					}
					else if ( *(xs+i) >= 65)
					{
						xv = (*(xs+i) - 65) + 10;
					}
					else
					{
						xv = *(xs+i) - 48;
					}
					*result += (xv * fact);
					fact *= 16;
				}
				else
				{
				// Conversion was abnormally terminated
				// by non hexadecimal digit, hence
				// returning only the converted with
				// an error value 4 (illegal hex character)
				return 0;
				}
			}
		}

		// Nothing to convert
		//return 0;
		// Since the return value is now the result, these error codes are removed.
		return retVal;
	}

	double String::toDouble()
	{
		return atof(Text);
	}

	float String::toFloat()
	{
		return (float)toDouble();
	}
};