#include "InputController.h"

#include "CharacterController.h"

InputController *Ogre::Singleton<InputController>::msSingleton = 0;

InputController::InputController(void)
{
}

InputController::~InputController(void)
{
}

bool InputController::keyPressed(const OIS::KeyEvent& evt)
{
	if (CharacterController::getSingletonPtr() != 0)
	{
		CharacterController::getSingleton().injectKeyDown(evt);
	}

	return true;
}

bool InputController::keyReleased(const OIS::KeyEvent& evt)
{
	if (CharacterController::getSingletonPtr() != 0)
	{
		CharacterController::getSingleton().injectKeyUp(evt);
	}

	return true;
}

bool InputController::mouseMoved(const OIS::MouseEvent& evt)
{
	if (CharacterController::getSingletonPtr() != 0)
	{
		CharacterController::getSingleton().injectMouseMove(evt);
	}

	return true;
}

bool InputController::mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	if (CharacterController::getSingletonPtr() != 0)
	{
		CharacterController::getSingleton().injectMouseDown(evt, id);
	}

	return true;
}

bool InputController::mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	return true;
}