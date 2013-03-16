#include "Items.h"
#include "Main.h"

CItems::CItems(unsigned int ItmID, unsigned int NumOf, Rect DroppingObject)
{
	ItemID = ItmID;
	Amount = NumOf;

	SetLT( Textures.GetLoadedItem(ItemID)->LT );

	Rect ThisRect = GetRect();

	X = DroppingObject.left + DroppingObject.hW() - ThisRect.hW();
	Y = DroppingObject.bottom - ThisRect.height();
}

CItems::CItems(void)
{
}

CItems::~CItems(void)
{
}