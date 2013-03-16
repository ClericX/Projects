#pragma once

#include "Ogre.h"

class WindowController : public Ogre::Singleton<WindowController>, public Ogre::FrameListener, public Ogre::WindowEventListener
{
public:
	WindowController(void);
	~WindowController(void);

	bool frameStarted(const Ogre::FrameEvent &evt);
	bool frameRenderingQueued(const Ogre::FrameEvent &evt);
	bool frameEnded(const Ogre::FrameEvent &evt);

	void windowMoved(Ogre::RenderWindow *rw);
	void windowResized(Ogre::RenderWindow *rw);
	bool windowClosing(Ogre::RenderWindow *rw);
	void windowClosed(Ogre::RenderWindow *rw);
	void windowFocusChange(Ogre::RenderWindow *rw);
};