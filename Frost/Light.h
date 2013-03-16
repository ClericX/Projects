#pragma once

#include "DLL.h"

#include "String.h"
#include "Color.h"
#include "NamedObject.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Light : public NamedObject
		{
		public:
			Light(void);
			~Light(void);

			Color Ambient;
			Color Diffuse;
			Color Specular;
		};
	};
};