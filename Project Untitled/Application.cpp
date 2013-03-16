#include "Application.h"

#include "WindowController.h"
#include "InputController.h"
#include "CharacterController.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shlobj.h>
#include <io.h>

Application *Ogre::Singleton<Application>::msSingleton = 0;

Application::Application(void)
{
}

Application::~Application(void)
{
}

void Application::go()
{
	initApp();
	setupContent();
	initRandomMap();
	new CharacterController();

	if (Ogre::Root::getSingleton().getRenderSystem() != 0)
	{
		//Ogre::Root::getSingleton().startRendering();

		while (!wnd->isClosed())
		{
			keyboard->capture();
			mouse->capture();
			Ogre::Root::getSingleton().renderOneFrame();
			wnd->update(false);
			wnd->swapBuffers();
			Ogre::WindowEventUtilities::messagePump();
		}
	}
	else
	{
		Ogre::LogManager::getSingleton().logMessage("FATAL ERROR: Failed to acquire a render system.");
	}

	Ogre::Root::getSingleton().saveConfig();

	closeApp();
}

void Application::initApp()
{
	// init some filesystem stuff
	findAppPath();
	prepareUserHome(OGRE_VERSION_NAME);

	// Initialize OGRE root singleton
	OGRE_NEW Ogre::Root(getConfigFilePath("plugins.cfg"), getWritablePath("ogre.cfg"), getWritablePath("ogre.log"));

	// restore configuration settings
	if (!Ogre::Root::getSingleton().restoreConfig())
	{
		Ogre::LogManager::getSingleton().logMessage("FATAL ERROR: Failed to find configuration information.");
	}

	// output calculated necessary paths
	Ogre::LogManager::getSingleton().logMessage(configPath);
	Ogre::LogManager::getSingleton().logMessage(homePath);

	// Initialize controller class singletons
	new WindowController();
	new InputController();

	// create the app's main window
	createWindow();

	// clear all viewports -- probably not necessary
	wnd->removeAllViewports();

	// create the scene manager
	sceneMgr = Ogre::Root::getSingleton().createSceneManager(Ogre::ST_GENERIC);

	// setup the camera
	camera = sceneMgr->createCamera("MainCamera");
    // Position it at 500 in Z direction
    //camera->setPosition(Ogre::Vector3(0,0,20));
    // Look back along -Z
    //camera->lookAt(Ogre::Vector3(0,0,-10));
	camera->setNearClipDistance(0.1);
	camera->setFarClipDistance(100000);
	
	// setup the viewport
	viewport = wnd->addViewport(camera);
	// Alter the camera aspect ratio to match the viewport
	camera->setAspectRatio((Ogre::Real)viewport->getActualWidth() / (Ogre::Real)viewport->getActualHeight());

	// setup OIS
	setupInput();
	// setup the resource path configuration
	locateResources();
	// the RT shader system has to be loaded before the resources are loaded
	loadRTShaderSystem();
	// actually load all the resources
	loadResources();

	// set default mipmap level
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

	// add frame & window event listeners to the window from the window controller
	Ogre::Root::getSingleton().addFrameListener(WindowController::getSingletonPtr());
	Ogre::WindowEventUtilities::addWindowEventListener(wnd, WindowController::getSingletonPtr());
}

void Application::createWindow()
{
	wnd = Ogre::Root::getSingleton().initialise(true, "Project Untitled Client v0.01 ALPHA");
}

void Application::setupInput()
{
	OIS::ParamList pl;
	size_t winHandle = 0;
	std::ostringstream winHandleStr;

	wnd->getCustomAttribute("WINDOW", &winHandle);
	winHandleStr << winHandle;

	pl.insert(std::make_pair("WINDOW", winHandleStr.str()));
	if (/*nograb*/false)
	{
		pl.insert(std::make_pair("x11_keyboard_grab", "false"));
		pl.insert(std::make_pair("x11_mouse_grab", "false"));
		pl.insert(std::make_pair("w32_mouse", "DISCL_FOREGROUND"));
		pl.insert(std::make_pair("w32_mouse", "DISCL_NONEXCLUSIVE"));
		pl.insert(std::make_pair("w32_keyboard", "DISCL_FOREGROUND"));
		pl.insert(std::make_pair("w32_keyboard", "DISCL_NONEXCLUSIVE"));
	}

	ioMgr = OIS::InputManager::createInputSystem(pl);

	createInputDevices();      // create the specific input devices

	WindowController::getSingleton().windowResized(wnd);    // do an initial adjustment of mouse area
}

void Application::createInputDevices()
{
	keyboard = static_cast<OIS::Keyboard*>(ioMgr->createInputObject(OIS::OISKeyboard, true));
	mouse = static_cast<OIS::Mouse*>(ioMgr->createInputObject(OIS::OISMouse, true));

	keyboard->setEventCallback(InputController::getSingletonPtr());
	mouse->setEventCallback(InputController::getSingletonPtr());
}

void Application::locateResources()
{
	// load resource paths from config file
	Ogre::ConfigFile cf;
	cf.load(getConfigFilePath("resources.cfg"));

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
	Ogre::String sec, type, arch;

	// go through all specified resource groups
	while (seci.hasMoreElements())
	{
		sec = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap* settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;

		// go through all resource paths
		for (i = settings->begin(); i != settings->end(); i++)
		{
			type = i->first;
			arch = i->second;

			#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
			// OS X does not set the working directory relative to the app,
			// In order to make things portable on OS X we need to provide
			// the loading with it's own bundle path location
			if (!Ogre::StringUtil::startsWith(arch, "/", false)) // only adjust relative dirs
				arch = Ogre::String(Ogre::macBundlePath() + "/" + arch);
			#endif
			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch, type, sec);
		}
	}
}

void Application::loadResources()
{
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

void Application::shutdownInput()
{
	ioMgr->destroyInputObject(keyboard);
	ioMgr->destroyInputObject(mouse);

	OIS::InputManager::destroyInputSystem(ioMgr);
	ioMgr = 0;
}

void Application::closeApp()
{
	// cleanup content - nothing needed here
	// clear the scene
	sceneMgr->clearScene();

	// unload resources
	Ogre::ResourceGroupManager::ResourceManagerIterator resMgrs =
	Ogre::ResourceGroupManager::getSingleton().getResourceManagerIterator();

	while (resMgrs.hasMoreElements())
	{
		resMgrs.getNext()->unloadUnreferencedResources();
	}

	// remove shader generator from scene manager
	mShaderGenerator->removeSceneManager(sceneMgr);
	wnd->removeAllViewports();
	// destroy the scene manager
	Ogre::Root::getSingleton().destroySceneManager(sceneMgr);
	// destroy the rt shader generator
	finalizeRTShaderSystem();

	// remove window event listener before shutting down OIS
	Ogre::WindowEventUtilities::removeWindowEventListener(wnd, WindowController::getSingletonPtr());

	// Destroy OIS input devices and the input manager.
	shutdownInput();

	// destroy the root object instance
	OGRE_DELETE Ogre::Root::getSingletonPtr();
}

bool Application::initializeRTShaderSystem(Ogre::SceneManager* sceneMgr)
{			
	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();

		mShaderGenerator->addSceneManager(sceneMgr);

		// Setup core libraries and shader cache path.
		Ogre::StringVector groupVector = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
		Ogre::StringVector::iterator itGroup = groupVector.begin();
		Ogre::StringVector::iterator itGroupEnd = groupVector.end();
		Ogre::String shaderCoreLibsPath;
		Ogre::String shaderCachePath;
	
		for (; itGroup != itGroupEnd; ++itGroup)
		{
			Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
			Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
			Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
			bool coreLibsFound = false;

			// Try to find the location of the core shader lib functions and use it
			// as shader cache path as well - this will reduce the number of generated files
			// when running from different directories.
			for (; it != itEnd; ++it)
			{
				if ((*it)->archive->getName().find("RTShaderLib") != Ogre::String::npos)
				{
					shaderCoreLibsPath = (*it)->archive->getName() + "/cache/";
					shaderCachePath = shaderCoreLibsPath;
					coreLibsFound = true;
					break;
				}
			}
			// Core libs path found in the current group.
			if (coreLibsFound) 
				break; 
		}

		// Core shader libs not found -> shader generating will fail.
		if (shaderCoreLibsPath.empty())			
			return false;			

		// Set shader cache path.
		mShaderGenerator->setShaderCachePath(shaderCachePath);
		
		// Create and register the material manager listener if it doesn't exist yet.
		//if (mMaterialMgrListener == NULL) {
			mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);
			Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
		//}
	}

	return true;
}

void Application::finalizeRTShaderSystem()
{
	// Restore default scheme.
	Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);

	// Unregister the material manager listener.
	if (mMaterialMgrListener != NULL)
	{			
		Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
		delete mMaterialMgrListener;
		mMaterialMgrListener = NULL;
	}

	// Finalize RTShader system.
	if (mShaderGenerator != NULL)
	{				
		Ogre::RTShader::ShaderGenerator::finalize();
		mShaderGenerator = NULL;
	}
}

void Application::loadRTShaderSystem()
{
	// Initialize shader generator.
	// Must be before resource loading in order to allow parsing extended material attributes.
	bool success = initializeRTShaderSystem(sceneMgr);
	if (!success) 
	{
		OGRE_EXCEPT(Ogre::Exception::ERR_FILE_NOT_FOUND, 
			"Shader Generator Initialization failed - Core shader libs path not found", 
			"loadRTShaderSystem");
	}
	if(Ogre::Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_FIXED_FUNCTION) == false)
	{
		//newViewport->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
		
		// creates shaders for base material BaseWhite using the RTSS
		Ogre::MaterialPtr baseWhite = Ogre::MaterialManager::getSingleton().getByName("BaseWhite", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);				
		baseWhite->setLightingEnabled(false);
		mShaderGenerator->createShaderBasedTechnique(
			"BaseWhite", 
			Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
			Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);	
		mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, 
			"BaseWhite");
		baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
		baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
		baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
		baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());

		// creates shaders for base material BaseWhiteNoLighting using the RTSS
		mShaderGenerator->createShaderBasedTechnique(
			"BaseWhiteNoLighting", 
			Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
			Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);	
		mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, 
			"BaseWhiteNoLighting");
		Ogre::MaterialPtr baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
		baseWhiteNoLighting->getTechnique(0)->getPass(0)->setVertexProgram(
		baseWhiteNoLighting->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
		baseWhiteNoLighting->getTechnique(0)->getPass(0)->setFragmentProgram(
		baseWhiteNoLighting->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
	}
}

void Application::setupContent()
{
	//mSceneMgr->setSkyBox(true, "Examples/SpaceSkyBox");
	sceneMgr->setSkyDome(true, "Examples/CloudySky");

	// set background and some fog
	viewport->setBackgroundColour(Ogre::ColourValue(1.0f, 1.0f, 0.8f));
	//mSceneMgr->setFog(Ogre::FOG_LINEAR, ColourValue(1.0f, 1.0f, 0.8f), 0, 15, 100);

	// set shadow properties
	sceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_TEXTURE_MODULATIVE);
	sceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));
	sceneMgr->setShadowTextureSize(1024);
	sceneMgr->setShadowTextureCount(1);

	// use a small amount of ambient lighting
	sceneMgr->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));

	// add a bright light above the scene
	Ogre::Light* light = sceneMgr->createLight();
	light->setType(Ogre::Light::LT_POINT);
	light->setPosition(-10, 40, 20);
	light->setSpecularColour(Ogre::ColourValue::White);

	// create a floor mesh resource
	Ogre::MeshManager::getSingleton().createPlane("floor", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 1000, 1000, 10, 10, true, 1, 10, 10, Ogre::Vector3::UNIT_Z);

	// create a floor entity, give it a material, and place it at the origin
	Ogre::Entity* floor = sceneMgr->createEntity("Floor", "floor");
	floor->setMaterialName("Examples/Rockwall");
	floor->setCastShadows(false);
	sceneMgr->getRootSceneNode()->attachObject(floor);

	// add necessary resource path
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Frost");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(configPath + "\\res\\", "FileSystem", "Frost");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Frost");
	Ogre::ResourceGroupManager::getSingleton().loadResourceGroup("Frost");
}

void Application::cleanupContent()
{
	Ogre::MeshManager::getSingleton().remove("floor");
}

void Application::initRandomMap()
{
	// initialize wall mesh and create all wall nodes for the randomly generated map
	MapGenerator map(5, 5);

	for (unsigned int h = 0; h < map.getHeight(); h++)
	{
		for (unsigned int w = 0; w < map.getWidth(); w++)
		{
			for (unsigned int i = 0; i < WallPos::Count; i++)
			{
				if (map.hasWallAt(w, h, (WallPos)i))
				{
					createMapGenWall(w, h, (WallPos)i);
				}
			}

			// add check for start point / end point / map end
		}
	}
}

Ogre::SceneNode *Application::createMapGenWall(unsigned int x, unsigned int y, WallPos wp)
{
	std::stringstream wallName;
	wallName << "wall-" << x << "-" << y << "-" << (int)wp;
	Ogre::LogManager::getSingleton().logMessage(wallName.str());
	Ogre::SceneNode *ret = sceneMgr->getRootSceneNode()->createChildSceneNode(wallName.str());
	Ogre::Entity *wallEnt = sceneMgr->createEntity(wallName.str(), "wall.mesh");
	ret->attachObject(wallEnt);
	Ogre::Real wallWidth = wallEnt->getBoundingBox().getSize().x;
	Ogre::Real wallHeight = wallEnt->getBoundingBox().getSize().z;
	//ret->setScale(4, 4, 4);
	ret->setPosition(x * wallWidth * 3, 0, y * wallWidth * 3);
	
	Ogre::Vector3 trans(0, 0, 0);
	Ogre::Real deg(0);

	switch (wp)
	{
	case WallPos::UpLeft:
		deg = 225;
		trans.x = wallWidth;
		break;

	case WallPos::Up:
		trans.z = wallWidth;
		break;

	case WallPos::UpRight:
		deg = 45;
		trans.z = wallWidth * 2;
		break;

	case WallPos::Left:
		deg = 270;
		trans.x = wallWidth;
		break;

	case WallPos::Right:
		deg = 270;
		trans.x = wallWidth;
		trans.z = wallWidth * 2;
		break;

	case WallPos::DownLeft:
		deg = 45;
		trans.x = wallWidth * 2;
		break;

	case WallPos::Down:
		trans.x = wallWidth * 2;
		trans.z = wallWidth;
		break;

	case WallPos::DownRight:
		deg = 45;
		trans.x = wallWidth * 2;
		trans.z = wallWidth * 2;
		break;
	}

	ret->translate(trans);
	ret->yaw(Ogre::Degree(deg));
	
	return ret;
}

bool Application::fileExists(const Ogre::String& path)
{
	return _access(path.c_str(), 00) == 0;
}

bool Application::createDirectory(const Ogre::String& path)
{
	return CreateDirectory(path.c_str(), NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS;
}

void Application::findAppPath()
{
	// try to determine the application's path
	DWORD bufsize = 256;
	char* resolved = 0;
	do
	{
		char* buf = OGRE_ALLOC_T(char, bufsize, Ogre::MEMCATEGORY_GENERAL);
		DWORD retval = GetModuleFileName(NULL, buf, bufsize);
		if (retval == 0)
		{
			// failed
			OGRE_FREE(buf, Ogre::MEMCATEGORY_GENERAL);
			break;
		}

		if (retval < bufsize)
		{
			// operation was successful.
			resolved = buf;
		}
		else
		{
			// buffer was too small, grow buffer and try again
			OGRE_FREE(buf, Ogre::MEMCATEGORY_GENERAL);
			bufsize <<= 1;
		}
	} while (!resolved);

	Ogre::String appPath = resolved;
	if (resolved)
		OGRE_FREE(resolved, Ogre::MEMCATEGORY_GENERAL);
	if (!appPath.empty())
	{
		// need to strip the application filename from the path
		Ogre::String::size_type pos = appPath.rfind('\\');
		if (pos != Ogre::String::npos)
			appPath.erase(pos);
	}
	else
	{
		// fall back to current working dir
		appPath = ".";
	}

	// use application path as config search path
	configPath = (appPath + '\\');
}

void Application::prepareUserHome(const Ogre::String& subdir)
{
	TCHAR path[MAX_PATH];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL|CSIDL_FLAG_CREATE, NULL, 0, path)))
	{
		// need to convert to OEM codepage so that fstream can use
		// it properly on international systems.
		TCHAR oemPath[MAX_PATH];
		CharToOem(path, oemPath);
		homePath = oemPath;
		// create Ogre subdir
		homePath += "\\Ogre\\";
		if (! CreateDirectory(homePath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
		{
			// couldn't create directory, fall back to current working dir
			homePath.clear();
		}
		else
		{
			homePath += subdir + '\\';
			// create release subdir
			if (! CreateDirectory(homePath.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
			{
				// couldn't create directory, fall back to current working dir
				homePath.clear();
			}
		}
	}

	if (homePath.empty())
	{
		// couldn't create dir in home directory, fall back to cwd
		homePath = "";
	}
}

Ogre::String Application::getConfigFilePath(Ogre::String filename)
{
#if OGRE_DEBUG_MODE == 1
	// add _d suffix to config files
	Ogre::String::size_type pos = filename.rfind('.');
	if (pos != Ogre::String::npos)
		filename = filename.substr(0, pos) + "_d" + filename.substr(pos);
#endif
	// look for the requested file in several locations:

	// 1. in the writable path (so user can provide custom files)
	Ogre::String path = getWritablePath(filename);
	if (fileExists(path))
		return path;

	// 2. in the config file search path
	path = configPath + filename;
	if (fileExists(path))
		return path;

	// 3. fallback to current working dir
	return filename;
}

Ogre::String Application::getWritablePath(Ogre::String filename)
{
	return homePath + filename;
}