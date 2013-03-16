#include "UI.h"
#include "Main.h"

CGUIWindow::CGUIWindow(void)
{
	Parent = (CGUIWindow*)GetDesktop();
	Color = Colors.White;
	Background = GUIImg;
	X = 0;
	Y = 0;
	Width = 1;
	Height = 1;
	Visible = true;
}

CGUIWindow::~CGUIWindow(void)
{
	for (unsigned int i = 0; i < Children.size(); i++)
	{
		delete Children[i];
	}
}

Rect CGUIWindow::GetRect()
{
	Rect ply;
	ZeroMemory(&ply, sizeof(Rect));
	ply.left = X;
	ply.right = ply.left + Width;
	ply.top = Y;
	ply.bottom = ply.top + Height;
	return ply;
}

CGUIWindow *CGUIWindow::FindChildAtPoint(POINT coord)
{
	if (!HasChildren())
	{
		return this;
	}
	
	for (unsigned int i = Children.size(); i > 0; i--)
	{
		if (Collision.PointToRect(coord, Children[i-1]->GetRect()))
		{
			return Children[i-1]->FindChildAtPoint(coord);
		}
	}
	
	return this;
}

void CGUIWindow::SetParent(CGUIWindow* parent)
{
	parent->AddChild(this);
}

void CGUIWindow::BringToTop()
{
	CGUIWindow *oldparent = Parent;
	oldparent->RemoveChild(this);
	oldparent->AddChild(this);
}

void CGUIWindow::AddChild(CGUIWindow* child)
{
	if (HasChildren())
	{
		for (unsigned int i = 0; i < Children.size(); i++)
		{
			if ( Children[i] == child )
			{
				return;
			}
		}
	}

	child->Parent = this;
	Children.push_back(child);
}

void CGUIWindow::RemoveChild(CGUIWindow* child)
{
	if (!HasChildren())
	{
		return;
	}
	
	for (unsigned int i = 0; i < Children.size(); i++)
	{
		if ( Children[i] == child )
		{
			Children[i]->Parent = (CGUIWindow*)GetDesktop();
			Children.erase(Children.begin()+i);
			break;
		}
	}
}

void CGUIWindow::DrawCore()
{
	if (Visible)
	{
		//D3DXMATRIX matrix;
		//D3DXMatrixTransformation2D(&matrix, 0, 0, &D3DXVECTOR2((float)Width, (float)Height), 0, 0, 0);
		//D3D.Sprite->SetTransform(&matrix);
		D3DXMATRIX oldmat;

D3DXVECTOR2 spriteCentre=D3DXVECTOR2(0.0f,0.0f);
D3DXVECTOR2 trans=D3DXVECTOR2((float)X,(float)Y);
float rotation=0.0f;
D3DXMATRIX mat;
D3DXVECTOR2 scaling((float)Width,(float)Height);
D3DXMatrixTransformation2D(&mat,NULL,0.0,&scaling,&spriteCentre,rotation,&trans);
D3D.Sprite->GetTransform(&oldmat);
D3D.Sprite->SetTransform(&mat);

		if (Parent)
			D3D.Draw(Background->Texture, X + Parent->X, Y + Parent->Y, 0, D3DCOLOR_ARGB( 0x55, 0xFF, 0xFF, 0xFF ), false);
		else
			D3D.Draw(Background->Texture, X, Y, 0, D3DCOLOR_ARGB( 0x55, 0xFF, 0xFF, 0xFF ), false);

D3D.Sprite->SetTransform(&oldmat);

		if (HasChildren())
		{
			for (unsigned int i = 0; i < Children.size(); i++)
			{
				Children[i]->DrawAll();
			}
		}
	}
}

POINT CGUIWindow::ScreenToWindow(POINT coord)
{
	POINT temp;
	temp.x = coord.x - X;
	temp.y = coord.y - Y;
	return temp;
}

POINT CGUIWindow::WindowToScreen(POINT coord)
{
	POINT temp;
	temp.x = coord.x + X;
	temp.y = coord.y + Y;
	return temp;
}

void CGUIWindow::CX_WM_LBUTTONDOWN()
{
	return;
}

void CGUIWindow::CX_WM_LBUTTONUP()
{
	if (Active)
	{
		CX_WM_CLICK();
	}
}

void CGUIWindow::CX_WM_CLICK()
{
	MessageBox(NULL, "Click recieved", "Click recieved", MB_OK);
	return;
}

/////////////////////////////////////////////////////////////////////////////////////////

CGUIDesktop::CGUIDesktop(void)
{
	Parent = 0;
	ActiveWindow = 0;
	ActiveElement = 0;
}

CGUIDesktop::~CGUIDesktop(void)
{
}

void CGUIDesktop::MakeActive(CGUIWindow *target)
{
	if (ActiveWindow != this && ActiveElement != this)
	{
		target->Active = true;
		target->BringToTop();
		
		if (target->IsWindow())
		{
			ActiveWindow->Active = false;
			ActiveWindow = target;
		}
		else if (target->IsElement())
		{
			ActiveElement->Active = false;
			ActiveElement = target;
		}
	}
}

void CGUIDesktop::CX_WM_LBUTTONDOWN()
{
	CGUIWindow *FoundChild = FindChildAtPoint(App.ClientCursorPos);
	if (FoundChild != GetDesktop())
	{
		FoundChild->CX_WM_LBUTTONDOWN();
		MakeActive(FoundChild);
	}
}

void CGUIDesktop::CX_WM_LBUTTONUP()
{
	FindChildAtPoint(App.ClientCursorPos)->CX_WM_LBUTTONUP();
}

/////////////////////////////////////////////////////////////////////////////////////////

CGUITextbox::CGUITextbox(void)
{
	IsAnElement = true;
}

CGUITextbox::~CGUITextbox(void)
{
}

void CGUITextbox::DrawAll()
{
	DrawCore();
	
	if (Active)
	{
		CaretTime -= App.TimeElapsedThisFrame;
		if (CaretTime <= 0)
			CaretTime = 1;
		
		//draw it
		D3DCOLOR_ARGB( (int)CaretTime, 0xFF, 0xFF, 0xFF );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////

CGUIButton::CGUIButton(void)
{
	IsAnElement = true;
}

CGUIButton::~CGUIButton(void)
{
}

//////////////////////////////////////////////////////////////////////////////////

CUI::CUI(void)
{
	Desktop = new CGUIDesktop();
}

CUI::~CUI(void)
{
}

void CUI::LoadBarBG()
{
	BarBG = new CMapObject();
	BarBG->SetLT(Textures.InitCore("img/UI/bg.png"));
}

void CUI::LoadHPBar()
{
	HPBar = new CMapObject();
	HPBar->SetLT(Textures.InitCore("img/UI/hp.png"));
	HPBar->SetPos(100, 700);
}

void CUI::LoadMPBar()
{
	MPBar = new CMapObject();
	MPBar->SetLT(Textures.InitCore("img/UI/mp.png"));
	MPBar->SetPos(100+210, 700);
}

void CUI::LoadEXPBar()
{
	EXPBar = new CMapObject();
	EXPBar->SetLT(Textures.InitCore("img/UI/exp.png"));
	EXPBar->SetPos(100+210+210, 700);
}

void CUI::LoadLevelBG()
{
	LevelBG = new CMapObject();
	LevelBG->SetLT(BarBG->LT);
	LevelBG->SetPos(10, 700-3);
}

void CUI::DrawBarBGs()
{
	BarBG->SetPos(100-3, 700-3);
	BarBG->Draw(DRAW_SCREEN);
	BarBG->SetPos(100+210-3, 700-3);
	BarBG->Draw(DRAW_SCREEN);
	BarBG->SetPos(100+210+210-3, 700-3);
	BarBG->Draw(DRAW_SCREEN);
}

void CUI::DrawHPBar()
{
	HPBar->ChangeSrcRect(MakeRect(0, 0, GetHPPixels(), 60));
	HPBar->Draw(DRAW_SCREEN);
}

void CUI::DrawMPBar()
{
	MPBar->ChangeSrcRect(MakeRect(0, 0, GetMPPixels(), 60));
	MPBar->Draw(DRAW_SCREEN);
}

void CUI::DrawEXPBar()
{
	EXPBar->ChangeSrcRect(MakeRect(0, 0, GetEXPPixels(), 60));
	EXPBar->Draw(DRAW_SCREEN);
}

void CUI::DrawLevelBG()
{
	LevelBG->ChangeSrcRect(MakeRect(0, 0, 70, 60));
	LevelBG->Draw(DRAW_SCREEN);
}

void CUI::DrawHPText(LPD3DXFONT Font, int X, int Y, DWORD Color)
{
	char itoabuf[20];
	char buf[256];
	ZeroMemory(&buf, sizeof(buf));

	strcat(buf, "HP: ");
	strcat(buf, _ultoa(Player.HP, itoabuf, 10));
	strcat(buf, " / ");
	strcat(buf, _ultoa(Player.MaxHP, itoabuf, 10));

	D3D.Text(Font, X, Y, Color, buf);
}

void CUI::DrawMPText(LPD3DXFONT Font, int X, int Y, DWORD Color)
{
	char itoabuf[20];
	char buf[256];
	ZeroMemory(&buf, sizeof(buf));

	strcat(buf, "MP: ");
	strcat(buf, _ultoa(Player.MP, itoabuf, 10));
	strcat(buf, " / ");
	strcat(buf, _ultoa(Player.MaxMP, itoabuf, 10));

	D3D.Text(Font, X, Y, Color, buf);
}

void CUI::DrawEXPText(LPD3DXFONT Font, int X, int Y, DWORD Color)
{
	char itoabuf[20];
	char buf[256];
	ZeroMemory(&buf, sizeof(buf));

	strcat(buf, "EXP: ");
	strcat(buf, _ultoa(Player.EXP, itoabuf, 10));
	strcat(buf, " / ");
	strcat(buf, _ultoa(Player.EXPNeeded, itoabuf, 10));

	D3D.Text(Font, X, Y, Color, buf);
}

int CUI::GetHPPixels()
{
	float Percent = (float)( (float)Player.HP / (float)Player.MaxHP );
	return (int)((int)(Percent * 100) * 2);
}

int CUI::GetMPPixels()
{
	float Percent = (float)( (float)Player.MP / (float)Player.MaxMP );
	return (int)((int)(Percent * 100) * 2);
}

int CUI::GetEXPPixels()
{
	float Percent = (float)( (float)Player.EXP / (float)Player.EXPNeeded );
	return (int)((int)(Percent * 100) * 2);
}

void CUI::AutoScrollCamera()
{
	long TargetCamX = -(App.ClientWidth / 2) + (Player.X + ( ( Player.GetRect().right - Player.GetRect().left ) / 2 ) );
	long TargetCamY = -(App.ClientHeight / 2) + (Player.Y + ( ( Player.GetRect().bottom - Player.GetRect().top ) / 2 ) );

	if (D3D.CamX != TargetCamX)
	{
		D3D.CamX = TargetCamX;
	}
	if (D3D.CamY != TargetCamY)
	{
		D3D.CamY = TargetCamY;
	}
}