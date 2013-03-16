#define FROST_EXPORT
#include "Vertex.h"

#include <Windows.h>
#include <gl/GL.h>

namespace Frost
{
	namespace Graphics3D
	{
		Vertex::Vertex(void)
		{
			ID = 0;
		}

		Vertex::~Vertex(void)
		{
		}

		void Vertex::Draw(MovableObject3D *pos, TexCoord *vt, Color *clr)
		{
			if (clr != 0)
			{
				glColor4ub(clr->R, clr->G, clr->B, clr->A);
			}
			else
			{
				//glColor4ub(0xCC, 0xCC, 0xCC, 0xFF);
				glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
			}

			if (vt != 0)
			{
				glTexCoord2f(vt->u, vt->v);
			}

			Vector3f drawPos = pos->getLocation() + Pos;
			glPushMatrix();
			glRotatef(pos->getRoll(), 0, 0, 1);
			glRotatef(pos->getPitch(), 1, 0, 0);
			glRotatef(pos->getYaw(), 0, 1, 0);
			glVertex3f(drawPos.x, drawPos.y, drawPos.z);
			glPopMatrix();
		}
	};
};