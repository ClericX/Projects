#pragma once

#include "DLL.h"

#include "Vector.h"
#include "Vertex.h"
#include "MovableObject3D.h"
#include "TexCoord.h"
#include "Material.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Face
		{
		public:
			Face(void);
			~Face(void);

			unsigned int ID;
			Vector<Vertex *> Vertices;
			Vector<TexCoord *> TexCoords;

			void Draw(MovableObject3D *pos, Material *mat);
		};
	};
};