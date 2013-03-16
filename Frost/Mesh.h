#pragma once

#include "DLL.h"

#include "String.h"
#include "Vertex.h"
#include "Face.h"
#include "Vector.h"
#include "NamedObject.h"
#include "MovableObject3D.h"
#include "TexCoord.h"
#include "Material.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Mesh : public NamedObject
		{
		public:
			Mesh(void);
			~Mesh(void);

			// Model-wide banks.
			Vector<Vertex *> Vertices;
			Vector<TexCoord *> TexCoords;

			// Mesh-specific bank.
			Vector<Face *> Faces;

			// Material used for the whole mesh when drawn.
			// Probably needs to be moved since meshes shouldn't be directly associated with a material.
			Material *usedMaterial;

			void Draw(MovableObject3D *pos);
		};
	};
};