#include "Application.h"
#include "GUIWindow.h"
#include "Input.h"
#include "Flags.h"
#include "../FrostGame/Block.h"
#include "Block.h"
#include "../FrostGame/Player.h"

#include <Math.h>
#include "../Frost/List.h"

#include "../Frost/Vector3f.h"
#include "Point.h"
#include "myglu.h"

#include "PacketStructures.h"

void PotentiallySendPlayerUpdatePacket()
{
	static Vector3f oldPos;
	static float oldPitch;
	static float oldYaw;

	if (oldPos != Application::CharPos || abs(floor(Application::CamPitch) - floor(oldPitch)) > 2 || abs(floor(Application::CamYaw) - floor(oldYaw)) > 2)
	{
		oldPos = Application::CharPos;
		oldPitch = Application::CamPitch;
		oldYaw = Application::CamYaw;

		// Construct and send a player update packet.
		PlayerRelatedPacket p;
		p.Location = oldPos;
		p.Pitch = oldPitch;
		p.Yaw = oldYaw;

		Application::SendPacket(p.toPacket());
	}
}

void DrawBlock(Block::Instance *blockData)
{
	blockData->Type->TheTexture->Bind();

	glBegin(GL_QUADS);
	//glColor4ub(0xCC, 0xCC, 0xCC, 255);
	//Front
	//glColor4ub(0xFF, 0, 0, 0xFF);
	glTexCoord2f(0, 0);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10), (blockData->Position.z * 10));
	glTexCoord2f(1, 0);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10), (blockData->Position.z * 10));
	glTexCoord2f(1, 1);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10)+10, (blockData->Position.z * 10));
	glTexCoord2f(0, 1);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10)+10, (blockData->Position.z * 10));
	// Back
	//glColor4ub(0x55, 0, 0, 0xFF);
	glTexCoord2f(0, 0);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10), (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 0);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10), (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 1);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10)+10, (blockData->Position.z * 10)+10);
	glTexCoord2f(0, 1);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10)+10, (blockData->Position.z * 10)+10);
	//Top
	//glColor4ub(0, 0xFF, 0, 0xFF);
	glTexCoord2f(0, 0);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10), (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 0);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10), (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 1);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10), (blockData->Position.z * 10));
	glTexCoord2f(0, 1);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10), (blockData->Position.z * 10));
	//Bottom
	//glColor4ub(0, 0x55, 0, 0xFF);
	glTexCoord2f(0, 0);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10)+10, (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 0);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10)+10, (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 1);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10)+10, (blockData->Position.z * 10));
	glTexCoord2f(0, 1);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10)+10, (blockData->Position.z * 10));
	//Left
	//glColor4ub(0, 0, 0xFF, 0xFF);
	glTexCoord2f(0, 0);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10), (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 0);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10), (blockData->Position.z * 10));
	glTexCoord2f(1, 1);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10)+10, (blockData->Position.z * 10));
	glTexCoord2f(0, 1);
	glVertex3f((blockData->Position.x * 10), (blockData->Position.y * 10)+10, (blockData->Position.z * 10)+10);
	//Right
	//glColor4ub(0, 0, 0x55, 0xFF);
	glTexCoord2f(0, 0);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10), (blockData->Position.z * 10)+10);
	glTexCoord2f(1, 0);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10), (blockData->Position.z * 10));
	glTexCoord2f(1, 1);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10)+10, (blockData->Position.z * 10));
	glTexCoord2f(0, 1);
	glVertex3f((blockData->Position.x * 10)+10, (blockData->Position.y * 10)+10, (blockData->Position.z * 10)+10);
	glEnd();

	blockData->Type->TheTexture->Unbind();
}

void DrawPlayer(Player::Instance *playerData)
{
	glPushMatrix();

	glTranslatef(playerData->Location.x, playerData->Location.y, playerData->Location.z); //move the camera
	//glRotatef(-playerData->Pitch, 1.0, 0.0, 0.0); //rotate the camera (more precisly move everything in the opposit direction)
	glRotatef(playerData->Yaw, 0.0, 1.0, 0.0);

	glBegin(GL_QUADS);

	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glVertex3f(10, 30, 0);
	glVertex3f(0, 30, 0);

	glEnd();

	glPopMatrix();
}

	double wx1, wy1, wz1;
	double wx2, wy2, wz2;

void CalculateUnprojection()
{
	static int x = 400;
	static int y = 300;

	int viewport[16];
	double mvmatrix[16];
	double projmatrix[16];
	int realy;
	//double wx1, wy1, wz1;
	//double wx2, wy2, wz2;

	glGetIntegerv( GL_VIEWPORT, viewport );
	glGetDoublev( GL_MODELVIEW_MATRIX, mvmatrix );
	glGetDoublev( GL_PROJECTION_MATRIX, projmatrix );
	realy = viewport[3] - (GLint) y - 1;
	gluUnProject( (GLdouble) x, (GLdouble) realy, 0.0, mvmatrix, projmatrix, viewport, &wx1, &wy1, &wz1 );
	gluUnProject( (GLdouble) x, (GLdouble) realy, 1.0, mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2 );
}

bool BlocksInit = false;

void InitBlocks()
{
	if (!BlocksInit)
	{
		BlockRelatedPacket p;
		p.ActionType = BlockRelatedPacket::Update;
		Application::SendPacket(p.toPacket());

		BlocksInit = true;
	}
}

void ToggleConsole()
{
	if (Input::GetKeyByChar('~')->WasPressed())
	{
		if (GUI::GetDesktop()->IsVisible())
		{
			GUI::GetDesktop()->Hide();
		}
		else
		{
			GUI::GetDesktop()->Show();
		}
		GUI::Chatbox::Message->CaretPosition = 0;
		GUI::Chatbox::Message->Text.clear();
	}
}

// y1 is a coordinate (x,y,z) of the first point
// y2 is a coordinate (x,y,z) of the second point
// mu is the distance from the first line to the other which indicates the point desired to be interpolated.
double LinearInterpolate(double y1, double y2, double mu)
{
   return(y1*(1-mu)+y2*mu);
}

bool Jumping = false;
bool StillFallingFromJump = false;
float JumpedHeight = 0.0f;
float FallHeight = 0.0f;

void BasicCollisionAndGravity()
{
	Vector3f CharPosBlock = Block::AbsCoordToGridCoord(Application::CharPos);
	Vector3f BlockUnderCharPos = Block::AbsCoordToGridCoord(Vector3f(Application::CharPos.x, Application::CharPos.y - 0.15, Application::CharPos.z));

	if (Input::GetKeyByKeyCode(Input::Keys::Spacebar)->WasPressed() /* && !Jumping && !StillFallingFromJump */ && Input::IsForGame())
	{
		Jumping = true;
	}

	if (Jumping)
	{
		Application::CharPos.y += 0.15;
		JumpedHeight += 0.15;
	}

	if (JumpedHeight >= 20.0)
	{
		Jumping = false;
		JumpedHeight = 0.0;
		StillFallingFromJump = true;
	}

	if (Block::Get(BlockUnderCharPos) == 0 && !Jumping)
	{
		Application::CharPos.y -= 0.15;
		FallHeight += 0.15;
	}
	else if (StillFallingFromJump)
	{
		StillFallingFromJump = false;
		FallHeight = 0.0;
		//Application::CharPos.y = CharPosBlock.y;
	}
}

void Application::DrawScene()
{
	// Clear the color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	setDrawMode(DRAWMODE_3D);
	ApplyCharacterMovement();
	UpdateCamera();
	InitBlocks();
	CalculateUnprojection();
	BasicCollisionAndGravity();
	PotentiallySendPlayerUpdatePacket();

	// Handle mouse locking.
	if (Input::GetKeyByChar('t')->WasPressed())
	{
		Input::MouseLocked = !Input::MouseLocked;
	}

	if (Input::MouseLocked)
	{
		Input::CursorPos.x = Application::ScreenWidth / 2;
		Input::CursorPos.y = Application::ScreenHeight / 2;
		SetCursorPos(Input::CursorPos.x, Input::CursorPos.y);
	}

	// Very simple code to draw every block loaded into the client's memory.
	for (unsigned int i = 0; i < Block::WorldBlockList.count(); i++)
	{
		DrawBlock(Block::WorldBlockList[i]);
	}

	// Very simple code to draw every player loaded into the client's memory.
	for (unsigned int i = 0; i < Player::Players.count(); i++)
	{
		//DrawPlayer(Player::Players[i]);
	}

	// Code for determining how/where blocks can be placed.
	// Remember to move this as well as enable free-place mode.
	float ClickDistance = Vector3f::distance(Vector3f(wx1, wy1, wz1), Vector3f(wx2, wy2, wz2));
	Vector3f PossibleBlockPlacementPos;
	Vector3f PossibleBlockRemovePos;
	bool FoundPossibleBlockPlacement = false;

	for (int i = 0; i < 10; i++) // For up to 10 blocks ahead of you... (A rough estimation).
	{
		double PointDist = (i * 10.0) / ClickDistance;
		Vector3f AbsBlockCoord;
		AbsBlockCoord.x = LinearInterpolate(wx1, wx2, PointDist);
		AbsBlockCoord.y = LinearInterpolate(wy1, wy2, PointDist);
		AbsBlockCoord.z = LinearInterpolate(wz1, wz2, PointDist);
		AbsBlockCoord = Block::AbsCoordToGridCoord(AbsBlockCoord);
		PossibleBlockRemovePos = AbsBlockCoord;
		Block::Instance *ThisBlock = Block::Get(PossibleBlockRemovePos);

		if (ThisBlock != 0)
		{
			if (Input::GetKeyByKeyCode(Input::Keys::RMouse)->WasPressed())
			{
				Block::RequestRemove(ThisBlock);
			}
			// If we found a removable block, there won't be any blocks further down
			// that we need to check for, so just break out of the loop.
			break;
		}
		else if (!FoundPossibleBlockPlacement)
		{
			// Check EVERY possible location beside the current mouse location to see if placing a block here is valid.
			Block::Instance *TopBlock = Block::Get(Vector3f(AbsBlockCoord.x, AbsBlockCoord.y - 1, AbsBlockCoord.z));
			Block::Instance *BottomBlock = Block::Get(Vector3f(AbsBlockCoord.x, AbsBlockCoord.y + 1, AbsBlockCoord.z));
			Block::Instance *LeftBlock = Block::Get(Vector3f(AbsBlockCoord.x - 1, AbsBlockCoord.y, AbsBlockCoord.z));
			Block::Instance *RightBlock = Block::Get(Vector3f(AbsBlockCoord.x + 1, AbsBlockCoord.y, AbsBlockCoord.z));
			Block::Instance *FrontBlock = Block::Get(Vector3f(AbsBlockCoord.x, AbsBlockCoord.y, AbsBlockCoord.z - 1));
			Block::Instance *BackBlock = Block::Get(Vector3f(AbsBlockCoord.x, AbsBlockCoord.y, AbsBlockCoord.z + 1));

			if (TopBlock != 0 || BottomBlock != 0 || LeftBlock != 0 || RightBlock != 0 || FrontBlock != 0 || BackBlock != 0)
			{
				PossibleBlockPlacementPos = AbsBlockCoord;
				Block::Instance *tempBlock = new Block::Instance();
				tempBlock->Position = PossibleBlockPlacementPos;
				tempBlock->Type = Block::GetTypeInfoByID(0);
				DrawBlock(tempBlock);
				if (Input::GetKeyByKeyCode(Input::Keys::LMouse)->WasPressed() && Input::IsForGame())
				{
					tempBlock->Type = Block::GetTypeInfoByID(PlacementBlockID);
					Block::RequestAdd(tempBlock);
				}
				else
				{
					delete tempBlock;
				}
				FoundPossibleBlockPlacement = true;
			}
		}
	}

	setDrawMode(DRAWMODE_2D);
	GUI::GetDesktop()->Draw();
	ToggleConsole();

	// Just a temporary thing to show what the currently selected block is/looks like.
	Application::DrawQuad(760, 10, 32, 32, 0, Block::GetTypeInfoByID(PlacementBlockID)->TheTexture);

	String DebugString;
	DebugString << "CamLocX: " << String(CamPos.x, 5) << " | CamLocY: " << String(CamPos.y, 5) << " | CamLocZ: " << String(CamPos.z, 5) << '\n';
	DebugString << "CamPitch: " << String(CamPitch, 5) << " | CamYaw: " << String(CamYaw, 5) << '\n';
	DebugString << "AddBlock: " << String(PossibleBlockPlacementPos.x, 0) << ", " << String(PossibleBlockPlacementPos.y, 0) << ", " << String(PossibleBlockPlacementPos.z, 0) << '\n';
	DebugString << "RemBlock: " << String(PossibleBlockRemovePos.x, 0) << ", " << String(PossibleBlockRemovePos.y, 0) << ", " << String(PossibleBlockRemovePos.z, 0) << '\n';
	Vector3f charBlock = Block::AbsCoordToGridCoord(Application::CharPos);
	DebugString << "CharBlock: " << String(charBlock.x, 0) << ", " << String(charBlock.y, 0) << ", " << String(charBlock.z, 0);
	PrintText(0, 0, DebugString);

	// Swap the internal buffer with the drawn device context. (Render the scene)
	SwapBuffers( WindowDC );
}