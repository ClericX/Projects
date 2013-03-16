#define FROST_EXPORT
#include "Entity.h"

namespace Frost
{
	namespace Graphics3D
	{
		Entity::Entity(void)
		{
		}

		Entity::~Entity(void)
		{
		}

		void Entity::setModel(Model *m)
		{
			mdl = m;
		}

		Model *Entity::getModel()
		{
			return mdl;
		}

		void Entity::Draw(MovableObject3D *pos)
		{
			MovableObject3D newPos = (MovableObject3D)(*this) + (*pos);

			getModel()->Draw(&newPos);
		}
	};
};