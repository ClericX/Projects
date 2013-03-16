#define FROST_EXPORT
#include "Log.h"

#include <Windows.h>
#include <iostream>

namespace Frost
{
	namespace Log
	{
		void Msg(String msg, LogMessageType LogType)
		{
			String TimePrefix;
			String NotificationTypePrefix;

			TimePrefix = String((long)GetTickCount());

			switch (LogType)
			{
			case Info:
				{
					NotificationTypePrefix = "[INFO]";
				}
				break;

			case Debug:
				{
					NotificationTypePrefix = "[DEBUG]";
				}
				break;

			case Warning:
				{
					NotificationTypePrefix = "[WARNING]";
				}
				break;

			case Error:
				{
					NotificationTypePrefix = "[ERROR]";
				}
				break;

			case FatalError:
				{
					NotificationTypePrefix = "[FATAL]";
				}
				break;
			}

			std::cout << TimePrefix << " " << NotificationTypePrefix << " " << msg << std::endl;

			if (LogType == FatalError)
			{
				MessageBoxA(0, "A fatal error occurred, please check the log.", "Fatal", MB_OK);
				Sleep(5000);
				TerminateProcess(GetCurrentProcess(), 1);
			}
		}
	}
};