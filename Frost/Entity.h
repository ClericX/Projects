#pragma once

#include "DLL.h"

#include "Vector3f.h"
#include "List.h"
#include "Model.h"
#include "String.h"
#include "NamedObject.h"
#include "MovableObject3D.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI Entity : public NamedObject, public MovableObject3D
		{
		public:
			Entity(void);
			~Entity(void);

			Vector3f Pos;
			Model *mdl;

			void setModel(Model *m);
			Model *getModel();
			void Draw(MovableObject3D *pos);
		};
	};
};