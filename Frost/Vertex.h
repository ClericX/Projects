#pragma once

#include "DLL.h"

#include "Vector3f.h"
#include "Color.h"
#include "MovableObject3D.h"
#include "TexCoord.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Vertex
		{
		public:
			Vertex(void);
			~Vertex(void);

			unsigned int ID;
			Vector3f Pos;

			void Draw(MovableObject3D *pos, TexCoord *vt, Color *clr);
		};
	};
};