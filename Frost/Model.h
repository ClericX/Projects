#pragma once

#include "DLL.h"

#include "Mesh.h"
#include "Material.h"
#include "NamedObject.h"
#include "MovableObject3D.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Model : public NamedObject
		{
		public:
			Model(void);
			~Model(void);

			List<Mesh *> meshList;
			List<Material *> matLib;

			Vertex *getVertex(unsigned int id);
			TexCoord *getTexCoord(unsigned int id);
			Material *getMaterial(String name);

			void Draw(MovableObject3D *pos);

			void LoadXML(String path);
			void LoadOBJ(String path);
			void LoadMTL(String path);
		};
	};
};