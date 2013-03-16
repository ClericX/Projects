#include "OgrePlatform.h"
#include "OgreString.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "Application.h"

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, INT nShowCmd)
//int main(int argc, char *argv[])
{
	try
	{
		//bool nograb = false;
		//if (argc >= 2 && Ogre::String(argv[1]) == "nograb") { nograb = true; }
		// somewhat hacky, but much simpler than other solutions
		//if (Ogre::String(cmdLine).find("nograb") != Ogre::String::npos) { nograb = true; }
		//OgreBites::SampleBrowser brows (nograb);
		//brows.go();

		Application app;
		app.go();
	}
	catch (Ogre::Exception& e)
	{
		MessageBoxA(NULL, e.getFullDescription().c_str(), "An exception has occurred!", MB_ICONERROR | MB_TASKMODAL);
	}

	return 0;
}