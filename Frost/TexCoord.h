#pragma once

#include "DLL.h"

namespace Frost
{
	namespace Graphics3D
	{
		class TexCoord
		{
		public:
			TexCoord(void);
			~TexCoord(void);

			unsigned int ID;
			float u;
			float v;
		};
	};
};