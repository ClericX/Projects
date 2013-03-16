#define FROST_EXPORT
#include "SceneManager.h"

namespace Frost
{
	namespace Graphics3D
	{
		namespace SceneManager
		{
			SceneNode *RootSceneNode = new SceneNode();
			List<Light *> Lights;
			List<Entity *> Entities;
			List<Animation *> Animations;
			List<Model *> Models;
			Color AmbientLight;

			Light *createLight(String Name)
			{
				Light *ret = new Light();
				ret->setName(Name);
				Lights.Add(ret);
				return ret;
			}

			Light *getLight(String Name)
			{
				for (unsigned int i = 0; i < Lights.count(); i++)
				{
					if (Lights[i]->getName() == Name)
					{
						return Lights[i];
					}
				}
				return (Light *)0;
			}

			void destroyLight(String Name)
			{
				destroyLight(getLight(Name));
			}

			void destroyLight(Light *light)
			{
				for (unsigned int i = 0; i < Lights.count(); i++)
				{
					if (Lights[i] == light)
					{
						Lights.Remove(i);
						delete light;
					}
				}
			}

			void destroyAllLights()
			{
				for (unsigned int i = 0; i < Lights.count(); i++)
				{
					delete Lights[i];
				}
				Lights.clear();
			}

			SceneNode *createSceneNode()
			{
				SceneNode *ret = new SceneNode();
				getRootSceneNode()->attachNode(ret);
				return ret;
			}

			SceneNode *createSceneNode(String Name)
			{
				SceneNode *ret = createSceneNode();
				ret->setName(Name);
				return ret;
			}

			void destroySceneNode(String Name)
			{
				SceneNode *node = getRootSceneNode()->getNode(Name);
				if (node)
				{
					delete node;
				}
				else
				{
					Log::Msg(String("Tried to delete a scene node that didn't exist! Name: ") + Name, Log::Error);
				}
			}

			void destroySceneNode(SceneNode *sn)
			{
				for (unsigned int i = 0; i < getRootSceneNode()->SubNodes.count(); i++)
				{
					SceneNode *curNode = getRootSceneNode()->SubNodes[i];
					if (curNode == sn)
					{
						delete curNode;
						getRootSceneNode()->SubNodes.Remove(i);
						break;
					}
				}
			}

			SceneNode *getRootSceneNode()
			{
				return RootSceneNode;
			}

			Model *loadModel(String Filename, String Modelname)
			{
				Model *ret = new Model();
				ret->setName(Modelname);
				ret->LoadOBJ(Filename);
				Models.Add(ret);
				return ret;
			}

			Model *getModel(String Name)
			{
				for (unsigned int i = 0; i < Models.count(); i++)
				{
					Model *getM = Models[i];
					if (getM->getName() == Name)
					{
						return getM;
					}
				}
				return (Model *)0;
			}

			Model *getModel(unsigned int index)
			{
				return Models[index];
			}

			Entity *createEntity(String EntityName, String ModelName)
			{
				return createEntity(EntityName, getModel(ModelName));
			}

			Entity *createEntity(String EntityName, Model *mdl)
			{
				if (!mdl)
				{
					Log::Msg("An entity was initialized with a null pointer for the mesh!", Log::Error);
				}

				Entity *ret = new Entity();
				ret->setName(EntityName);
				ret->setModel(mdl);
				Entities.Add(ret);
				return ret;
			}

			Entity *getEntity(String EntityName)
			{
				for (unsigned int i = 0; i < Entities.count(); i++)
				{
					if (Entities[i]->getName() == EntityName)
					{
						return Entities[i];
					}
				}
				return (Entity *)0;
			}

			void destroyEntity(Entity *ent)
			{
				for (unsigned int i = 0; i < Entities.count(); i++)
				{
					if (Entities[i] == ent)
					{
						delete Entities[i];
						Entities.Remove(i);
					}
				}
			}

			void destroyEntity(String EntityName)
			{
				destroyEntity(getEntity(EntityName));
			}

			void destroyAllEntities()
			{
				for (unsigned int i = 0; i < Entities.count(); i++)
				{
					delete Entities[i];
				}
				Entities.clear();
			}

			void clearScene()
			{
				// Remember to add other clears as things are added.
				// TODO: All node contents must also be cleared.
				destroyAllLights();
				destroyAllEntities();
			}

			void setAmbientLight(Color clr)
			{
				AmbientLight = clr;
			}

			Color getAmbientLight()
			{
				return AmbientLight;
			}

			Animation *createAnimation(String Name, double Length)
			{
				Animation *ret = new Animation();
				ret->setName(Name);
				ret->setLength(Length);
				Animations.Add(ret);
				return ret;
			}

			Animation *getAnimation(String Name)
			{
				for (unsigned int i = 0; i < Animations.count(); i++)
				{
					Animation *curAnim = Animations[i];
					
					if (curAnim->getName() == Name)
					{
						return curAnim;
					}
				}
				return (Animation *)0;
			}

			void destroyAnimation(String Name)
			{
				destroyAnimation(getAnimation(Name));
			}

			void destroyAnimation(Animation *anim)
			{
				for (unsigned int i = 0; i < Animations.count(); i++)
				{
					Animation *curAnim = Animations[i];
					
					if (curAnim == anim)
					{
						delete curAnim;
						Animations.Remove(i);
					}
				}
			}

			void destroyAllAnimations()
			{
				for (unsigned int i = 0; i < Animations.count(); i++)
				{
					delete Animations[i];
				}
				Animations.clear();
			}
		};
	};
};