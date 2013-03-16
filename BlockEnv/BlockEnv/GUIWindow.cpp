#include "GUIWindow.h"
#include "Application.h"
#include "Color.h"
#include "Flags.h"
#include "Collision.h"
#include "Input.h"
#include "../Frost/StringList.h"
#include "Block.h"

GUIWindow::GUIWindow(void)
{
}

GUIWindow::GUIWindow(int iWindowType, long lX, long lY, long lWidth, long lHeight, GUIWindow *pParent)
{
	Parent = 0;
	ActiveChild = 0;
	Visible = true;
	ReadOnly = false;
	Disabled = false;
	PushState = 0;
	WindowFlags = 0;
	CaretTime = 0;
	CaretPosition = 0;
	OnClick = 0;
	OnEnter = 0;
	UsesScrollbar = false;
	ScrollbarPosition = 0.0f;

	WindowType = iWindowType;
	X(lX);
	Y(lY);
	Width(lWidth);
	Height(lHeight);

	if (pParent)
	{
		SetParent(pParent);
	}
}

GUIWindow::~GUIWindow(void)
{
	if (HasChildren())
	{
		for (unsigned int i = 0; i < Children.size()-1; i++)
		{
			delete Children[i];
		}
	}
}

void GUIWindow::SetParent(GUIWindow *newParent)
{
	if (Parent != 0)
	{
		Parent->RemoveChild(this);
	}
	if (newParent != 0)
	{
		newParent->AddChild(this);
	}
}

bool GUIWindow::HasChildren()
{
	return (Children.size() > 0);
}

void GUIWindow::AddChild(GUIWindow *child)
{
	// Check if the request is valid.
	if (child->Parent != 0)
	{
		MessageBoxA(0, "Tried to add a child window that already has a parent to another window!", "Error!", MB_OK);
		return;
	}

	child->Parent = this;
	Children.push_back(child);

	// Make recently added children instantly active.
	if (!child->Active())
	{
		SetActiveChild(child);
	}
}

void GUIWindow::RemoveChild(GUIWindow* child)
{
	// Check if the request is valid.
	if (!HasChildren())
	{
		MessageBoxA(0, "Tried to remove a child from a window that has no children!", "Error!", MB_OK);
		return;
	}
	
	bool FoundChild = false;
	for (unsigned int i = 0; i < Children.size()-1; i++)
	{
		if ( Children[i] == child )
		{
			Children[i]->Parent = 0;
			Children.erase(Children.begin()+i);
			FoundChild = true;
			break;
		}
	}

	// Make sure it worked.
	if (!FoundChild)
	{
		MessageBoxA(0, "Tried to remove a child window from a window that it is not a child of!", "Error!", MB_OK);
	}
}

void GUIWindow::SetActiveChild(GUIWindow *child)
{
	if (child != 0)
	{
		// Check if the request is valid.
		if (child->Parent != this)
		{
			MessageBoxA(0, "Tried to set a child that is not owned as active.", "Error!", MB_OK);
			return;
		}

		// Set the active child of the window.
		ActiveChild = child;

		// Only try to bring them to the foreground if they are not already in it.
		if (!child->IsForegroundChild())
		{
			child->BringToTop();
		}
	}
	else
	{
		ActiveChild = 0;
	}
}

GUIWindow *GUIWindow::ChildAtPoint(Point coord)
{
	if (!HasChildren())
	{
		return (GUIWindow*)0;
	}

	for (int i = Children.size()-1; i >= 0; i--)
	{
		if (Collision::PointToRect(Children[i]->ClientToWindow(coord), Children[i]->ClientArea) && Children[i]->IsVisible())
		{
			//return Children[i-1]->FindChildAtPoint(coord);
			return Children[i];
		}
	}

	return (GUIWindow*)0;
}

void GUIWindow::BringToTop()
{
	if (Parent)
	{
		// Removing and re-adding the window back to the parent puts it at the top of the z-order.
		SetParent(Parent);
	}
}

void GUIWindow::Hide()
{
	Visible = false;
}

void GUIWindow::Show()
{
	Visible = true;
}

bool GUIWindow::IsVisible()
{
	if (!Visible)
	{
		return false;
	}
	else if (Parent)
	{
		return Parent->IsVisible();
	}
	else
	{
		return true;
	}
}

bool GUIWindow::Active()
{
	return GUI::GetActiveWindow() == this || GUI::GetActiveControl() == this;
}

bool GUIWindow::IsForegroundWindow()
{
	return (GUI::GetDesktop()->Children[GUI::GetDesktop()->Children.size()-1] == this);
}

bool GUIWindow::IsForegroundChild()
{
	if (Parent)
	{
		return (Parent->Children[Parent->Children.size()-1] == this);
	}
	else
	{
		return false;
	}
}

long GUIWindow::X()
{
	return ClientArea.Left;
}

void GUIWindow::X(long NewX)
{
	ClientArea.Left = NewX;
}

long GUIWindow::Y()
{
	return ClientArea.Top;
}

void GUIWindow::Y(long NewY)
{
	ClientArea.Top = NewY;
}

long GUIWindow::Width()
{
	return ClientArea.Width();
}

void GUIWindow::Width(long NewWidth)
{
	ClientArea.setWidth(NewWidth);
}

long GUIWindow::Height()
{
	return ClientArea.Height();
}

void GUIWindow::Height(long NewHeight)
{
	ClientArea.setHeight(NewHeight);
}

bool GUIWindow::IsDesktop()
{
	return (WindowType == WINDOW_TYPE_DESKTOP);
}

bool GUIWindow::IsWindow()
{
	return (WindowType == WINDOW_TYPE_WINDOW);
}

bool GUIWindow::IsButton()
{
	return (WindowType == WINDOW_TYPE_BUTTON);
}

bool GUIWindow::IsTextbox()
{
	return (WindowType == WINDOW_TYPE_TEXTBOX);
}

bool GUIWindow::CanBeTypedIn()
{
	return (IsTextbox() && Visible && !ReadOnly && !Disabled);
}

bool GUIWindow::IsMousedOver()
{
	Point derp(Input::ClientCursorPos.x, Input::ClientCursorPos.y);
	return Collision::PointToRect(ClientToWindow(derp), ClientArea);
}

void GUIWindow::ResetCaretPosition()
{
	CaretPosition = Text.length();
}

Point GUIWindow::WindowToClient(Point coord)
{
	Point temp = coord;

	GUIWindow *currentParent = Parent;
	while (currentParent != 0)
	{
		temp.X += currentParent->X();
		temp.Y += currentParent->Y();
		currentParent = currentParent->Parent;
	}

	return temp;
}

Point GUIWindow::ClientToWindow(Point coord)
{
	Point temp = coord;

	GUIWindow *currentParent = Parent;
	while (currentParent != 0)
	{
		if (currentParent)
		{
			temp.X -= currentParent->X();
			temp.Y -= currentParent->Y();
			currentParent = currentParent->Parent;
		}
	}

	return temp;
}

void GUIWindow::Draw()
{
	// Obviously, there's nothing to draw if the window shouldn't be visible.
	if (Visible)
	{
		Point DrawPoint = WindowToClient(Point(X(), Y()));
		float DrawX = (float)DrawPoint.X;
		float DrawY = (float)DrawPoint.Y;
		// Draw the window, based on what type of window it is.
		switch (WindowType)
		{
		case WINDOW_TYPE_DESKTOP:
			{
				// Just draw a general box of the area.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), (float)Height(), &(Colors::Black - Color(0, 0, 0, 150)), 0);
			}
			break;

		case WINDOW_TYPE_WINDOW:
			{
				// First draw the general area of the window.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), (float)Height(), &(Colors::Blue - Color(0, 0, 0, 100)), 0);
				// Then draw a border to make it look more like a window.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), 1, &Colors::DarkBlue, 0); // Top line.
				Application::DrawQuad(DrawX, DrawY+Height()-1, (float)Width(), 1, &Colors::DarkBlue, 0); // Bottom line.
				Application::DrawQuad(DrawX, DrawY, 1, (float)Height(), &Colors::DarkBlue, 0); // Left line.
				Application::DrawQuad(DrawX+Width()-1, DrawY, 1, (float)Height(), &Colors::DarkBlue, 0); // Right line.
				// Draw the title bar with the title of the window.
				Application::DrawQuad(DrawX, DrawY-16, (float)Width(), 16, &Colors::DarkBlue, 0);
				Application::PrintText(DrawX, DrawY-16, Text);
			}
			break;

		case WINDOW_TYPE_BUTTON:
			{
				// Handle button clicks.
				if (PushState == PUSH_STATE_NORMAL && IsMousedOver())
				{
					PushState = PUSH_STATE_HOVER;
				}
				if (PushState == PUSH_STATE_HOVER && !IsMousedOver())
				{
					PushState = PUSH_STATE_NORMAL;
				}
				if (PushState == PUSH_STATE_PUSHED && Input::GetKeyByKeyCode(0x01)->WasReleased() && Input::IsForGUI())
				{
					PushState = PUSH_STATE_NORMAL;
					if (OnClick != 0)
					{
						(*OnClick)();
					}
				}

				// Conditionally define the button colors based on push state.
				Color ButtonColor;
				Color TopBorderColor;
				Color BottomBorderColor;

				switch (PushState)
				{
				case PUSH_STATE_NORMAL:
					{
						ButtonColor = Colors::Green;
						TopBorderColor = Colors::White;
						BottomBorderColor = Colors::Black;
					}
					break;

				case PUSH_STATE_HOVER:
					{
						ButtonColor = Colors::LightGreen;
						TopBorderColor = Colors::White;
						BottomBorderColor = Colors::Black;
					}
					break;

				case PUSH_STATE_PUSHED:
					{
						ButtonColor = Colors::DarkGreen;
						TopBorderColor = Colors::Black;
						BottomBorderColor = Colors::White;
					}
					break;
				}

				// First draw the general area of the button.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), (float)Height(), &ButtonColor, 0);
				// Then draw the button border to make it look like a button.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), 2, &TopBorderColor, 0); // Top line.
				Application::DrawQuad(DrawX, DrawY+Height()-2, (float)Width(), 2, &BottomBorderColor, 0); // Bottom line.
				Application::DrawQuad(DrawX, DrawY, 2, (float)Height(), &TopBorderColor, 0); // Left line.
				Application::DrawQuad(DrawX+Width()-2, DrawY, 2, (float)Height(), &BottomBorderColor, 0); // Right line.
			}
			break;

		case WINDOW_TYPE_TEXTBOX:
			{
				// First draw the general area of the textbox.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), (float)Height(), &(Colors::Red - Color(0, 0, 0, 100)), 0);
				// Then draw the button border to make it look like a textbox.
				Application::DrawQuad(DrawX, DrawY, (float)Width(), 2, &Colors::DarkRed, 0); // Top line.
				Application::DrawQuad(DrawX, DrawY+Height()-2, (float)Width(), 2, &Colors::DarkRed, 0); // Bottom line.
				Application::DrawQuad(DrawX, DrawY, 2, (float)Height(), &Colors::DarkRed, 0); // Left line.
				Application::DrawQuad(DrawX+Width()-2, DrawY, 2, (float)Height(), &Colors::DarkRed, 0); // Right line.
				// Draw the text the textbox should have inside of it.
				if (Text.length() > 0)
				{
					Application::PrintText(DrawX, DrawY, Text);
				}
				else if (DefaultText.length() > 0 && !Active())
				{
					Application::PrintText(DrawX, DrawY, DefaultText);
				}

				if (Active())
				{
					CaretTime -= (float)Application::TimeElapsedThisFrame;
					if (CaretTime < 1000)
					{
						CaretTime = 3000;
					}
					Application::DrawQuad(DrawX+2+(CaretPosition*12), DrawY+2, (float)3, 12.0f, &(Colors::White - Color(0, 0, 0, (BYTE)(255-( (CaretTime/3000.0f) * 255) ) )), 0);
				}
			}
			break;
		}

		/*
		// Highlight active controls. [Debug option]
		if (GUIGetActiveWindow() == this)
		{
			Application::DrawQuad(DrawX, DrawY, Width(), Height(), &(Colors::Green - Color(0, 0, 0, 100)));
		}
		else if (GUIGetActiveControl() == this)
		{
			Application::DrawQuad(DrawX, DrawY, Width(), Height(), &(Colors::Red - Color(0, 0, 0, 100)));
		}
		*/

		// Draw all this window's children.
		if (HasChildren())
		{
			for (unsigned int i = 0; i < Children.size(); i++)
			{
				Children[i]->Draw();
			}
		}
	}
}

namespace GUI
{
	GUIWindow *GetDesktop()
	{
		static GUIWindow *Desktop = new GUIWindow(WINDOW_TYPE_DESKTOP, 0, 0, Application::ClientWidth, Application::ClientHeight, 0);
		return Desktop;
	}

	GUIWindow *GetActiveWindow()
	{
		GUIWindow *TargetWnd = GetDesktop();
		
		while (TargetWnd->ActiveChild != 0)
		{
			// TODO: Shouldn't it be the window's responsibility to switch active children if the currently active child
			// is no longer visible? That would mean this could need to work a little differently....
			if (TargetWnd->ActiveChild->IsWindow() && TargetWnd->ActiveChild->IsVisible())
			{
				TargetWnd = TargetWnd->ActiveChild;
			}
			else
			{
				break;
			}
		}

		return TargetWnd;
	}

	GUIWindow *GetActiveControl()
	{
		return GUI::GetActiveWindow()->ActiveChild;
	}

	void RecieveKeyboardKey(KeyboardKey *Key)
	{
		if (!GetDesktop()->IsVisible())
		{
			return;
		}

		bool IsShiftPressed = Input::IsShiftDown();
		GUIWindow *TargetControl = GUI::GetActiveControl();
		GUIWindow *TargetWindow = GUI::GetActiveWindow();

		// Mouse handling.
		if (Key->KeyCode == 0x01) // Handle left clicks.
		{
			Point ClickPoint;
			ClickPoint.X = Input::ClientCursorPos.x;
			ClickPoint.Y = Input::ClientCursorPos.y;

			GUIWindow *ClickedWindow = GetDesktop()->ChildAtPoint(ClickPoint);
			// Check if a valid window was found.
			if (ClickedWindow != 0)
			{
				bool WasActive = false;
				if (ClickedWindow->Active())
				{
					WasActive = true;
				}
				else
				{
					GetDesktop()->SetActiveChild(ClickedWindow);
				}

				GUIWindow *ClickedControl = ClickedWindow->ChildAtPoint(ClickPoint);
				// Check if a valid control was found.
				if (ClickedControl != 0)
				{
					ClickedWindow->SetActiveChild(ClickedControl);
					if (ClickedControl->IsButton())
					{
						ClickedControl->PushState = PUSH_STATE_PUSHED;
					}
				}
				else
				{
					if (WasActive)
					{
						ClickedWindow->SetActiveChild(0);
					}
				}
			}
		}

		// Keyboard (typing stuff) handling.
		if (TargetControl != 0 && TargetWindow != 0)
		{
			if (Key->KeyCode == 0x0D) // Handle enter.
			{
				if (TargetControl->OnEnter != 0)
				{
					(*TargetControl->OnEnter)();
				}
			}

			if (TargetControl->CanBeTypedIn())
			{
				if (Key->CheckForRepeat())
				{
					Key->SetRepeatComplete();
				}

				if ((Key->HasCapitalChar() && IsShiftPressed))
				{
					TargetControl->Text += Key->CapitalKey;
					TargetControl->CaretPosition += 1;
				}
				else if (Key->HasChar())
				{
					TargetControl->Text += Key->Key;
					TargetControl->CaretPosition += 1;
				}
				else if (Key->KeyCode == 0x08) // Handle backspace.
				{
					if (TargetControl->Text.length() > 0 && TargetControl->CaretPosition > 0)
					{
						TargetControl->Text.Trim(TargetControl->CaretPosition-1, TargetControl->CaretPosition);
						TargetControl->CaretPosition -= 1;
					}
				}
				else if (Key->KeyCode == 0x2E) // Handle delete.
				{
					if (TargetControl->Text.length() > 0 && TargetControl->CaretPosition < TargetControl->Text.length())
					{
						TargetControl->Text.Trim(TargetControl->CaretPosition, TargetControl->CaretPosition+1);
					}
				}
				else if (Key->KeyCode == 0x25) // Handle left arrow key
				{
					if (TargetControl->CaretPosition > 0)
					{
						TargetControl->CaretPosition -= 1;
					}
				}
				else if (Key->KeyCode == 0x27) // Handle right arrow key
				{
					if (TargetControl->CaretPosition < TargetControl->Text.length())
					{
						TargetControl->CaretPosition += 1;
					}
				}
			}
		}
	}

	namespace Chatbox
	{
		GUIWindow *MainWnd;
		GUIWindow *Message;
		GUIWindow *ChatHistory;

		void Init()
		{
			MainWnd = new GUIWindow(WINDOW_TYPE_WINDOW, 10, 300, 600, 250, GUI::GetDesktop());
			Message = new GUIWindow(WINDOW_TYPE_TEXTBOX, 10, 220, 580, 20, MainWnd);
			ChatHistory = new GUIWindow(WINDOW_TYPE_TEXTBOX, 10, 10, 580, 200, MainWnd);
			ChatHistory->ReadOnly = true;
			Message->DefaultText = "{Enter a Command}";
			MainWnd->Text = "Console";
			Message->OnEnter = Chatbox::OnEnter;
			MainWnd->SetActiveChild(Message);
			//MainWnd->Hide();
		}

		void OnEnter()
		{
			if (!Message->Text.empty())
			{
				String entry = Message->Text;
				Message->Text = "";
				Message->ResetCaretPosition();
				//ChatHistory->Text.addLine(entry);

				StringList split = entry.split(" ");

				if (split[0] == "/me")
				{
					String thestuff;
					thestuff << "* ClericX " << split[1];
					ChatHistory->Text.addLine(thestuff);
				}
				else if (split[0] == "/clearchat")
				{
					ChatHistory->Text = "";
				}
				else if (split[0] == "/spawnblock")
				{
					Block::Instance *add = new Block::Instance();
					add->Position.x = (float)split[1].toInt();
					add->Position.y = (float)split[2].toInt();
					add->Position.z = (float)split[3].toInt();
					Block::Add(add);
					String outputString;
					outputString << "Block spawned at X: " << (int)add->Position.x << ", Y: " << (int)add->Position.y << ", Z: " << (int)add->Position.z;
					ChatHistory->Text.addLine(outputString);
				}
				else if (split[0] == "/killblock")
				{
					bool removed = Block::Remove(Vector3f((float)split[1].toInt(), (float)split[2].toInt(), (float)split[3].toInt()));
					ChatHistory->Text.addLine((removed ? "Block successfully removed." : "No block exists there."));
				}
				else if (split[0] == "/reqblock")
				{
					Block::Instance *add = new Block::Instance();
					add->Position.x = (float)split[1].toInt();
					add->Position.y = (float)split[2].toInt();
					add->Position.z = (float)split[3].toInt();
					Block::RequestAdd(add);
					String outputString;
					outputString << "Requested a block spawn at X: " << (int)add->Position.x << ", Y: " << (int)add->Position.y << ", Z: " << (int)add->Position.z;
					ChatHistory->Text.addLine(outputString);
				}
				else if (split[0] == "/reqkillblock")
				{
					Block::Instance *add = new Block::Instance();
					add->Position.x = (float)split[1].toInt();
					add->Position.y = (float)split[2].toInt();
					add->Position.z = (float)split[3].toInt();
					Block::RequestRemove(add);
					String outputString;
					outputString << "Requested a block removal at X: " << (int)add->Position.x << ", Y: " << (int)add->Position.y << ", Z: " << (int)add->Position.z;
					ChatHistory->Text.addLine(outputString);
				}
				else if (split[0] == "/placeblock")
				{
					Application::PlacementBlockID = (unsigned int)split[1].toInt();
					ChatHistory->Text.addLine(String("Changed block placement type to \"") + Block::GetTypeInfoByID(Application::PlacementBlockID)->Name + "\".");
				}
			}
		}
	};
}