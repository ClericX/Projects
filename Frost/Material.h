#pragma once

#include "DLL.h"

#include "String.h"
#include "Color.h"
#include "NamedObject.h"
#include "Texture.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Material : public NamedObject
		{
		public:
			Material(void);
			~Material(void);

			Color Ambient;
			Color Diffuse;
			Color Specular;
			int Illumination;
			float Intensity;
			float SpecularExponent;
			float Dissolve;
			float OpticalDensity; // Index of refraction.
			Texture *DiffuseMap;
		};
	};
};