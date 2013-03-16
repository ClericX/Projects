#pragma once

#include "Includes.h"

////////////////////////////////////////////////////////////
//Holds functions for performing keypress-related actions.//
////////////////////////////////////////////////////////////

namespace KeyActions
{
	namespace CharMovement
	{
		void MoveLeft();
		void MoveRight();
		void Jump();
	};

	namespace CameraMovement
	{
		void CamRight();
		void CamLeft();
		void CamDown();
		void CamUp();
		void Cam(string Direction, int Offset);
	};

	namespace Testing
	{
		void HealHP();
		void HealMP();
		void Haste();
		void Attack();
		void ReviveSnail();
		void SwitchMap();
		void Looting();
	};
};