#define FROST_EXPORT
#include "StringList.h"

namespace Frost
{
	StringList::StringList()
	{
	}

	StringList::~StringList()
	{
		for (unsigned int i = 0; i < strs.count(); i++)
		{
			delete strs[i];
		}
	}

	void StringList::Add(String &theString)
	{
		strs.Add(new String(theString));
	}

	void StringList::Add(char *theString)
	{
		String tmp(theString);
		Add(tmp);
	}

	unsigned int StringList::count()
	{
		return strs.count();
	}

	String &StringList::operator [] (int nIndex)
	{
		return (*strs[nIndex]);
	}
};