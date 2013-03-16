#include "WindowController.h"

#include "CharacterController.h"

WindowController *Ogre::Singleton<WindowController>::msSingleton = 0;

WindowController::WindowController(void)
{
}

WindowController::~WindowController(void)
{
}

bool WindowController::frameStarted(const Ogre::FrameEvent &evt)
{
	if (CharacterController::getSingletonPtr() != 0)
	{
		CharacterController::getSingleton().updateBody(evt.timeSinceLastFrame);
		CharacterController::getSingleton().updateCamera(evt.timeSinceLastFrame);
	}

	return true;
}

bool WindowController::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
	return true;
}

bool WindowController::frameEnded(const Ogre::FrameEvent &evt)
{
	return true;
}

void WindowController::windowMoved(Ogre::RenderWindow *rw)
{
	//
}

void WindowController::windowResized(Ogre::RenderWindow *rw)
{
	//
}

bool WindowController::windowClosing(Ogre::RenderWindow *rw)
{
	return true;
}

void WindowController::windowClosed(Ogre::RenderWindow *rw)
{
	//
}

void WindowController::windowFocusChange(Ogre::RenderWindow *rw)
{
	//
}