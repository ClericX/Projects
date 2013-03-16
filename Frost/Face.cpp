#define FROST_EXPORT
#include "Face.h"

#include "Log.h"
#include <Windows.h>
#include <gl/GL.h>

namespace Frost
{
	namespace Graphics3D
	{
		Face::Face(void)
		{
			ID = 0;
		}

		Face::~Face(void)
		{
		}

		void Face::Draw(MovableObject3D *pos, Material *mat)
		{
			bool BoundTexture = false;

			// Bind a texture if necessary.
			if (mat != 0 && mat->DiffuseMap != 0)
			{
				mat->DiffuseMap->Bind();
				BoundTexture = true;
			}

			// Determine the polygon type and begin interpreting it accordingly.
			if (Vertices.count() == 3)
			{
				glBegin(GL_TRIANGLES);
			}
			else if (Vertices.count() == 4)
			{
				glBegin(GL_QUADS);
			}
			else
			{
				Log::Msg("A face with an unrecognized number of vertices was attempted to be drawn.", Log::FatalError);
			}

			// Iterate through the Vector and draw each vertex.
			for (unsigned int i = 0; i < Vertices.count(); i++)
			{
				Vertices[i]->Draw(pos, (TexCoords.count() > i ? TexCoords[i] : 0), 0);
			}

			// End this vertex batch drawing.
			glEnd();

			// Unbind a texture if it was used.
			if (BoundTexture)
			{
				mat->DiffuseMap->Unbind();
			}
		}
	};
};