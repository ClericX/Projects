#pragma once

#include "Includes.h"
#include "MapObject.h"

class CAttacking
{
public:
	CAttacking(void);
	~CAttacking(void);

	class xSkills
	{
	public:
		class xStarSmash
		{
		public:
			long X;
			long X2;
			long Y;
			CMapObject* Star1;
			CMapObject* Star2;
			bool Visible;
			bool StartAttack;
			long TargX;
			long TargY;
			void Load();
			void Draw();
			void Do();
		} StarSmash;
	} Skills;
};