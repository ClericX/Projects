#pragma once

#include "DLL.h"

#include "List.h"
#include "NamedObject.h"
#include "Vector3f.h"
#include "MovableObject3D.h"
#include "Entity.h"

namespace Frost
{
	namespace Graphics3D
	{
		class FROSTAPI SceneNode : public NamedObject, public MovableObject3D
		{
		public:
			SceneNode(void);
			~SceneNode(void);

			void attachEntity(Entity *ent);
			void detachEntity(Entity *ent);
			void attachNode(SceneNode *n);
			void detachNode(SceneNode *n);
			SceneNode *getNode(unsigned int index);
			SceneNode *getNode(String Name);
			SceneNode *getParentNode();
			void setParentNode(SceneNode *n);
			Vector3f getNodeTreeLocation();
			void Draw(MovableObject3D *pos);

			SceneNode *ParentNode;
			List<SceneNode *> SubNodes;
			List<Entity *> Entities;
		};
	};
};