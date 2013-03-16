#include "CharacterController.h"

#include "Application.h"

CharacterController *Ogre::Singleton<CharacterController>::msSingleton = 0;

CharacterController::CharacterController(void)
{
	setupBody();
	setupCamera();
}

CharacterController::~CharacterController(void)
{
}

void CharacterController::setupBody()
{
	// create main model
	mBodyNode = Application::getSingleton().sceneMgr->getRootSceneNode()->createChildSceneNode(Ogre::Vector3::UNIT_Y * CHAR_HEIGHT);
	mBodyEnt = Application::getSingleton().sceneMgr->createEntity("SinbadBody", "Sinbad.mesh");
	mBodyNode->attachObject(mBodyEnt);

	// create swords and attach to sheath
	mSword1 = Application::getSingleton().sceneMgr->createEntity("SinbadSword1", "Sword.mesh");
	mSword2 = Application::getSingleton().sceneMgr->createEntity("SinbadSword2", "Sword.mesh");
	mBodyEnt->attachObjectToBone("Sheath.L", mSword1);
	mBodyEnt->attachObjectToBone("Sheath.R", mSword2);

	// create a couple of ribbon trails for the swords, just for fun
	Ogre::NameValuePairList params;
	params["numberOfChains"] = "2";
	params["maxElements"] = "80";
	mSwordTrail = (Ogre::RibbonTrail *)Application::getSingleton().sceneMgr->createMovableObject("RibbonTrail", &params);
	mSwordTrail->setMaterialName("Examples/LightRibbonTrail");
	mSwordTrail->setTrailLength(20);
	mSwordTrail->setVisible(false);
	Application::getSingleton().sceneMgr->getRootSceneNode()->attachObject(mSwordTrail);


	for (int i = 0; i < 2; i++)
	{
		mSwordTrail->setInitialColour(i, 1, 0.8, 0);
		mSwordTrail->setColourChange(i, 0.75, 1.25, 1.25, 1.25);
		mSwordTrail->setWidthChange(i, 1);
		mSwordTrail->setInitialWidth(i, 0.5);
	}

	mKeyDirection = Ogre::Vector3::ZERO;
	mVerticalVelocity = 0;
}

void CharacterController::setupCamera()
{
	// create a pivot at roughly the character's shoulder
	mCameraPivot = Application::getSingleton().camera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	// this is where the camera should be soon, and it spins around the pivot
	mCameraGoal = mCameraPivot->createChildSceneNode(Ogre::Vector3(0, 0, 15));
	// this is where the camera actually is
	mCameraNode = Application::getSingleton().camera->getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mCameraNode->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

	mCameraPivot->setFixedYawAxis(true);
	mCameraGoal->setFixedYawAxis(true);
	mCameraNode->setFixedYawAxis(true);

	// our model is quite small, so reduce the clipping planes
	//cam->setNearClipDistance(0.1);
	//cam->setFarClipDistance(100);
	mCameraNode->attachObject(Application::getSingleton().camera);

	mPivotPitch = 0;
}

void CharacterController::updateBody(Ogre::Real deltaTime)
{
	mGoalDirection = Ogre::Vector3::ZERO;   // we will calculate this

	if (mKeyDirection != Ogre::Vector3::ZERO && mBaseAnimID != ANIM_DANCE)
	{
		// calculate actually goal direction in world based on player's key directions
		mGoalDirection += mKeyDirection.z * mCameraNode->getOrientation().zAxis();
		mGoalDirection += mKeyDirection.x * mCameraNode->getOrientation().xAxis();
		mGoalDirection.y = 0;
		mGoalDirection.normalise();
		/*
		Ogre::Quaternion toGoal = mBodyNode->getOrientation().zAxis().getRotationTo(mGoalDirection);

		// calculate how much the character has to turn to face goal direction
		Ogre::Real yawToGoal = toGoal.getYaw().valueDegrees();
		// this is how much the character CAN turn this frame
		Ogre::Real yawAtSpeed = yawToGoal / Ogre::Math::Abs(yawToGoal) * deltaTime * TURN_SPEED;
		// reduce "turnability" if we're in midair
		if (mBaseAnimID == ANIM_JUMP_LOOP) yawAtSpeed *= 0.2f;

		// turn as much as we can, but not more than we need to
		if (yawToGoal < 0) yawToGoal = std::min<Ogre::Real>(0, std::max<Ogre::Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, yawAtSpeed, 0);
		else if (yawToGoal > 0) yawToGoal = std::max<Ogre::Real>(0, std::min<Ogre::Real>(yawToGoal, yawAtSpeed)); //yawToGoal = Math::Clamp<Real>(yawToGoal, 0, yawAtSpeed);
			
		mBodyNode->yaw(Ogre::Degree(yawToGoal));
		*/
		// move in current body direction (not the goal direction)
		mBodyNode->translate(0, 0, deltaTime * RUN_SPEED/* * mAnims[mBaseAnimID]->getWeight()*/, Ogre::Node::TS_LOCAL);
	}

	if (mBaseAnimID == ANIM_JUMP_LOOP)
	{
		// if we're jumping, add a vertical offset too, and apply gravity
		mBodyNode->translate(0, mVerticalVelocity * deltaTime, 0, Ogre::Node::TS_LOCAL);
		mVerticalVelocity -= GRAVITY * deltaTime;
			
		Ogre::Vector3 pos = mBodyNode->getPosition();
		if (pos.y <= CHAR_HEIGHT)
		{
			// if we've hit the ground, change to landing state
			pos.y = CHAR_HEIGHT;
			mBodyNode->setPosition(pos);
			//setBaseAnimation(ANIM_JUMP_END, true);
			mTimer = 0;
		}
	}
}

void CharacterController::updateCamera(Ogre::Real deltaTime)
{
	// place the camera pivot roughly at the character's shoulder
	mCameraPivot->setPosition(mBodyNode->getPosition() + Ogre::Vector3::UNIT_Y * CAM_HEIGHT);
	// move the camera smoothly to the goal
	Ogre::Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - mCameraNode->getPosition();
	mCameraNode->translate(goalOffset * deltaTime * 9.0f);
	// always look at the pivot
	mCameraNode->lookAt(mCameraPivot->_getDerivedPosition(), Ogre::Node::TS_WORLD);
}

void CharacterController::updateCameraGoal(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom)
{
	mCameraPivot->yaw(Ogre::Degree(deltaYaw), Ogre::Node::TS_WORLD);

	// bound the pitch
	if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
		!(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
	{
		mCameraPivot->pitch(Ogre::Degree(deltaPitch), Ogre::Node::TS_LOCAL);
		mPivotPitch += deltaPitch;
	}

	Ogre::Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
	Ogre::Real distChange = deltaZoom * dist;

	// bound the zoom
	if (!(dist + distChange < 8 && distChange < 0) &&
		!(dist + distChange > 25 && distChange > 0))
	{
		mCameraGoal->translate(0, 0, distChange, Ogre::Node::TS_LOCAL);
	}
}

void CharacterController::injectKeyDown(const OIS::KeyEvent& evt)
{
	// keep track of the player's intended direction
	if (evt.key == OIS::KC_W) mKeyDirection.z = -1;
	else if (evt.key == OIS::KC_A) mKeyDirection.x = -1;
	else if (evt.key == OIS::KC_S) mKeyDirection.z = 1;
	else if (evt.key == OIS::KC_D) mKeyDirection.x = 1;
}

void CharacterController::injectKeyUp(const OIS::KeyEvent& evt)
{
	// keep track of the player's intended direction
	if (evt.key == OIS::KC_W && mKeyDirection.z == -1) mKeyDirection.z = 0;
	else if (evt.key == OIS::KC_A && mKeyDirection.x == -1) mKeyDirection.x = 0;
	else if (evt.key == OIS::KC_S && mKeyDirection.z == 1) mKeyDirection.z = 0;
	else if (evt.key == OIS::KC_D && mKeyDirection.x == 1) mKeyDirection.x = 0;
}

void CharacterController::injectMouseMove(const OIS::MouseEvent& evt)
{
	// update camera goal based on mouse movement
	updateCameraGoal(-0.5f * evt.state.X.rel, -0.5f * evt.state.Y.rel, -0.0005f * evt.state.Z.rel);

	// auto rotate character to face directly in front of the camera view
	Ogre::Real targetYaw = mBodyNode->getOrientation().zAxis().getRotationTo(-mCameraNode->getOrientation().zAxis()).getYaw().valueDegrees();
	mBodyNode->yaw(Ogre::Degree(targetYaw));
}

void CharacterController::injectMouseDown(const OIS::MouseEvent& evt, OIS::MouseButtonID id)
{
	//
}