#pragma once

#include "DLL.h"

#include "String.h"

namespace Frost
{
	namespace Log
	{
		enum FROSTAPI LogMessageType
		{
			Info,
			Debug,
			Warning,
			Error,
			FatalError
		};

		void FROSTAPI Msg(String msg, LogMessageType LogType);
	};
};