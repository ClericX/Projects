// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <d3d9.h>
#include "C:/Program Files (x86)/Microsoft DirectX SDK (August 2008)/Include/d3dx9.h"

// include the Direct3D Library files
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "C:/Program Files (x86)/Microsoft DirectX SDK (August 2008)/Lib/x86/d3dx9.lib")

class HiResTimer
{
public:
	LARGE_INTEGER Start;
	LARGE_INTEGER Stop;
	LARGE_INTEGER Frequency;

	bool Initialize()
	{
		Start.QuadPart = Stop.QuadPart = Frequency.QuadPart = 0;
		if (!QueryPerformanceFrequency(&Frequency))
		{
			return false;
		}
		else
		{
			QueryPerformanceCounter(&Start);
			Stop = Start;
			return true;
		}
	}

	void Update()
	{
		Start = Stop;
		QueryPerformanceCounter(&Stop);
	}

	LONGLONG GetElapsedMicroseconds()
	{
		return (Stop.QuadPart - Start.QuadPart);
	}

	double GetElapsedSeconds()
	{
		return ((double)GetElapsedMicroseconds() / (double)Frequency.QuadPart);
	}

	double GetElapsedMilliseconds()
	{
		return (GetElapsedSeconds() * 1000);
	}

	float GetFPS()
	{
		/*
		static float ReturnFPS = 0;
		static float FPSCounter = 0;
		static float ElapsedSecTime = 0;
		ElapsedSecTime += GetElapsedMilliseconds();
		FPSCounter++;
		if (ElapsedSecTime >= 1000)
		{
			ElapsedSecTime = 0;
			ReturnFPS = FPSCounter;
			FPSCounter = 0;
		}
		return ReturnFPS;
		*/

		static const int NUM_FPS_SAMPLES = 100;
		static float fpsSamples[NUM_FPS_SAMPLES] = { 0 };
		static int currentSample = 0;

		fpsSamples[currentSample] = 1.0f / (float)GetElapsedSeconds();

		float fps = 0;

		for (int i = 0; i < NUM_FPS_SAMPLES; i++)
			fps += fpsSamples[i];

		fps /= NUM_FPS_SAMPLES;

		currentSample++;

		if (currentSample > NUM_FPS_SAMPLES)
			currentSample = 0;

		return fps;
	}

	void LockFPS(float TargetFPS)
	{
		LARGE_INTEGER OldLastTime = Stop;
		while (GetFPS() > TargetFPS)
		{
			QueryPerformanceCounter(&Stop);
		}
		Stop = OldLastTime;
	}
};

HiResTimer hrTimer;

int getRandom(int min, int max)
{
	return (rand() % max) + min;
}

float distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
}

// global variables
HWND hwnd;
unsigned int wndwidth = 100;
unsigned int wndheight = 100;
int mx = 0;
int my = 0;
bool mousePressed = false;
bool wasMousePressed = false;
MSG msg;
WNDCLASSEX wcex;
LPDIRECT3D9 d3d;				// the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;		// the pointer to the device class
LPD3DXSPRITE sprite;			// the sprite object

// button press state defines
#define BUTTON_PRESS_STATE_UP 1
#define BUTTON_PRESS_STATE_HOVERED 2
#define BUTTON_PRESS_STATE_DOWN 3

// gamestate defines
#define GAMESTATE_MAINMENU 1
#define GAMESTATE_BANDCREATION 2
#define GAMESTATE_BANDMANAGEMENT 3
#define GAMESTATE_OVERWORLD 4
#define GAMESTATE_CHARACTERDETAILS 5
#define GAMESTATE_BATTLEFIELD 6
#define GAMESTATE_BATTLESTAGE 7
#define GAMESTATE_CREDITS 8

unsigned int currentGameState = GAMESTATE_MAINMENU;

void setState(int gs)
{
	currentGameState = gs;
}

// misc variables
LPD3DXFONT mainMenuOptionFont;
LPD3DXFONT textboxFont;

class Texture
{
public:
	unsigned int width;
	unsigned int height;
	LPDIRECT3DTEXTURE9 pTex;

	static Texture *Load(std::string path)
	{
		Texture *ret = new Texture();

		D3DXIMAGE_INFO d3dx_ii;
		D3DXCreateTextureFromFileExA(d3ddev, path.c_str(), D3DX_FROM_FILE, D3DX_FROM_FILE, D3DX_FROM_FILE, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, &d3dx_ii, 0, &ret->pTex);
		ret->width = d3dx_ii.Width;
		ret->height = d3dx_ii.Height;

		return ret;
	}
};

// background textures
Texture *mainMenuBackground;
Texture *bandCreationBackground;
Texture *overworldBackground;

// battlefield textures
Texture *grassTileTexture;
Texture *movementTileOverlay;
Texture *attackTileOverlay;
Texture *menuPanelTexture;
Texture *menuCursorTexture;
Texture *statPanelTexture;
Texture *charIndicatorTexture;

bool mouseClicked()
{
	return mousePressed && !wasMousePressed;
}

int getTileFromAbs(int absVal)
{
	return (int)floor((float)absVal / 64.0f);
}

bool pointInBox(int px, int py, int rx, int ry, int rw, int rh)
{
	return px >= rx && py >= ry && px <= rx + rw && py <= ry + rh;
}

void drawString(LPD3DXFONT fnt, LPCSTR txt, unsigned int x, unsigned int y, DWORD color = D3DCOLOR_XRGB(255, 255, 255))
{
	RECT fontRect;
	fontRect.left = x;
	fontRect.top = y;
	unsigned int txtlen = strlen(txt);
	fnt->DrawTextA(sprite, txt, txtlen, &fontRect, DT_CALCRECT, color);
	fnt->DrawTextA(sprite, txt, txtlen, &fontRect, DT_LEFT, color);
}

class DrawnObject
{
public:
	DrawnObject()
	{
		setLocation(0, 0);
		setSize(0, 0);
	}

	int x;
	int y;
	int width;
	int height;

	void setLocation(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	void setSize(int _width, int _height)
	{
		width = _width;
		height = _height;
	}

	bool isHovered()
	{
		return pointInBox(mx, my, x, y, width, height);
	}

	bool isPressed()
	{
		return isHovered() && mousePressed;
	}

	virtual void draw() = 0;
};

class GUIWindow : public DrawnObject
{
public:
	std::string text;

	void setText(std::string txt)
	{
		text = txt;
	}
};

class GUIStatic : public GUIWindow
{
public:
	GUIStatic()
	{
		font = textboxFont;
	}

	LPD3DXFONT font;
	DWORD color;

	void draw()
	{
		drawString(font, text.c_str(), x, y, color);
	}
};

class GUIButton : public GUIWindow
{
public:
	GUIButton(unsigned int _x, unsigned int _y, std::string txt)
	{
		setLocation(_x, _y);
		setSize(300, 75);
		setText(txt);
		label.font = mainMenuOptionFont;
		label.color = D3DCOLOR_XRGB(0, 0, 0);

		pressState = BUTTON_PRESS_STATE_UP;
		onClick = 0;
	}

	GUIStatic label;
	unsigned int pressState;

	void (*onClick)();

	void setLocation(int _x, int _y)
	{
		GUIWindow::setLocation(_x, _y);
		label.setLocation(x + 30, y + 10);
	}

	void setText(std::string txt)
	{
		label.setText(txt);
	}

	void processPressState()
	{
		if (pressState == BUTTON_PRESS_STATE_UP && isHovered())
		{
			pressState = BUTTON_PRESS_STATE_HOVERED;
		}
		else if (pressState == BUTTON_PRESS_STATE_HOVERED && isPressed())
		{
			pressState = BUTTON_PRESS_STATE_DOWN;
		}
		else if (pressState == BUTTON_PRESS_STATE_HOVERED && !isHovered())
		{
			pressState = BUTTON_PRESS_STATE_UP;
		}
		else if (pressState == BUTTON_PRESS_STATE_DOWN && !isPressed())
		{
			pressState = BUTTON_PRESS_STATE_UP;

			if (onClick != 0)
			{
				onClick();
			}
		}
	}

	void draw()
	{
		processPressState();

		D3DXVECTOR3 pos((float)x, (float)y, 0);

		if (pressState == BUTTON_PRESS_STATE_DOWN)
		{
			sprite->Draw(downImg->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		}
		else if (pressState == BUTTON_PRESS_STATE_HOVERED)
		{
			sprite->Draw(hoverImg->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		}
		else if (pressState == BUTTON_PRESS_STATE_UP)
		{
			sprite->Draw(upImg->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		}

		label.draw();
	}

	static Texture *upImg;
	static Texture *hoverImg;
	static Texture *downImg;
};

Texture *GUIButton::upImg = 0;
Texture *GUIButton::hoverImg = 0;
Texture *GUIButton::downImg = 0;

class GUITextbox : public GUIWindow
{
public:
	GUITextbox()
	{
		setSize(300, 35);
		label.font = textboxFont;
	}

	GUIStatic label;

	void setLocation(int _x, int _y)
	{
		GUIWindow::setLocation(_x, _y);
		label.setLocation(x + 5, y + 5);
	}

	void draw()
	{
		if (isPressed())
		{
			activeTextbox = this;
		}

		D3DXVECTOR3 pos((float)x, (float)y, 0);
		sprite->Draw(backImg->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		pos.x += 5;
		pos.y += 5;
		sprite->Draw(cursorImg->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));

		label.draw();
	}

	static Texture *cursorImg;
	static Texture *backImg;

	static GUITextbox *activeTextbox;
};

Texture *GUITextbox::cursorImg = 0;
Texture *GUITextbox::backImg = 0;

GUITextbox *GUITextbox::activeTextbox = 0;

// menu gui elements
GUIButton *newGameButton = 0;
GUIButton *loadGameButton = 0;
GUIButton *creditsButton = 0;
GUIButton *exitGameButton = 0;

// band creation gui elements
GUIStatic *bandNameLabel = 0;
GUITextbox *bandNameTextbox = 0;
GUIButton *bandStartButton = 0;
GUIButton *bandRandomizeButton1 = 0;
GUIButton *bandRandomizeButton2 = 0;
GUIButton *bandRandomizeButton3 = 0;

// credits gui elements
GUIStatic *creditsTextLabel = 0;
GUIButton *creditsBackButton = 0;

class UnitAnimation
{
public:
	std::string path;
	std::string name;
	unsigned int numFrames;
	unsigned int frameDelay;
	std::vector<Texture *> armsFrames;
	std::vector<Texture *> armorFrames;
	std::vector<Texture *> bodyFrames;
	std::vector<Texture *> hatFrames;
	std::vector<Texture *> headFrames;
	std::vector<Texture *> legsFrames;
	std::vector<Texture *> shieldFrames;
	std::vector<Texture *> weaponFrames;

	void loadAnimationSet(std::string setName, std::vector<Texture *> *frameVec)
	{
		for (unsigned int i = 0; i < numFrames; i++)
		{
			std::stringstream framePath;
			framePath << path << "/" << setName << "/" << i << ".png";
			frameVec->push_back(Texture::Load(framePath.str().c_str()));
		}
	}

	static UnitAnimation *Load(std::string _name)
	{
		UnitAnimation *ret = new UnitAnimation();
		ret->name = _name;
		ret->path = "GGJ/Images/Characters/Animations/" + _name;

		// acquire frame delay
		char infoBuf[10];
		std::ifstream finfo;
		finfo.open((ret->path + "/info.dat").c_str(), std::ios::in);
		finfo.getline(infoBuf, 10);
		ret->numFrames = atoi(infoBuf);
		finfo.getline(infoBuf, 10);
		ret->frameDelay = atoi(infoBuf);
		finfo.close();

		// load all the frames
		ret->loadAnimationSet("Arm", &ret->armsFrames);
		ret->loadAnimationSet("Armor", &ret->armorFrames);
		ret->loadAnimationSet("Body", &ret->bodyFrames);
		ret->loadAnimationSet("Hat", &ret->hatFrames);
		ret->loadAnimationSet("Head", &ret->headFrames);
		ret->loadAnimationSet("Legs", &ret->legsFrames);
		ret->loadAnimationSet("Shield", &ret->shieldFrames);
		ret->loadAnimationSet("Weapon", &ret->weaponFrames);

		return ret;
	}
};

UnitAnimation *idleAnimation = 0;
UnitAnimation *deathAnimation = 0;
UnitAnimation *attackAnimation = 0;

class Item
{
public:
	unsigned int id;
	enum ItemType
	{
		ITEMTYPE_WEAPON,
		ITEMTYPE_ARMOR,
		ITEMTYPE_SHIELD,
		ITEMTYPE_HELMET,
		ITEMTYPE_CONSUMABLE,
		ITEMTYPE_MISC
	} type;
	unsigned int weight;
	unsigned int modifier;
};

class FightingUnit : public DrawnObject
{
public:
	FightingUnit()
	{
		name = "Unnamed Unit";
		attack = 1000;
		defense = 5;
		hp = 5;
		maxhp = 5;
		speed = getRandom(1, 100);
		accuracy = 5;
		avoid = 5;
		experience = 0;
		happiness = 0;
		move = 5;
		lastTurnNumber = 0;

		// init portion colors all to white
		headColor = D3DCOLOR_XRGB(255, 255, 255);
		bodyColor = D3DCOLOR_XRGB(255, 255, 255);
		legColor = D3DCOLOR_XRGB(255, 255, 255);
		armColor = D3DCOLOR_XRGB(255, 255, 255);

		// init animation info to defaults
		setAnimation(idleAnimation);
	}

	std::string name;
	int id;
	int attack;
	int defense;
	int hp;
	int maxhp;
	int speed;
	int accuracy;
	int avoid;
	int experience;
	int happiness;
	int move;
	int lastTurnNumber;

	unsigned int getLevelUpEXP()
	{
		return 100;
	}
	
	Item *hat;
	Item *shield;
	Item *weapon;
	Item *armor;

	DWORD headColor;
	DWORD bodyColor;
	DWORD legColor;
	DWORD armColor;

	UnitAnimation *currentAnimation;
	unsigned int currentAnimationFrame;
	double currentAnimationTime;

	void setAnimation(UnitAnimation *ua)
	{
		currentAnimation = ua;
		currentAnimationFrame = 0;
		currentAnimationTime = 0.0f;
	}

	void moveAnimation(double elapsedTime)
	{
		currentAnimationTime += elapsedTime;

		if (currentAnimationTime >= currentAnimation->frameDelay)
		{
			currentAnimationFrame++;
			currentAnimationTime = 0;
		}
		
		if (currentAnimationFrame >= currentAnimation->numFrames)
		{
			currentAnimationFrame = 0;
		}
	}
	
	int tileStepsTo(int _x, int _y)
	{
		return abs(getTileFromAbs(x) - _x) + abs(getTileFromAbs(y) - _y);
	}

	void draw(int xOff, int yOff)
	{
		D3DXVECTOR3 pos((float)x + xOff, (float)y + yOff, 0);
		sprite->Draw(currentAnimation->legsFrames[currentAnimationFrame]->pTex, 0, 0, &pos, legColor);
		sprite->Draw(currentAnimation->bodyFrames[currentAnimationFrame]->pTex, 0, 0, &pos, bodyColor);
		sprite->Draw(currentAnimation->armsFrames[currentAnimationFrame]->pTex, 0, 0, &pos, armColor);
		sprite->Draw(currentAnimation->armorFrames[currentAnimationFrame]->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		sprite->Draw(currentAnimation->headFrames[currentAnimationFrame]->pTex, 0, 0, &pos, headColor);
		sprite->Draw(currentAnimation->hatFrames[currentAnimationFrame]->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		sprite->Draw(currentAnimation->shieldFrames[currentAnimationFrame]->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		sprite->Draw(currentAnimation->weaponFrames[currentAnimationFrame]->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
	}

	void draw()
	{
		draw(0, 0);
	}

	void attackUnit(FightingUnit *fu)
	{
		fu->hp -= std::max<int>(1, attack - fu->defense);

		if (fu->hp <= 0)
		{
			fu->hp = 0;
			fu->die();
		}
	}

	void die()
	{
		setAnimation(deathAnimation);
	}

	bool dead()
	{
		return hp <= 0;
	}
};

class Band
{
public:
	Band()
	{
		money = 0;
	}

	unsigned int money;
	std::vector<Item *> items;
	std::vector<FightingUnit *> units;
	std::string name;
};

Band *activeBand = 0;

class BattleInstance
{
public:
	BattleInstance(FightingUnit *_attacker, FightingUnit *_defender)
	{
		attacker = _attacker;
		defender = _defender;
	}

	FightingUnit *attacker;
	FightingUnit *defender;

	int getAttackerDamage()
	{
		return 1;
	}

	int getDefenderDamage()
	{
		return 1;
	}
};

class UnitTurnExecution
{
public:
	UnitTurnExecution()
	{
		battle = 0;
		isMoving = false;
		hasMoved = false;
		isAttacking = false;
		hasAttacked = false;
		menuIsVisible = true;
		selectedMenuOption = 0;
	}

	BattleInstance *battle;

	bool isMoving;
	bool hasMoved;

	bool isAttacking;
	bool hasAttacked;

	bool menuIsVisible;
	int selectedMenuOption;

	void setBattle(FightingUnit *attacker, FightingUnit *defender)
	{
		if (battle != 0)
		{
			delete battle;
		}

		battle = new BattleInstance(attacker, defender);
	}

	void startMoving()
	{
		isMoving = true;
		menuIsVisible = false;
	}

	void cancelMoving()
	{
		isMoving = false;
		menuIsVisible = true;
	}

	void endMoving()
	{
		cancelMoving();
		hasMoved = true;
	}

	void startAttacking()
	{
		isAttacking = true;
		menuIsVisible = false;
	}

	void cancelAttacking()
	{
		isAttacking = false;
		menuIsVisible = true;
	}

	void endAttacking()
	{
		cancelAttacking();
		hasAttacked = true;
	}
};

class Battlefield
{
public:
	Battlefield()
	{
		currentTurnIndex = 0;

		// add party to ally list and assign units to default positions (for now)
		allies.push_back(activeBand->units[0]);
		allies.push_back(activeBand->units[1]);
		allies.push_back(activeBand->units[2]);

		moveUnitToTile(allies[0], 0, 2);
		moveUnitToTile(allies[1], 0, 3);
		moveUnitToTile(allies[2], 0, 4);

		// spawn the enemies in the battlefield and assign default positions (for now)
		enemies.push_back(new FightingUnit());
		enemies.push_back(new FightingUnit());
		enemies.push_back(new FightingUnit());

		moveUnitToTile(enemies[0], 15, 2);
		moveUnitToTile(enemies[1], 15, 3);
		moveUnitToTile(enemies[2], 15, 4);

		// now that we have all the players we want in the battlefield, gather them all into one list for turn order
		generateTurnList();

		// Start the first unit's turn in the battlefield
		currentTurn = 0;
		endCurrentTurn();
	}

	FightingUnit *getAllyAt(int _x, int _y)
	{
		for (unsigned int i = 0; i < allies.size(); i++)
		{
			if (getTileFromAbs(allies[i]->x) == _x && getTileFromAbs(allies[i]->y) == _y)
			{
				return allies[i];
			}
		}

		return (FightingUnit *)0;
	}

	FightingUnit *getEnemyAt(int _x, int _y)
	{
		for (unsigned int i = 0; i < enemies.size(); i++)
		{
			if (getTileFromAbs(enemies[i]->x) == _x && getTileFromAbs(enemies[i]->y) == _y)
			{
				return enemies[i];
			}
		}

		return (FightingUnit *)0;
	}

	FightingUnit *getUnitAt(int _x, int _y)
	{
		FightingUnit *ret = getAllyAt(_x, _y);

		if (ret == 0)
		{
			ret = getEnemyAt(_x, _y);
		}

		return ret;
	}

	bool isAlly(FightingUnit *fu)
	{
		return (getAllyAt(getTileFromAbs(fu->x), getTileFromAbs(fu->y)) != 0);
	}

	bool isEnemy(FightingUnit *fu)
	{
		return !isAlly(fu);
	}

	FightingUnit *getNextUnitTurn()
	{
		FightingUnit *ret = 0;
		// this would fail if everyone is dead OR no units on the map
		while (ret == 0 || ret->dead())
		{
			ret = turnList[currentTurnIndex];
			currentTurnIndex++;
			if ((unsigned int)currentTurnIndex >= turnList.size())
			{
				currentTurnIndex = 0;
			}
		}
		return ret;
	}

	void endCurrentTurn()
	{
		if (currentTurn != 0)
		{
			delete currentTurn;
		}

		currentTurn = new UnitTurnExecution();
		selectedUnit = getNextUnitTurn();
	}

	void generateTurnList()
	{
		for (unsigned int i = 0; i < allies.size(); i++)
		{
			turnList.push_back(allies[i]);
		}

		for (unsigned int i = 0; i < enemies.size(); i++)
		{
			turnList.push_back(enemies[i]);
		}
	}

	bool canMoveToTile(FightingUnit *fu, int _x, int _y)
	{
		// this might have to consider terrain restrictions
		return (fu->tileStepsTo(_x, _y) <= (int)fu->move && fu->tileStepsTo(_x, _y) != 0) && getUnitAt(_x, _y) == 0;
	}

	bool canAttackTile(FightingUnit *fu, int _x, int _y)
	{
		// this will have to change when weapon ranges have to be taken into consideration
		return fu->tileStepsTo(_x, _y) == 1 && getUnitAt(_x, _y) != 0;
	}

	void drawUnit(FightingUnit *fu)
	{
		fu->draw(-32, -64);
	}

	void moveUnitToTile(FightingUnit *fu, int _x, int _y)
	{
		fu->setLocation((_x * 64), (_y * 64));
	}

	std::vector<FightingUnit *> allies;
	std::vector<FightingUnit *> enemies;
	std::vector<FightingUnit *> turnList;
	int currentTurnIndex;

	FightingUnit *selectedUnit;
	UnitTurnExecution *currentTurn;
};

Battlefield *activeBattlefield = 0;

class OverworldTown : public DrawnObject
{
public:
	OverworldTown()
	{
		id = 0;
		revealed = false;
		setTexture(townTex);
	}

	int id;
	bool revealed;
	std::vector<int> neighbours;

	void setTexture(Texture *t)
	{
		setSize(t->width, t->height);
	}

	void moveHere()
	{
		activeBand->units[0]->x = this->x - 32;
		activeBand->units[0]->y = this->y - 64;

		if (!revealed)
		{
			setState(GAMESTATE_BATTLEFIELD);
			activeBattlefield = new Battlefield();
			// NOTE: THIS IS NOT NECESSARILY TRUE
			revealed = true;
		}
	}

	void draw()
	{
		if (isPressed())
		{
			moveHere();
		}

		D3DXVECTOR3 pos((float)x, (float)y, 0);
		sprite->Draw(revealed ? townTex->pTex : nullTownTex->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
	}

	static Texture *nullTownTex;
	static Texture *townTex;
};

Texture *OverworldTown::nullTownTex = 0;
Texture *OverworldTown::townTex = 0;

class Overworld
{
public:
	std::vector<OverworldTown *> towns;

	OverworldTown *getTownById(int id)
	{
		for (unsigned int i = 0; i < towns.size(); i++)
		{
			if (towns[i]->id == id)
			{
				return towns[i];
			}
		}

		return (OverworldTown *)0;
	}
};

Overworld *activeOverworld = 0;

void newGameButton_onClick()
{
	activeBand = new Band();

	for (unsigned int i = 0; i < 3; i++)
	{
		FightingUnit *u = new FightingUnit();
		u->x = 125 + (325 * i);
		u->y = 300;
		activeBand->units.push_back(u);
	}

	setState(GAMESTATE_BANDCREATION);
}

void loadGameButton_onClick()
{
	MessageBoxA(0, "this doesn't work yet", "ok", MB_OK);
}

void creditsButton_onClick()
{
	setState(GAMESTATE_CREDITS);
}

void bandRandomizeButton1_onClick()
{
	FightingUnit *u = activeBand->units[0];
	u->headColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->armColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->bodyColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->legColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
}

void bandRandomizeButton2_onClick()
{
	FightingUnit *u = activeBand->units[1];
	u->headColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->armColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->bodyColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->legColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
}

void bandRandomizeButton3_onClick()
{
	FightingUnit *u = activeBand->units[2];
	u->headColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->armColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->bodyColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
	u->legColor = D3DCOLOR_XRGB(getRandom(0, 255), getRandom(0, 255), getRandom(0, 255));
}

void bandStartButton_onClick()
{
	activeBand->name = bandNameTextbox->label.text;
	activeOverworld->towns[0]->moveHere();

	setState(GAMESTATE_OVERWORLD);
}

void creditsBackButton_onClick()
{
	setState(GAMESTATE_MAINMENU);
}

void exitGameButton_onClick()
{
	DestroyWindow(hwnd);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_DESTROY:
            {
                PostQuitMessage(0);
                return 0;
            }

		case WM_MOUSEMOVE:
			{
				mx = GET_X_LPARAM(lParam);
				my = GET_Y_LPARAM(lParam);

				/*
				POINT mPos;
				GetCursorPos(&mPos);
				ScreenToClient(hwnd, &mPos);
				mx = mPos.x;
				my = mPos.y;
				*/

				break;
			}

		case WM_LBUTTONDOWN:
			{
				mousePressed = true;
				break;
			}

		case WM_LBUTTONUP:
			{
				mousePressed = false;
				break;
			}

		case WM_CHAR:
			{
				if (GUITextbox::activeTextbox != 0)
				{
					char key = (char)wParam;

					if (key == VK_BACK)
					{
						GUITextbox::activeTextbox->label.text = GUITextbox::activeTextbox->label.text.substr(0, GUITextbox::activeTextbox->label.text.length() - 1);
					}
					else
					{
						GUITextbox::activeTextbox->label.text += (char)wParam;
					}
				}
				break;
			}
    }

    return DefWindowProc (hWnd, message, wParam, lParam);
}

void initWindow(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	wndwidth = width;
	wndheight = height;

    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.lpszClassName = "WindowClass";

    RegisterClassEx(&wcex);

	RECT wndRect;
	wndRect.left = 0;
	wndRect.top = 0;
	wndRect.right = wndwidth;
	wndRect.bottom = wndheight;
	AdjustWindowRect(&wndRect, 0, true);

    hwnd = CreateWindowEx(NULL,
                          "WindowClass",
                          "game",
                          WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
						  200, 200,
						  wndRect.right - wndRect.left, wndRect.bottom - wndRect.top,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    ShowWindow(hwnd, nCmdShow);
}

void initD3D()
{
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;

    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.hDeviceWindow = hwnd;
    d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
    d3dpp.BackBufferWidth = wndwidth;
    d3dpp.BackBufferHeight = wndheight;

    // create a device class using this information and the info from the d3dpp stuct
    d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hwnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);
}

void beginScene()
{
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
    d3ddev->BeginScene();
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void endScene()
{
	sprite->End();
    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void initResources()
{
	// general resources
	D3DXCreateSprite(d3ddev, &sprite);
	D3DXCreateFont(d3ddev, 50, 20, 0, 0, false, 0, 0, 0, 0, "Arial", &mainMenuOptionFont);
	D3DXCreateFont(d3ddev, 20, 10, 0, 0, false, 0, 0, 0, 0, "Arial", &textboxFont);

	// character appearance/animation frames
	idleAnimation = UnitAnimation::Load("Idle");
	deathAnimation = UnitAnimation::Load("Death");
	attackAnimation = UnitAnimation::Load("Attack");

	// menu resources
	mainMenuBackground = Texture::Load("GGJ/Images/Menu/Backgrounds/title.png");

	// band creation resources
	bandCreationBackground = Texture::Load("GGJ/Images/BandCreation/bg.png");

	// overworld resources
	overworldBackground = Texture::Load("GGJ/Images/Overworld/bg.png");
	OverworldTown::nullTownTex = Texture::Load("GGJ/Images/Overworld/Icon/nulltown.png");
	OverworldTown::townTex = Texture::Load("GGJ/Images/Overworld/Icon/town1.png");

	// button resources
	GUIButton::upImg = Texture::Load("GGJ/Images/GUI/Buttons/upButton.png");
	GUIButton::hoverImg = Texture::Load("GGJ/Images/GUI/Buttons/hoverButton.png");
	GUIButton::downImg = Texture::Load("GGJ/Images/GUI/Buttons/downButton.png");

	// textbox resources
	GUITextbox::cursorImg = Texture::Load("GGJ/Images/GUI/Textbox/cursor.png");
	GUITextbox::backImg = Texture::Load("GGJ/Images/GUI/Textbox/textBG.png");

	// init gui elements for the main menu
	newGameButton = new GUIButton(180, 200, "New Game");
	newGameButton->onClick = newGameButton_onClick;
	loadGameButton = new GUIButton(180, 300, "Load Game");
	loadGameButton->onClick = loadGameButton_onClick;
	creditsButton = new GUIButton(180, 400, "Credits");
	creditsButton->onClick = creditsButton_onClick;
	exitGameButton = new GUIButton(180, 500, "Exit Game");
	exitGameButton->onClick = exitGameButton_onClick;

	// init gui elements for band creation
	bandNameLabel = new GUIStatic();
	bandNameLabel->setText("Band name:");
	bandNameLabel->setLocation(400, 50);
	bandNameTextbox = new GUITextbox();
	bandNameTextbox->setLocation(400, 100);
	bandStartButton = new GUIButton(375, 650, "Start Game");
	bandStartButton->onClick = bandStartButton_onClick;
	bandRandomizeButton1 = new GUIButton(50, 500, "Randomize!");
	bandRandomizeButton1->onClick = bandRandomizeButton1_onClick;
	bandRandomizeButton2 = new GUIButton(375, 500, "Randomize!");
	bandRandomizeButton2->onClick = bandRandomizeButton2_onClick;
	bandRandomizeButton3 = new GUIButton(700, 500, "Randomize!");
	bandRandomizeButton3->onClick = bandRandomizeButton3_onClick;

	// init gui elements for credits
	creditsTextLabel = new GUIStatic();
	creditsTextLabel->setText("hi these are credits ok?");
	creditsBackButton = new GUIButton(375, 650, "Go Back");
	creditsBackButton->onClick = creditsBackButton_onClick;

	// init overworld and it's towns
	activeOverworld = new Overworld();

	char overworldBuf[50];
	std::ifstream finfo;
	finfo.open("GGJ/Images/Overworld/nodes.dat", std::ios::in);
	// yeah this is a pretty bad way to do it but the string class doesn't have a split function and i feel too lazy right now
	for (unsigned int i = 0; i < 14; i++)
	{
		finfo.getline(overworldBuf, 50);
		OverworldTown *loadingTown = new OverworldTown();
		char *tokens = strtok(overworldBuf, "|");
		loadingTown->id = atoi(tokens);
		tokens = strtok(0, "|");
		loadingTown->x = atoi(tokens);
		tokens = strtok(0, "|");
		loadingTown->y = atoi(tokens);

		while (tokens != 0)
		{
			tokens = strtok(0, "|");
			if (tokens != 0)
			{
				loadingTown->neighbours.push_back(atoi(tokens));
			}
		}

		activeOverworld->towns.push_back(loadingTown);
	}
	finfo.close();

	// init battlefield elements
	grassTileTexture = Texture::Load("GGJ/Images/Battle/Tiles/grass.png");
	movementTileOverlay = Texture::Load("GGJ/Images/Battle/UI/moveIndicator.png");
	attackTileOverlay = Texture::Load("GGJ/Images/Battle/UI/attackIndicator.png");
	menuPanelTexture = Texture::Load("GGJ/Images/Battle/UI/menuPanel.png");
	menuCursorTexture = Texture::Load("GGJ/Images/Battle/UI/menuCursor.png");
	statPanelTexture = Texture::Load("GGJ/Images/Battle/UI/statPanel.png");
	charIndicatorTexture = Texture::Load("GGJ/Images/Characters/indicator.png");
}

void drawScene()
{
	if (currentGameState == GAMESTATE_MAINMENU)
	{
		D3DXVECTOR3 pos(0, 0, 0);
		sprite->Draw(mainMenuBackground->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		newGameButton->draw();
		loadGameButton->draw();
		creditsButton->draw();
		exitGameButton->draw();
	}
	else if (currentGameState == GAMESTATE_BANDCREATION)
	{
		D3DXVECTOR3 pos(0, 0, 0);
		sprite->Draw(bandCreationBackground->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		for (unsigned int i = 0; i < 3; i++)
		{
			activeBand->units[i]->draw();
		}
		bandNameLabel->draw();
		bandNameTextbox->draw();
		bandRandomizeButton1->draw();
		bandRandomizeButton2->draw();
		bandRandomizeButton3->draw();
		bandStartButton->draw();
	}
	else if (currentGameState == GAMESTATE_OVERWORLD)
	{
		D3DXVECTOR3 pos(0, 0, 0);
		sprite->Draw(overworldBackground->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
		for (unsigned int i = 0; i < activeOverworld->towns.size(); i++)
		{
			activeOverworld->towns[i]->draw();
		}
		activeBand->units[0]->draw();
	}
	else if (currentGameState == GAMESTATE_BATTLEFIELD)
	{
		int mousedOverTileX = getTileFromAbs(mx);
		int mousedOverTileY = getTileFromAbs(my);
		FightingUnit *mousedOverUnit = activeBattlefield->getUnitAt(mousedOverTileX, mousedOverTileY);

		D3DXVECTOR3 pos(0, 0, 0);

		// draw all the map tiles on the field (do this first)
		for (unsigned int hTile = 0; hTile < 12; hTile++)
		{
			for (unsigned int vTile = 0; vTile < 16; vTile++)
			{
				sprite->Draw(grassTileTexture->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
				pos.x += 64;
			}

			pos.x = 0;
			pos.y += 64;
		}

		// turn execution processing
		if (activeBattlefield->currentTurn != 0)
		{
			// is in the process of selecting a movement
			if (activeBattlefield->currentTurn->isMoving)
			{
				// draw potential movement range
				for (unsigned int h = 0; h < 12; h++)
				{
					for (unsigned int w = 0; w < 16; w++)
					{
						if (activeBattlefield->canMoveToTile(activeBattlefield->selectedUnit, w, h))
						{
							pos.x = (float)(64 * w);
							pos.y = (float)(64 * h);
							sprite->Draw(movementTileOverlay->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
						}
					}
				}
			}

			// was moving and a click was made
			if (activeBattlefield->currentTurn->isMoving && mouseClicked())
			{
				// NOTE: canMoveToTile() should perform a unit (non)existence check itself.
				if (activeBattlefield->canMoveToTile(activeBattlefield->selectedUnit, mousedOverTileX, mousedOverTileY))
				{
					activeBattlefield->moveUnitToTile(activeBattlefield->selectedUnit, mousedOverTileX, mousedOverTileY);
					activeBattlefield->currentTurn->endMoving();
				}
				else
				{
					activeBattlefield->currentTurn->cancelMoving();
				}
			}

			// is in the process of selecting an attack target
			if (activeBattlefield->currentTurn->isAttacking)
			{
				// draw potential attack range
				for (unsigned int h = 0; h < 12; h++)
				{
					for (unsigned int w = 0; w < 16; w++)
					{
						if (activeBattlefield->canAttackTile(activeBattlefield->selectedUnit, w, h))
						{
							pos.x = (float)(64 * w);
							pos.y = (float)(64 * h);
							sprite->Draw(attackTileOverlay->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
						}
					}
				}
			}

			// was attacking and a click was made
			if (activeBattlefield->currentTurn->isAttacking && mouseClicked())
			{
				// NOTE: canAttackTile() should perform a unit existence check itself.
				if (activeBattlefield->canAttackTile(activeBattlefield->selectedUnit, mousedOverTileX, mousedOverTileY))
				{
					activeBattlefield->selectedUnit->attackUnit(activeBattlefield->getUnitAt(mousedOverTileX, mousedOverTileY));
					activeBattlefield->currentTurn->endAttacking();
					activeBattlefield->selectedUnit->setAnimation(attackAnimation);
				}
				else
				{
					activeBattlefield->currentTurn->cancelAttacking();
				}
			}

			// menu options are visible
			if (activeBattlefield->currentTurn->menuIsVisible)
			{
				pos.x = 0;
				pos.y = (float)(wndheight - 200);
				sprite->Draw(menuPanelTexture->pTex, 0, 0, &pos, D3DCOLOR_XRGB(100, 100, 255));
				pos.y += 20 + (50 * activeBattlefield->currentTurn->selectedMenuOption);
				sprite->Draw(menuCursorTexture->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
				pos.y -= (50 * activeBattlefield->currentTurn->selectedMenuOption);
				pos.x += 20;
				drawString(mainMenuOptionFont, "Move", (int)pos.x, (int)pos.y, activeBattlefield->currentTurn->hasMoved ? D3DCOLOR_XRGB(50, 50, 50) : D3DCOLOR_XRGB(255, 255, 255));
				drawString(mainMenuOptionFont, "Attack", (int)pos.x, (int)pos.y + 50, activeBattlefield->currentTurn->hasAttacked ? D3DCOLOR_XRGB(50, 50, 50) : D3DCOLOR_XRGB(255, 255, 255));
				drawString(mainMenuOptionFont, "Wait", (int)pos.x, (int)pos.y + 100, D3DCOLOR_XRGB(255, 255, 255));

				if (pointInBox(mx, my, (int)pos.x, (int)pos.y, 192, 50) && !activeBattlefield->currentTurn->hasMoved)
				{
					activeBattlefield->currentTurn->selectedMenuOption = 0;

					if (mouseClicked())
					{
						activeBattlefield->currentTurn->startMoving();
					}
				}
				else if (pointInBox(mx, my, (int)pos.x, (int)pos.y + 50, 192, 50) && !activeBattlefield->currentTurn->hasAttacked)
				{
					activeBattlefield->currentTurn->selectedMenuOption = 1;

					if (mouseClicked())
					{
						activeBattlefield->currentTurn->startAttacking();
					}
				}
				else if (pointInBox(mx, my, (int)pos.x, (int)pos.y + 100, 192, 50) || (activeBattlefield->currentTurn->hasMoved && activeBattlefield->currentTurn->hasAttacked))
				{
					activeBattlefield->currentTurn->selectedMenuOption = 2;

					if (mouseClicked())
					{
						activeBattlefield->endCurrentTurn();
					}
				}
			}
		}

		// draw all the units on the field
		for (unsigned int hTile = 0; hTile < 12; hTile++)
		{
			for (unsigned int vTile = 0; vTile < 16; vTile++)
			{
				FightingUnit *tileUnit = activeBattlefield->getUnitAt(vTile, hTile);
				if (tileUnit != 0)
				{
					if (!(tileUnit->dead() && tileUnit->currentAnimationFrame == 2))
					{
						tileUnit->moveAnimation(hrTimer.GetElapsedMilliseconds());
					}

					activeBattlefield->drawUnit(tileUnit);

					// draw the character indictor above the character who's turn it is' head
					if (tileUnit == activeBattlefield->selectedUnit)
					{
						pos.x = (float)tileUnit->x - 32;
						pos.y = (float)tileUnit->y - 64;
						sprite->Draw(charIndicatorTexture->pTex, 0, 0, &pos, D3DCOLOR_XRGB(255, 255, 255));
					}
				}
			}
		}

		// draw the stat display overlay on mouse over of units
		if (mousedOverUnit != 0)
		{
			pos.x = (float)wndwidth - 192;
			pos.y = (float)wndheight - 256;
			sprite->Draw(statPanelTexture->pTex, 0, 0, &pos, activeBattlefield->isAlly(mousedOverUnit) ? D3DCOLOR_XRGB(100, 100, 255) : D3DCOLOR_XRGB(255, 100, 100));

			std::stringstream ss;
			drawString(textboxFont, mousedOverUnit->name.c_str(), (int)pos.x + 20, (int)pos.y + 40);

			ss.str(std::string());
			ss << "HP: " << mousedOverUnit->hp << " / " << mousedOverUnit->maxhp;
			drawString(textboxFont, ss.str().c_str(), (int)pos.x + 20, (int)pos.y + 60);

			ss.str(std::string());
			ss << "Attack: " << mousedOverUnit->attack;
			drawString(textboxFont, ss.str().c_str(), (int)pos.x + 20, (int)pos.y + 80);

			ss.str(std::string());
			ss << "Defense: " << mousedOverUnit->defense;
			drawString(textboxFont, ss.str().c_str(), (int)pos.x + 20, (int)pos.y + 100);

			ss.str(std::string());
			ss << "Avoid: " << mousedOverUnit->avoid;
			drawString(textboxFont, ss.str().c_str(), (int)pos.x + 20, (int)pos.y + 120);

			ss.str(std::string());
			ss << "Accuracy: " << mousedOverUnit->accuracy;
			drawString(textboxFont, ss.str().c_str(), (int)pos.x + 20, (int)pos.y + 140);

			// draw the attack info menus when a target is moused over (and do it above unit drawing)
			if (activeBattlefield->currentTurn->isAttacking)
			{
				// define the (potential) battle's parameters if they're not already set
				if (activeBattlefield->currentTurn->battle == 0)
				{
					activeBattlefield->currentTurn->setBattle(activeBattlefield->selectedUnit, mousedOverUnit);
				}

				// TODO: THIS IS WHAT I'M DOING NEXT/ATM | display potential battle parameters in menus for the player
			}
		}
	}
	else if (currentGameState == GAMESTATE_CREDITS)
	{
		creditsTextLabel->draw();
		creditsBackButton->draw();
	}
}

void windowLoop()
{
    while (TRUE)
    {
		// handle window messages
        while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		// Use escape as a quit hotkey
		if (msg.message == WM_CHAR && msg.wParam == VK_ESCAPE)
		{
			if (activeBattlefield->currentTurn && (activeBattlefield->currentTurn->isMoving || activeBattlefield->currentTurn->isAttacking))
			{
				activeBattlefield->currentTurn->cancelMoving();
				activeBattlefield->currentTurn->cancelAttacking();
			}
			else
			{
				//DestroyWindow(hwnd);
			}
		}

		// Quit when recieving the quit message
        if(msg.message == WM_QUIT)
            break;

		hrTimer.Update();

		// render a frame if we're good to go
		beginScene();
        drawScene();
		endScene();

		// make mouseClicked() properly functional to detect recent clicks
		if (mousePressed && !wasMousePressed)
		{
			wasMousePressed = true;
		}
		if (!mousePressed && wasMousePressed)
		{
			wasMousePressed = false;
		}
    }
}

void cleanD3D()
{
    d3ddev->Release();    // close and release the 3D device
    d3d->Release();    // close and release Direct3D
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand((unsigned int)time(0));
	initWindow(hInstance, nCmdShow, 1024, 768);
	initD3D();
	initResources();
	hrTimer.Initialize();
	windowLoop();
    cleanD3D();

    return msg.wParam;
}