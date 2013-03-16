#pragma once

#include "Ogre.h"
#include "OIS.h"

class InputController : public Ogre::Singleton<InputController>, public OIS::KeyListener, public OIS::MouseListener
{
public:
	InputController(void);
	~InputController(void);

	bool keyPressed(const OIS::KeyEvent& evt);
	bool keyReleased(const OIS::KeyEvent& evt);

	bool mouseMoved(const OIS::MouseEvent& evt);
	bool mousePressed(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent& evt, OIS::MouseButtonID id);
};