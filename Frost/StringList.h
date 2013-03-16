#pragma once

#include "DLL.h"
#include "String.h"
#include "List.h"

namespace Frost
{
	class FROSTAPI StringList
	{
	public:
		StringList();
		~StringList();

		List<String *> strs;

		void Add(String &theString);
		void Add(char *theString);
		unsigned int count();
		String &operator [] (int nIndex);
	};
};