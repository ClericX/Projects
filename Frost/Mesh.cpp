#define FROST_EXPORT
#include "Mesh.h"

namespace Frost
{
	namespace Graphics3D
	{
		Mesh::Mesh(void)
		{
			usedMaterial = 0;
		}

		Mesh::~Mesh(void)
		{
		}

		void Mesh::Draw(MovableObject3D *pos)
		{
			for (unsigned int i = 0; i < Faces.count(); i++)
			{
				Faces[i]->Draw(pos, usedMaterial);
			}
		}
	};
};