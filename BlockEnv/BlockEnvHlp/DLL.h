#pragma once

#if defined(FROST_EXPORT) // inside DLL
#   define FROSTAPI   __declspec(dllexport)
#else // outside DLL
#   define FROSTAPI   __declspec(dllimport)
#endif  // FROST_EXPORT

#pragma warning( disable: 4251 )