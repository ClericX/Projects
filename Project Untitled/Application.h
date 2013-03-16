#pragma once

// Ogre-related includes

#include "Ogre.h"
#include "OIS.h"
#include "OgreRTShaderSystem.h"
#include "ShaderGeneratorListener.h"

// Application-specific includes

#include "MapGenerator.h"

class Application : public Ogre::Singleton<Application>
{
public:
	Application(void);
	~Application(void);

	// General application initialization functionality

	void go();
	void initApp();
	void createWindow();
	void setupInput();
	void createInputDevices();
	void locateResources();
	void loadResources();
	void shutdownInput();
	void closeApp();
	bool initializeRTShaderSystem(Ogre::SceneManager* sceneMgr);
	void finalizeRTShaderSystem();
	void loadRTShaderSystem();
	void setupContent();
	void cleanupContent();

	// Map generator-related functionality

	void initRandomMap();
	Ogre::SceneNode *createMapGenWall(unsigned int x, unsigned int y, WallPos wp);

	// Utility functionality

	bool fileExists(const Ogre::String& path);
	bool createDirectory(const Ogre::String& path);
	void findAppPath();
	void prepareUserHome(const Ogre::String& subdir);
	Ogre::String getConfigFilePath(Ogre::String filename);
	Ogre::String getWritablePath(Ogre::String filename);

	Ogre::String configPath;
	Ogre::String homePath;

	// Various necessary variable instances

	Ogre::RenderWindow *wnd;
	OIS::InputManager *ioMgr;
	OIS::Keyboard *keyboard;
	OIS::Mouse *mouse;
	Ogre::SceneManager *sceneMgr;
	Ogre::Camera *camera;
	Ogre::Viewport *viewport;
	Ogre::RTShader::ShaderGenerator*			mShaderGenerator;			// The Shader generator instance.
	ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;		// Shader generator material manager listener.
};