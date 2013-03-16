#pragma once

#include "DLL.h"

#include "String.h"

namespace Frost
{
	class FROSTAPI NamedObject
	{
	public:
		NamedObject(void);
		~NamedObject(void);

		void setName(String Name);
		String getName();
		bool HasName();

	private:
		String Name;
	};
};