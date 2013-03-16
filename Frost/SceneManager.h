#pragma once

#include "DLL.h"
#include "String.h"
#include "Light.h"
#include "SceneNode.h"
#include "Entity.h"
#include "Model.h"
#include "Color.h"
#include "Animation.h"
#include "List.h"

namespace Frost
{
	namespace Graphics3D
	{
		namespace SceneManager
		{
			extern "C" FROSTAPI SceneNode *RootSceneNode;
			extern "C" FROSTAPI List<Light *> Lights;
			extern "C" FROSTAPI List<Entity *> Entities;
			extern "C" FROSTAPI List<Animation *> Animations;
			extern "C" FROSTAPI List<Model *> Models;
			extern "C" FROSTAPI Color AmbientLight;

			Light FROSTAPI *createLight(String Name);
			Light FROSTAPI *getLight(String Name);
			void FROSTAPI destroyLight(String Name);
			void FROSTAPI destroyLight(Light *light);
			void FROSTAPI destroyAllLights();
			SceneNode FROSTAPI *createSceneNode();
			SceneNode FROSTAPI *createSceneNode(String Name);
			void FROSTAPI destroySceneNode(String Name);
			void FROSTAPI destroySceneNode(SceneNode *sn);
			SceneNode FROSTAPI *getRootSceneNode();
			Model FROSTAPI *loadModel(String Filename, String Modelname);
			Model FROSTAPI *getModel(String Name);
			Model FROSTAPI *getModel(unsigned int index);
			Entity FROSTAPI *createEntity(String EntityName, String ModelName);
			Entity FROSTAPI *createEntity(String EntityName, Model *mdl);
			Entity FROSTAPI *getEntity(String EntityName);
			void FROSTAPI destroyEntity(Entity *ent);
			void FROSTAPI destroyEntity(String EntityName);
			void FROSTAPI destroyAllEntities();
			void FROSTAPI clearScene();
			void FROSTAPI setAmbientLight(Color clr);
			Color FROSTAPI getAmbientLight();
			Animation FROSTAPI *createAnimation(String Name, double Length);
			Animation FROSTAPI *getAnimation(String Name);
			void FROSTAPI destroyAnimation(String Name);
			void FROSTAPI destroyAnimation(Animation *anim);
			void FROSTAPI destroyAllAnimations();
		};
	};
};