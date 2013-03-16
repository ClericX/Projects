#include "Attacking.h"
#include "Main.h"

CAttacking::CAttacking(void)
{
	Skills.StarSmash.Visible = false;
	Skills.StarSmash.StartAttack = false;
}

CAttacking::~CAttacking(void)
{
}

void CAttacking::xSkills::xStarSmash::Load()
{
	Star1 = new CMapObject();
	Star2 = new CMapObject();
	Star1->SetLT( Textures.InitCore ("img/Skill/1/0.png") );
	Star2->SetLT( Star1->LT );
}

void CAttacking::xSkills::xStarSmash::Draw()
{
	if (Visible)
	{
		Star1->SetPos(X, Y);
		Star1->Draw(DRAW_WORLD);
		Star2->SetPos(X2, Y);
		Star2->Draw(DRAW_WORLD);
	}
}

void CAttacking::xSkills::xStarSmash::Do()
{
	if (StartAttack == true && !Player.Attacking)
	{
		TargY = Player.GetRect().bottom - Star1->LT->ImgHeight;
		X = Player.GetRect().right + 20;
		X2 = Player.GetRect().left - 20 - Star1->LT->ImgWidth;
		Y = Player.Y - 560;

		StartAttack = false;
		Player.Attacking = true;
		Visible = true;
	}

	if ( Y >= TargY && Player.Attacking == true)
	{
		Visible = false;
		Player.Attacking = false;
	}
	else if (Player.Attacking)
	{
		Y += 20;
	}
}