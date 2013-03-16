#define FROST_EXPORT
#include "SceneNode.h"

#include "SceneManager.h"
#include "Vector3f.h"

namespace Frost
{
	namespace Graphics3D
	{
		SceneNode::SceneNode(void)
		{
			ParentNode = 0;
		}

		SceneNode::~SceneNode(void)
		{
		}

		void SceneNode::attachEntity(Entity *ent)
		{
			Entities.Add(ent);
		}

		void SceneNode::detachEntity(Entity *ent)
		{
			for (unsigned int i = 0; i < Entities.count(); i++)
			{
				Entity *e = Entities[i];
				if (e == ent)
				{
					delete e;
					Entities.Remove(i);
				}
			}
		}

		void SceneNode::attachNode(SceneNode *n)
		{
			if (n->getParentNode() != this)
			{
				if (n->getParentNode() != 0)
				{
					n->getParentNode()->detachNode(n);
				}
				SubNodes.Add(n);
				n->setParentNode(this);
			}
		}

		void SceneNode::detachNode(SceneNode *n)
		{
			SubNodes.Remove(SubNodes.find(n));
			n->setParentNode(0);
		}

		SceneNode *SceneNode::getNode(unsigned int index)
		{
			return SubNodes[index];
		}

		SceneNode *SceneNode::getNode(String Name)
		{
			for (unsigned int i = 0; i < SubNodes.count(); i++)
			{
				if (SubNodes[i]->getName() == Name)
				{
					return SubNodes[i];
				}
			}
			return (SceneNode *)0;
		}

		SceneNode *SceneNode::getParentNode()
		{
			return ParentNode;
		}

		void SceneNode::setParentNode(SceneNode *n)
		{
			ParentNode = n;
		}

		Vector3f SceneNode::getNodeTreeLocation()
		{
			if (getParentNode() == 0)
			{
				return getLocation();
			}

			SceneNode *curNode = this;
			Vector3f retVal;

			while (curNode != SceneManager::getRootSceneNode())
			{
				retVal += curNode->getLocation();
				curNode = curNode->getParentNode();
			}

			return retVal;
		}

		void SceneNode::Draw(MovableObject3D *pos)
		{
			MovableObject3D newPos = (MovableObject3D)(*this) + (*pos);

			for (unsigned int i = 0; i < Entities.count(); i++)
			{
				Entities[i]->Draw(&newPos);
			}

			for (unsigned int i = 0; i < SubNodes.count(); i++)
			{
				getNode(i)->Draw(&newPos);
			}
		}
	};
};