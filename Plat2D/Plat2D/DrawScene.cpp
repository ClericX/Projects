#include "Includes.h"
#include "Main.h"

unsigned int FramesDrawn = 0;

char itoabuf[20];

// Move this somewhere else later.
void CheckForCharMovement()
{
	if ( Player.X != Player.LastX || Player.Y != Player.LastY )
	{
		SendMovementPacket(App.ClientSocket, Player.ID, Player.CurrentMapID, Player.X, Player.Y);
	}

	Player.LastX = Player.X;
	Player.LastY = Player.Y;
}

void DrawStats()
{
	/*
	stringstream ss;
	ss << "Frames Drawn: " << FramesDrawn << " | " << "FPS: " << Timer.GetFPS();
	(char *)ss.str().c_str();
	*/

	FramesDrawn++;

	char buf[256];
	ZeroMemory(&buf, sizeof(buf));

	strcat(buf, "Frames Drawn: ");
	strcat(buf, _itoa(FramesDrawn, itoabuf, 10));
	strcat(buf, " | ");
	strcat(buf, "FPS: ");
	sprintf(itoabuf, "%.04g", Timer.GetFPS());
	strcat(buf, itoabuf);
	strcat(buf, " | ");
	strcat(buf, "CamX: ");
	strcat(buf, _itoa(D3D.CamX, itoabuf, 10));
	strcat(buf, " | ");
	strcat(buf, "CamY: ");
	strcat(buf, _itoa(D3D.CamY, itoabuf, 10));
	strcat(buf, " | ");
	strcat(buf, "Haste: ");
	strcat(buf, (Player.Haste) ? "On" : "Off");
	strcat(buf, " | ");
	strcat(buf, "Money: ");
	strcat(buf, _itoa(Player.Money, itoabuf, 10));
	strcat(buf, " | ");
	strcat(buf, "Map: ");
	strcat(buf, Textures.GetLoadedMap(Player.CurrentMapID)->MapName.c_str());
	strcat(buf, "\n");
	strcat(buf, "AbsPos: ");
	strcat(buf, _itoa(App.ClientCursorPos.x, itoabuf, 10));
	strcat(buf, ", ");
	strcat(buf, _itoa(App.ClientCursorPos.y, itoabuf, 10));
	strcat(buf, " | ");
	strcat(buf, "RelPos: ");
	strcat(buf, _itoa(DI.RelPos.x, itoabuf, 10));
	strcat(buf, ", ");
	strcat(buf, _itoa(DI.RelPos.y, itoabuf, 10));
	strcat(buf, " | ");
	strcat(buf, "L: ");
	strcat(buf, (DI.IsMouseButtonDown(0)) ? "True" : "False");
	strcat(buf, " M: ");
	strcat(buf, (DI.IsMouseButtonDown(2)) ? "True" : "False");
	strcat(buf, " R: ");
	strcat(buf, (DI.IsMouseButtonDown(1)) ? "True" : "False");

	D3D.Text(D3D.FrostFont, 10, 10, D3DCOLOR_XRGB( 0, 0, 0xFF ), buf);
}

void DrawLevel()
{
	char buf[10];
	ZeroMemory(&buf, sizeof(buf));

	strcat(buf, "Lv. ");
	strcat(buf, _itoa(Player.Level, itoabuf, 10));

	D3D.Text(D3D.FrostFont, 15, 710, D3DCOLOR_XRGB( 0, 0, 0xFF ), buf);
}

void Direct3D::DrawScene()
{
	// Clear the screen buffer and begin the scene.
    D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255,0,128), 1.0f, 0);
	D3DDevice->BeginScene();

	// Tell the sprite interface to prepare for drawing.
	Sprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_DONOTMODIFY_RENDERSTATE);

	// Render Map Backgrounds.
	Textures.DrawAllBackgrounds();

	// Render Platforms.
	Textures.DrawAllPlatforms();

	// Render Mobs.
	Textures.DrawAllMobs();

	// Render Mob Stats.
	Textures.DrawAllMobStats();

	// Move and Render the weapon(s).
	Attacking.Skills.StarSmash.Do();
	Attacking.Skills.StarSmash.Draw();

	// Draw everyone else on the map.
	Textures.DrawAllOtherPlayers();

	// Render Player.
	Player.Draw(DRAW_WORLD);

	// Render Items.
	Textures.DrawAllItems();

	// Render portals.
	Textures.DrawAllPortals();

	// Check if the player has been damaged by a mob.
	Textures.CheckMobsForDamage();

	// Apply Gravity to Players/Mobs.
	Gravity.PlayerGravityCheck(Player);
	Textures.GravityAllMobs();

	// Apply monster movement sequences.
	Textures.AutoMoveAllMobs();

	// Check all monsters for if they're being attacked.
	if (Attacking.Skills.StarSmash.Visible == true)
	{
		Textures.DetectAttacksOnAllMobs(Attacking.Skills.StarSmash.Star1);
		Textures.DetectAttacksOnAllMobs(Attacking.Skills.StarSmash.Star2);
	}

	// Draw Level BG and number string.
	UI.DrawLevelBG();
	DrawLevel();

	// Render status text.
	DrawStats();

	// Other stuff
	CheckForCharMovement();

	// The status indicator bars are always drawn, and towards the end as well so they stay above most other things.
	UI.DrawBarBGs();
	UI.DrawHPBar();
	UI.DrawMPBar();
	UI.DrawEXPBar();
	UI.DrawHPText( FrostFont, 100+10, 700+15, D3DCOLOR_XRGB( 0, 0, 0xFF ) );
	UI.DrawMPText( FrostFont, 100+210+10, 700+15, D3DCOLOR_XRGB( 0, 0, 0xFF ) );
	UI.DrawEXPText( FrostFont, 100+210+210+10, 700+15, D3DCOLOR_XRGB( 0, 0, 0xFF ) );

	GetDesktop()->DrawAll();

	// Tell the sprite interface to wrap it up.
	Sprite->End();

	// Control auto-scrolling of the player's view based on character co-ords.
	UI.AutoScrollCamera();

	// End the scene and present it.
    D3DDevice->EndScene();
    D3DDevice->Present(NULL, NULL, NULL, NULL);
}