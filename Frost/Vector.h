#pragma once

#include <Windows.h>
#include "Log.h"

namespace Frost
{
	template <class T>
	class Vector
	{
	public:
		Vector(void)
		{
			NumElems = 0;
			ElemSize = sizeof(T);
			MemPtr = 0;
		}
		~Vector(void)
		{
			free(MemPtr);
		}

		unsigned char *MemPtr;
		unsigned int ElemSize;
		unsigned int NumElems;

		void Add(T elem)
		{
			// Allocate more memory for the new element.
			NumElems++;
			MemPtr = (unsigned char *)realloc(MemPtr, (ElemSize * NumElems));
		
			if (MemPtr == 0)
			{
				Log::Msg("A Vector memory realloc failed.", Log::FatalError);
			}

			(*this)[NumElems - 1] = elem;
		}

		T &operator [] (unsigned int index)
		{
			return ((T *)MemPtr)[index];
		}

		unsigned int count()
		{
			return NumElems;
		}
	};
};