/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

import java.awt.Color;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.util.glu.GLU.*;

import java.io.IOException;
import java.util.logging.FileHandler;
import java.util.logging.Level;
import java.util.logging.Logger;

import java.util.Vector;
import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.RandomAccessFile;
import java.io.FileNotFoundException;
import javax.imageio.ImageIO;

import org.lwjgl.LWJGLException;
import org.lwjgl.input.Keyboard;
import org.lwjgl.input.Mouse;
import org.lwjgl.opengl.Display;
import org.lwjgl.opengl.DisplayMode;
import org.lwjgl.opengl.GL11;

public class FrostFE
{
	// Window variables.
	public static int WindowWidth = 240;
	public static int WindowHeight = 150;
	
	// Input variables.
	public static int MouseX = 0;
	public static int MouseY = 0;
	
	// Tile engine-related variables.
	public static int TileSize = 15;
	public static String TileTypeNames[] = { "basicoutdoor", "buildings", "castleinterior", "dragongate", "mountains", "snow", "water" };
	public static FightableEntity CharBeingSelected = null;
	public static FightableEntity CharBeingHoveredOver = null;
	public static boolean SetHoveredOverCharThisFrame;
	public static int SelectedCharOptionMenuItem;
	public static boolean ChoosingCharMenuOption;
	public static boolean MovingSelectedCharacter;
	public static boolean AttackingWithSelectedCharacter;
	public static Texture MoveRangeTile = null;
	public static Texture AttackRangeTile = null;
	
	// Battle-related variables.
	public static Texture BattleFloorLeftTexture = null;
	public static Texture BattleFloorRightTexture = null;
	public static Texture BattleBannerRedTexture = null;
	public static Texture BattleBannerBlueTexture = null;
	public static Texture BattleBottomTexture = null;
	public static Texture BattleHitDmgCritTexture = null;
	
	public static BattleInformation CurrentBattleInformation = null;
	
	// Map variables.
	public static String Map[][] = null;
	public static String MapName = null;
	public static int MapWidth = 0;
	public static int MapHeight = 0;
	public static Point SpawnPos = null;
	
	// All tile type textures.
	public static Vector<Texture> BasicOutdoorTiles = new Vector();
	public static Vector<Texture> BuildingsTiles = new Vector();
	public static Vector<Texture> CastleInteriorTiles = new Vector();
	public static Vector<Texture> DragonGateTiles = new Vector();
	public static Vector<Texture> MountainsTiles = new Vector();
	public static Vector<Texture> SnowTiles = new Vector();
	public static Vector<Texture> WaterTiles = new Vector();
	
	public static Vector<Item> ItemBank = new Vector();
	public static Vector<CoreCharData> CharInfoBank = new Vector();
	public static Vector<SpellData> SpellBank = new Vector();
	
	public static Vector<FightableEntity> AlliedUnits = new Vector();
	public static Vector<FightableEntity> EnemyUnits = new Vector();
	
	public static int NumHelpScreens;
	public static int CurrentHelpScreen;
	public static Texture HelpBG;

	/**
	 * The entry point of the application; do some general initialization and loading, then start the game loop.
	 * Afterwards, cleanup accordingly.
	 * 
	 * @param args the command line arguments
	 */
	public static void main(String[] args)
	{
		// TODO code application logic here
		InitializeGame();
		InitializeTiles();
		InitializeItems();
		InitializeSpells();
		InitializeCharacterAnimations();
		LoadCharacters();
		Map = LoadMap("Res/Maps/map1.csv");
		
		MoveRangeTile = Texture.Load("Res/MoveRange.png");
		AttackRangeTile = Texture.Load("Res/AttackRange.png");
		BattleFloorLeftTexture = Texture.Load("Res/BattleFloorLeft.png");
		BattleFloorRightTexture = Texture.Load("Res/BattleFloorRight.png");
		BattleBannerRedTexture = Texture.Load("Res/BattleBannerRed.png");
		BattleBannerBlueTexture = Texture.Load("Res/BattleBannerBlue.png");
		BattleBottomTexture = Texture.Load("Res/BattleBottom.png");
		BattleHitDmgCritTexture = Texture.Load("Res/HitDmgCrit.png");
		
		SelectedCharOptionMenuItem = 0;
		ChoosingCharMenuOption = false;
		MovingSelectedCharacter = false;
		AttackingWithSelectedCharacter = false;
		SetHoveredOverCharThisFrame = false;
		
		NumHelpScreens = 3;
		CurrentHelpScreen = 0;
		HelpBG = Texture.Load("Res/HelpBG.png");
		
		TileCursor.Initialize();
		Timer.Initialize();
		Input.Initialize();
		TextEngine.Initialize();
		Application.Initialize();
		Application.SetGameState(Application.GAMESTATE_INTRO);
		GameLoop();
		CleanupResources();
	}
	
	// Function to acquire a tile name index (usually for use with GetTileCategoryFromIndex) given the name string.
	public static int GetTileCategoryIndexFromName(String CategoryName)
	{
		for (int i = 0; i < TileTypeNames.length; i++)
		{
			if (TileTypeNames[i].equalsIgnoreCase(CategoryName))
			{
				return i;
			}
		}
		return 9001; // This should not happen...
	}
	
	// Function to return the appropriate Texture Vector for the type of desired tile, based on Tile name index.
	public static Vector<Texture> GetTileCategoryFromIndex(int Index)
	{
		switch (Index)
		{
			case 0: return BasicOutdoorTiles;
			case 1: return BuildingsTiles;
			case 2: return CastleInteriorTiles;
			case 3: return DragonGateTiles;
			case 4: return MountainsTiles;
			case 5: return SnowTiles;
			case 6: return WaterTiles;
		}
		return null; // This should not happen...
	}
	
	// Function to get core character information by ID.
	public static CoreCharData GetCharDataByID(int ID)
	{
		for (int i = 0; i < CharInfoBank.size(); i++)
		{
			if (CharInfoBank.get(i).ID == ID)
			{
				return CharInfoBank.get(i);
			}
		}
		return null;
	}
	
	// Function to get an item by it's ID.
	public static Item GetItemByID(int ID)
	{
		for (int i = 0; i < ItemBank.size(); i++)
		{
			if (ItemBank.get(i).ID == ID)
			{
				return ItemBank.get(i);
			}
		}
		return null;
	}
	
	// Function to get a spell by it's ID.
	public static SpellData GetSpellByID(int ID)
	{
		for (int i = 0; i < SpellBank.size(); i++)
		{
			if (SpellBank.get(i).ID == ID)
			{
				return SpellBank.get(i);
			}
		}
		return null;
	}
	
	// Function to get the unit at a location (if one exists there).
	public static FightableEntity GetUnitAtPos(Point Pos)
	{
		for (int i = 0; i < Math.max(AlliedUnits.size(), EnemyUnits.size()); i++)
		{
			if (i < AlliedUnits.size())
			{
				FightableEntity ally = AlliedUnits.get(i);
				if (ally.Position.equals(Pos))
				{
					return ally;
				}
			}
			
			if (i < EnemyUnits.size())
			{
				FightableEntity enemy = EnemyUnits.get(i);
				if (enemy.Position.equals(Pos))
				{
					return enemy;
				}
			}
		}
		
		return null;
	}
	
	// A function that accepts a relative tile coordinate and returns the absolute coordinate of that tile.
	public static Point TileRelToAbsCoord(Point RelCoord)
	{
		Point ret = new Point();
		ret.x = (RelCoord.x * TileSize);
		ret.y = (RelCoord.y * TileSize);
		return ret;
	}
	
	// A basic function that actually returns a random value within the desired range.
	public static double GetRandom(double Min, double Max)
	{
		return (Math.random() * (Max - Min)) + Min;
	}

	// A general initialization of vital resources to the game, mainly JLWGL elements, as well as OpenGL initialization.
	public static void InitializeGame()
	{
		try
		{
			//Display
			Display.setDisplayMode(new DisplayMode(WindowWidth, WindowHeight));
			Display.setFullscreen(false);
			Display.setTitle("Frost Fire Emblem");
			Display.create();

			//Keyboard
			Keyboard.create();

			//Mouse
			Mouse.setGrabbed(false);
			Mouse.create();
		}
		catch (LWJGLException e)
		{
			System.out.println(e.toString());
		}
		
		// Initialize OpenGL with 2D drawing settings.
		glClearColor(0.0f,0.0f,0.0f,0.0f);
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, WindowWidth, WindowHeight, 0, 0, 1);
		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	// The function for initializing all the possible tile types.
	public static void InitializeTiles()
	{
		// Fill in the 0th index item in the vector with a null reference to a Texture class,
		// because the image file name numbers start at 1.
		for (int i = 0; i < TileTypeNames.length; i++)
		{
			GetTileCategoryFromIndex(i).add(null);
		}
		
		// Start loading every single tile for every tile type.
		boolean StillReadingFiles = true;
		int i = 0;
		int CurrentTileType = 0;
		while (StillReadingFiles)
		{
			i++;
			String imgpath = "res/tiles/" + TileTypeNames[CurrentTileType] + "_" + i + ".png";
			if (!new File(imgpath).exists())
			{
				if (CurrentTileType == TileTypeNames.length - 1)
				{
					StillReadingFiles = false;
				}
				else
				{
					System.out.println("Loaded " + (i - 1) + " " + TileTypeNames[CurrentTileType] + " texture tiles.");
					CurrentTileType++;
					i = 0;
				}
			}
			else
			{
				GetTileCategoryFromIndex(CurrentTileType).add(Texture.Load(imgpath));
				//System.out.println("Loaded \"" + imgpath + "\".");
			}
		}
	}
	
	// A function to initialize all the items available from the item bank configuration file.
	public static void InitializeItems()
	{
		RandomAccessFile ItemBankConfigFile = null;
		
		try
		{
			ItemBankConfigFile = new RandomAccessFile("Res/ItemList.csv", "r");
			
			while (ItemBankConfigFile.getFilePointer() < ItemBankConfigFile.length())
			{
				String CurrentItem[] = ItemBankConfigFile.readLine().split(",");

				Item TheNewItem = new Item();
				// Read each item stored in the format ID:MaxDurability.
				// This is a VERY basic format, but good enough for this purpose.
				TheNewItem.ID = Integer.parseInt(CurrentItem[0]);
				TheNewItem.MaxDurability = Integer.parseInt(CurrentItem[1]);
				TheNewItem.Name = CurrentItem[2];
				TheNewItem.Icon = Texture.Load("Res/Items/" + CurrentItem[0] + ".png");
				TheNewItem.MinAtkRange = Integer.parseInt(CurrentItem[3]);
				TheNewItem.MaxAtkRange = Integer.parseInt(CurrentItem[4]);

				// Add the loaded item information into the item bank.
				ItemBank.add(TheNewItem);
			}
			
			ItemBankConfigFile.close();
		}
		catch (IOException e)
		{
			System.out.println("Could not find the item list configuration file! (Or had an error reading it)");
		}
		
		System.out.println("Loaded " + ItemBank.size() + " item(s) into the item bank.");
	}
	
	// A function to load spell animations.
	public static void InitializeSpells()
	{
		RandomAccessFile SpellBankConfigFile = null;
		
		try
		{
			SpellBankConfigFile = new RandomAccessFile("Res/SpellList.csv", "r");
			
			while (SpellBankConfigFile.getFilePointer() < SpellBankConfigFile.length())
			{
				String CurrentSpell = SpellBankConfigFile.readLine();

				SpellData data = new SpellData();
				
				data.ID = Integer.parseInt(CurrentSpell);
				
				String CurrentSpellName = GetItemByID(data.ID).Name;
				
				int i = 1;
				
				while (new File("Res/Spells/" + CurrentSpellName + "/" + i + ".png").exists())
				{
					data.AnimationFrames.add(Texture.Load("Res/Spells/" + CurrentSpellName + "/" + i + ".png"));
					i++;
				}
				
				System.out.println("Loaded " + data.AnimationFrames.size() + " frames for \"" + CurrentSpellName + "\".");

				// Add the loaded spell information into the spell bank.
				SpellBank.add(data);
			}
			
			SpellBankConfigFile.close();
		}
		catch (IOException e)
		{
			System.out.println("Could not find the spell list configuration file! (Or had an error reading it)");
		}
	}
	
	// A function to load all the character animation frames for each type of animation.
	public static void InitializeCharacterAnimations()
	{
		RandomAccessFile CharAnimConfigFile = null;
		
		try
		{
			CharAnimConfigFile = new RandomAccessFile("Res/CharAnimList.csv", "r");
			
			String CurrentInfo[] = CharAnimConfigFile.readLine().split(",");
			
			CoreCharData TheCharData = new CoreCharData();
			TheCharData.Name = CurrentInfo[0];
			TheCharData.ID = Integer.parseInt(CurrentInfo[1]);
			
			String AnimImgFolder = "Res/Characters/" + TheCharData.Name + "/";
			
			// Load standing frames.
			// TODO Change this to load different types based on knowledge of types.
			boolean StillReadingCurrent = true;
			int i = 1;
			while (StillReadingCurrent)
			{
				String AnimPath = AnimImgFolder + "Stand_" + i + ".png";
				if (!new File(AnimPath).exists())
				{
					StillReadingCurrent = false;
				}
				else
				{
					TheCharData.Standing.add(Texture.Load(AnimPath));
					i++;
				}
			}
			
			i = 1;
			StillReadingCurrent = true;
			while (StillReadingCurrent)
			{
				String AnimPath = AnimImgFolder + "Selected_" + i + ".png";
				if (!new File(AnimPath).exists())
				{
					StillReadingCurrent = false;
				}
				else
				{
					TheCharData.Selected.add(Texture.Load(AnimPath));
					i++;
				}
			}
			
			// TODO Obviously in the real fire emblem it is a lot more than 1 frame...
			TheCharData.InBattle = Texture.Load(AnimImgFolder + "InBattle.png");
			
			// Add the loaded item information into the item bank.
			CharInfoBank.add(TheCharData);
			
			CharAnimConfigFile.close();
		}
		catch (IOException e)
		{
			System.out.println("Could not find the character animation list configuration file! (Or had an error reading it)");
		}
		
		System.out.println("Loaded " + CharInfoBank.size() + " char(s) into the char data bank.");
	}
	
	// A function to load all the characters based on the character information configuration file.
	public static void LoadCharacters()
	{
		RandomAccessFile CharsConfigFile = null;
		
		try
		{
			CharsConfigFile = new RandomAccessFile("Res/CharList.csv", "r");
			
			while (CharsConfigFile.getFilePointer() < CharsConfigFile.length())
			{
				String CurrentItem[] = CharsConfigFile.readLine().split(",");

				FightableEntity TheChar = new FightableEntity();

				TheChar.CharData = GetCharDataByID(Integer.parseInt(CurrentItem[0]));
				TheChar.Position.x = Integer.parseInt(CurrentItem[1]);
				TheChar.Position.y = Integer.parseInt(CurrentItem[2]);
				TheChar.Team = Integer.parseInt(CurrentItem[16]);
				
				TheChar.HP = Integer.parseInt(CurrentItem[3]);
				TheChar.MaxHP = Integer.parseInt(CurrentItem[4]);
				TheChar.Experience = Integer.parseInt(CurrentItem[5]);
				TheChar.Strength = Integer.parseInt(CurrentItem[6]);
				TheChar.Skill = Integer.parseInt(CurrentItem[7]);
				TheChar.Speed = Integer.parseInt(CurrentItem[8]);
				TheChar.Luck = Integer.parseInt(CurrentItem[9]);
				TheChar.Defense = Integer.parseInt(CurrentItem[10]);
				TheChar.Resistance = Integer.parseInt(CurrentItem[11]);
				TheChar.Move = Integer.parseInt(CurrentItem[12]);
				TheChar.Constitution = Integer.parseInt(CurrentItem[13]);
				TheChar.Condition = Integer.parseInt(CurrentItem[14]);

				// Add the loaded item information into the character bank.
				int UnitType = Integer.parseInt(CurrentItem[15]);
				
				if (UnitType == 1)
				{
					AlliedUnits.add(TheChar);
				}
				else if (UnitType == 2)
				{
					EnemyUnits.add(TheChar);
				}
				
				// Load all the character's equipped items.
				if (CurrentItem.length == 18)
				{
					String RawItemList[] = CurrentItem[17].split("\\|");
					
					for (int CurItem = 0; CurItem < RawItemList.length; CurItem++)
					{
						String CurrentItemStats[] = RawItemList[CurItem].split(":");
						
						InventoryItem NewItem = new InventoryItem();
						
						NewItem.ItemCore = GetItemByID(Integer.parseInt(CurrentItemStats[0]));
						NewItem.CurrentDurability = Integer.parseInt(CurrentItemStats[1]);
						NewItem.Equipped = Boolean.parseBoolean(CurrentItemStats[2]);
						
						TheChar.Items[CurItem] = NewItem;
						System.out.println("Added 1 item to a character, Item ID: " + NewItem.ItemCore.ID + " | CD: " + NewItem.CurrentDurability + " | EQ: " + NewItem.Equipped);
					}
				}
			}
			
			CharsConfigFile.close();
		}
		catch (IOException e)
		{
			System.out.println("Could not find the item list configuration file! (Or had an error reading it)");
		}
		
		System.out.println("Loaded " + (AlliedUnits.size() + EnemyUnits.size()) + " unit(s).");
	}
	
	// The game processing loop (equivalent to the window procedure in some ways).
	public static void GameLoop()
	{
		while(!Display.isCloseRequested() && !Keyboard.isKeyDown(Keyboard.KEY_ESCAPE))
		{
			if(Display.isVisible())
			{
				DrawScene();
			}
			else
			{
				if(Display.isDirty())
				{
					DrawScene();
				}
				Application.Sleep(100);
			}
			Display.update();
			//Display.sync(60);
		}
	}

	// Clean up all used resources as the application is closing.
	public static void CleanupResources()
	{
		// Methods already check if created before destroying.
		Mouse.destroy();
		Keyboard.destroy();
		Display.destroy();
	}
	
	// Draw all the content that is necessary for the current frame.
	public static void DrawScene()
	{
		Timer.Update();
		Application.RunTime += Timer.GetElapsedMilliseconds();
		Application.CurrentGameStateTime += Timer.GetElapsedMilliseconds();
		Input.Update();
		ProcessInput();
		
		glClear(GL_COLOR_BUFFER_BIT);
		
		switch (Application.GameState)
		{
			case Application.GAMESTATE_INTRO:
				DrawIntro();
				break;
				
			case Application.GAMESTATE_MAINMENU:
				DrawMainMenu();
				break;
				
			case Application.GAMESTATE_MAP:
				DrawMap();
				break;
				
			case Application.GAMESTATE_BATTLE:
				DrawBattle();
				break;
				
			case Application.GAMESTATE_STATS:
				DrawCharStats();
				break;
				
			case Application.GAMESTATE_GAMEOVER:
				DrawGameOver();
				break;
				
			case Application.GAMESTATE_HELP:
				DrawHelp();
				break;
		}
	}
	
	// Process all input that was polled this frame.
	public static void ProcessInput()
	{
		// Update the mouse location.
		MouseX = Mouse.getX();
		MouseY = Mouse.getY();
		
		// Process keyboard input for each state appropriately.
		
		// Input processing for the Main Menu state.
		if (Application.GameState == Application.GAMESTATE_MAINMENU)
		{
			if (Input.GetKeyByKeyCode(Keyboard.KEY_UP).WasPressed())
			{
				if (SelectedCharOptionMenuItem > 0)
				{
					SelectedCharOptionMenuItem--;
				}
			}
			
			if (Input.GetKeyByKeyCode(Keyboard.KEY_DOWN).WasPressed())
			{
				if (SelectedCharOptionMenuItem < 2)
				{
					SelectedCharOptionMenuItem++;
				}
			}
			
			if (Input.GetKeyByKeyCode(Keyboard.KEY_RETURN).WasPressed())
			{
				switch (SelectedCharOptionMenuItem)
				{
					case 0:
						Application.SetGameState(Application.GAMESTATE_MAP);
						break;
						
					case 1:
						Application.SetGameState(Application.GAMESTATE_HELP);
						break;
						
					case 2:
						System.exit(0);
						break;
				}
			}
		}
		
		else if (Application.GameState == Application.GAMESTATE_HELP)
		{
			if (Input.GetKeyByKeyCode(Keyboard.KEY_SPACE).WasPressed())
			{
				CurrentHelpScreen++;
				if (CurrentHelpScreen >= NumHelpScreens)
				{
					CurrentHelpScreen = 0;
					Application.SetGameState(Application.GAMESTATE_MAINMENU);
				}
			}
		}
		
		// Input processing for the Map state.
		else if (Application.GameState == Application.GAMESTATE_MAP)
		{
			if (Input.GetKeyByKeyCode(Keyboard.KEY_UP).WasPressed())
			{
				if (ChoosingCharMenuOption)
				{
					if (SelectedCharOptionMenuItem > 0)
					{
						SelectedCharOptionMenuItem--;
					}
				}

				if (!ChoosingCharMenuOption)
				{
					if (TileCursor.Location.y - 1 >= 0)
					{
						TileCursor.Location.y -= 1;
					}
				}
			}

			if (Input.GetKeyByKeyCode(Keyboard.KEY_DOWN).WasPressed())
			{
				if (ChoosingCharMenuOption)
				{
					// Limit the maximum movement to the maximum number of menu items.
					if (FrostFE.SelectedCharOptionMenuItem < CharBeingSelected.GetValidMenuOptions().size() - 1)
					{
						SelectedCharOptionMenuItem++;
					}
				}

				if (!ChoosingCharMenuOption)
				{
					if (TileCursor.Location.y + 1 <= MapHeight - 1)
					{
						TileCursor.Location.y += 1;
					}
				}
			}

			if (Input.GetKeyByKeyCode(Keyboard.KEY_LEFT).WasPressed())
			{
				if (!ChoosingCharMenuOption)
				{
					if (TileCursor.Location.x - 1 >= 0)
					{
						TileCursor.Location.x -= 1;
					}
				}
			}

			if (Input.GetKeyByKeyCode(Keyboard.KEY_RIGHT).WasPressed())
			{
				if (!ChoosingCharMenuOption)
				{
					if (TileCursor.Location.x + 1 <= MapWidth - 1)
					{
						TileCursor.Location.x += 1;
					}
				}
			}

			if (Input.GetKeyByKeyCode(Keyboard.KEY_Z).WasPressed())
			{
				boolean SelectedACharThisFrame = false;

				if (CharBeingSelected == null)
				{
					FightableEntity TargetChar = FightableEntity.FindAtPos(TileCursor.Location);
					if (TargetChar != null && !TargetChar.ActedThisTurn)
					{
						CharBeingSelected = TargetChar;
						ChoosingCharMenuOption = true;
						SelectedACharThisFrame = true;
						System.out.println("Selected a character. (Move: " + CharBeingSelected.Move + ")");
					}
				}

				// Handle menu item selection.
				if (CharBeingSelected != null && !SelectedACharThisFrame)
				{
					if (ChoosingCharMenuOption)
					{
						// Select the option.
						// TODO Fix menu option selection handling.
						Vector<String> PossibleOptions = CharBeingSelected.GetValidMenuOptions();
						String SelectedOption = PossibleOptions.get(SelectedCharOptionMenuItem);

						if (SelectedOption.equals("Move"))
						{
							ChoosingCharMenuOption = false;
							MovingSelectedCharacter = true;
							SelectedCharOptionMenuItem = 0;
						}
						else if (SelectedOption.equals("Attack"))
						{
							ChoosingCharMenuOption = false;
							MovingSelectedCharacter = false;
							AttackingWithSelectedCharacter = true;
							SelectedCharOptionMenuItem = 0;
						}
						else if (SelectedOption.equals("Wait"))
						{
							CharBeingSelected.ActedThisTurn = true;
							MovingSelectedCharacter = false;
							AttackingWithSelectedCharacter = false;
							ChoosingCharMenuOption = false;
							CharBeingSelected = null;
							SelectedCharOptionMenuItem = 0;
						}
					}
					else if (MovingSelectedCharacter && TileCursor.Location.equals(CharBeingSelected.Position))
					{
						MovingSelectedCharacter = false;
						ChoosingCharMenuOption = true;
					}
					else if (MovingSelectedCharacter)
					{
						if (CharBeingSelected.CanMoveToTileAtPos(TileCursor.Location))
						{
							// TODO Maybe in the future add the ability to undo movements.
							CharBeingSelected.MoveToTile(TileCursor.Location);
							CharBeingSelected.MovedThisTurn = true;
							ChoosingCharMenuOption = true;
						}
					}
					else if (AttackingWithSelectedCharacter)
					{
						// The general attacking procedure to select initiating a fight with an enemy.
						// Might have to be slightly reworked as a function call if it is desired to easily allow AI to attack.
						FightableEntity target = GetUnitAtPos(TileCursor.Location);

						if (target != null)
						{
							if (target.Team == FightableEntity.TEAM_ENEMY)
							{
								// Set battle parameters.
								CurrentBattleInformation = new BattleInformation();
								CurrentBattleInformation.AllyInFight = CharBeingSelected;
								CurrentBattleInformation.EnemyInFight = target;
								CurrentBattleInformation.BattleInitiator = CurrentBattleInformation.AllyInFight;
								CurrentBattleInformation.MakeCalculations();

								// Reset some menu variables.
								ChoosingCharMenuOption = false;
								MovingSelectedCharacter = false;
								AttackingWithSelectedCharacter = false;
								CharBeingSelected.ActedThisTurn = true;
								CharBeingSelected.MovedThisTurn = true;
								CharBeingSelected = null;

								// Switch to the battle state.
								Application.SetGameState(Application.GAMESTATE_BATTLE);
							}
						}
					}
				}
			}

			if (Input.GetKeyByKeyCode(Keyboard.KEY_X).WasPressed())
			{
				if (MovingSelectedCharacter)
				{
					MovingSelectedCharacter = false;
					ChoosingCharMenuOption = true;
					SelectedCharOptionMenuItem = 0;
				}
				else if (AttackingWithSelectedCharacter)
				{
					AttackingWithSelectedCharacter = false;
					ChoosingCharMenuOption = true;
					SelectedCharOptionMenuItem = 0;
				}
				else if (ChoosingCharMenuOption)
				{
					ChoosingCharMenuOption = false;
					CharBeingSelected = null;
					SelectedCharOptionMenuItem = 0;
				}
			}

			// TODO change this, it temporarily just rotates game states.
			if (Input.GetKeyByKeyCode(Keyboard.KEY_D).WasPressed())
			{
				if (Application.GameState == Application.GAMESTATE_MAP)
				{
					Application.SetGameState(Application.GAMESTATE_BATTLE);
				}
				else if (Application.GameState == Application.GAMESTATE_BATTLE)
				{
					Application.SetGameState(Application.GAMESTATE_STATS);
				}
				else
				{
					Application.SetGameState(Application.GAMESTATE_MAP);
				}
			}

			// TODO Just testing turn switching...
			if (Input.GetKeyByKeyCode(Keyboard.KEY_F).WasPressed())
			{
				if (Application.CurrentTeamTurn == Application.TEAM_PLAYER)
				{
					Application.SwitchTurn(Application.TEAM_ENEMY);
				}
				else
				{
					Application.SwitchTurn(Application.TEAM_PLAYER);
				}
			}
		}
	}
	
	// The introduction drawing.. basically the fake loading bar.
	public static void DrawIntro()
	{
		if (Application.CurrentGameStateTime < 2500)
		{
			TextEngine.Draw(45, 20, "Loading Frost Fire Emblem", null);
			DrawQuad(20, 70, 200, 40, Color.gray, null);
			DrawQuad(20, 70, (int)(((float)Math.min(Application.CurrentGameStateTime, 2000) / 2000.0f) * 200), 40, Color.green, null);
			DrawQuad(20, 70, 200, 20, new Color(255, 255, 255, 100), null);
		}
		else 
		{
			Application.SetGameState(Application.GAMESTATE_MAINMENU);
		}
	}
	
	// The main menu drawing. The main menu is basically just to select play or help.
	public static void DrawMainMenu()
	{
		DrawQuad(0, 45 + (SelectedCharOptionMenuItem * 13), WindowWidth, 13, new Color(255, 255, 255, 100), null);
		
		// TODO Fill in the main menu.
		String MainMenuString = "Main Menu\n\n";
		
		MainMenuString += (SelectedCharOptionMenuItem == 0 ? " --- Play --- " : "     Play") + "\n";
		MainMenuString += (SelectedCharOptionMenuItem == 1 ? " --- Instructions --- " : "     Instructions") + "\n";
		MainMenuString += (SelectedCharOptionMenuItem == 2 ? " --- Quit --- " : "     Quit") + "\n";
		
		TextEngine.Draw(20, 20, MainMenuString, null);
	}
	
	// Map drawing functionality, handles things like drawing map tiles and character animations.
	public static void DrawMap()
	{
		// Draw the whole map's tiles.
		for (int VertTile = 0; VertTile < MapWidth; VertTile++)
		{
			for (int HorzTile = 0; HorzTile < MapHeight; HorzTile++)
			{
				DrawMapTileFromString(new Point(VertTile, HorzTile), Map[VertTile][HorzTile]);
			}
		}
		
		// Draw all units on the map.
		for (int CurUnit = 0; CurUnit < Math.max(AlliedUnits.size(), EnemyUnits.size()); CurUnit++)
		{
			if (CurUnit < AlliedUnits.size())
			{
				FightableEntity Ally = AlliedUnits.get(CurUnit);
				Ally.UpdateAnimationState();
				Ally.Draw();
			}
			
			if (CurUnit < EnemyUnits.size())
			{
				FightableEntity Enemy = EnemyUnits.get(CurUnit);
				Enemy.UpdateAnimationState();
				Enemy.Draw();
			}
		}
		
		// Deal with if a character is being selected.
		if (CharBeingSelected != null)
		{
			if (!CharBeingSelected.MovedThisTurn && MovingSelectedCharacter)
			{
				// Detect and draw all movable tiles.
				for (int MoveDistX = 0; MoveDistX < MapWidth; MoveDistX++)
				{
					for (int MoveDistY = 0; MoveDistY < MapHeight; MoveDistY++)
					{
						Point TheTilePos = new Point(MoveDistX, MoveDistY);
						if (CharBeingSelected.CanMoveToTileAtPos(TheTilePos))
						{
							// TODO Load the actual movement/attack textures.
							DrawTile(MoveRangeTile, TheTilePos);
						}
					}
				}
			}
			
			if (AttackingWithSelectedCharacter)
			{
				// Detect and draw all movable tiles.
				for (int MoveDistX = 0; MoveDistX < MapWidth; MoveDistX++)
				{
					for (int MoveDistY = 0; MoveDistY < MapHeight; MoveDistY++)
					{
						Point TheTilePos = new Point(MoveDistX, MoveDistY);
						if (CharBeingSelected.CanAttackTileAtPos(TheTilePos))
						{
							DrawTile(AttackRangeTile, TheTilePos);
						}
					}
				}
			}
			
			// Draw the character options menu.
			if (ChoosingCharMenuOption)
			{
				Vector<String> PossibleOptions = CharBeingSelected.GetValidMenuOptions();
				String DrawnOptionsString = "";
				
				for (int i = 0; i < PossibleOptions.size(); i++)
				{
					DrawnOptionsString += PossibleOptions.get(i);
					if (i < PossibleOptions.size() - 1)
					{
						DrawnOptionsString += '\n';
					}
				}
				
				DrawQuad(180, 30, 60, 60, new Color(0, 0, 0, 100), null);
				DrawQuad(182, 32, 56, 56, new Color(0, 0, 0, 100), null);
				DrawQuad(185, 32 + (SelectedCharOptionMenuItem * 13) + 5, 3, 3, Color.white, null);
				TextEngine.Draw(193, 33, DrawnOptionsString, null);
			}
		}
		
		// Draw the tile cursor.
		DrawTile(TileCursor.Tex, new Point(TileCursor.Location.x, TileCursor.Location.y));
		
		// Update the team switching state.
		Application.UpdateTeamSwitchingState();
		// Reset the hovered over character this frame detector variable.
		SetHoveredOverCharThisFrame = false;
	}
	
	// Drawing a battle scene, this handles the general UI of the fight, the background, and character animations,
	// as well as other related functionality.
	public static void DrawBattle()
	{
		// Check if the necessary calculations have been made and if not, do not draw the battle.
		if (CurrentBattleInformation == null)
		{
			TextEngine.Draw(50, 75, "Invalid battle parameters.", null);
			return;
		}
		
		// Use the map as a background for the battle.
		DrawMap();
		// Fade it darker somewhat as a nice effect.
		DrawQuad(0, 0, WindowWidth, WindowHeight, new Color(0, 0, 0, 100), null);
		
		// Draw some general battle scenery.
		DrawQuad(0, 0, 64, 29, null, BattleBannerRedTexture);
		DrawQuad(240-64, 0, 64, 29, null, BattleBannerBlueTexture);
		DrawQuad(33, 79, 87, 40, null, BattleFloorLeftTexture);
		DrawQuad(33+87, 79, 87, 40, null, BattleFloorRightTexture);
		
		// Draw the general bottom stat area.
		DrawQuad(0, 99, 240, 51, null, BattleBottomTexture);
		DrawQuad(1, 103, 16, 24, null, BattleHitDmgCritTexture);
		DrawQuad(199, 103, 16, 24, null, BattleHitDmgCritTexture);
		
		// Draw the unit names.
		TextEngine.Draw(6, 10, CurrentBattleInformation.EnemyInFight.CharData.Name, null);
		TextEngine.Draw(240-50, 10, CurrentBattleInformation.AllyInFight.CharData.Name, null);
		
		// Draw the unit.
		TextEngine.Draw(20, 135, CurrentBattleInformation.EnemyInFight.HP + " / " + CurrentBattleInformation.EnemyInFight.MaxHP, null);
		TextEngine.Draw(150, 135, CurrentBattleInformation.AllyInFight.HP + " / " + CurrentBattleInformation.AllyInFight.MaxHP, null);
		
		Texture EnemyBattleAnimation = CurrentBattleInformation.EnemyInFight.CharData.InBattle;
		DrawQuad(70, 60, EnemyBattleAnimation.ImageWidth, EnemyBattleAnimation.ImageHeight, null, EnemyBattleAnimation);
		Texture AllyBattleAnimation = CurrentBattleInformation.AllyInFight.CharData.InBattle;
		DrawQuad(140, 60, AllyBattleAnimation.ImageWidth, AllyBattleAnimation.ImageHeight, null, AllyBattleAnimation);
		
		if (CurrentBattleInformation.EnemyInFight.GetEquippedItem() != null)
		{
			TextEngine.Draw(20, 100, Integer.toString(CurrentBattleInformation.Attacker == CurrentBattleInformation.AllyInFight ? CurrentBattleInformation.DefenderHitRate : CurrentBattleInformation.AttackerHitRate), null);
			TextEngine.Draw(20, 110, Integer.toString(CurrentBattleInformation.Attacker == CurrentBattleInformation.AllyInFight ? CurrentBattleInformation.DefenderDamage : CurrentBattleInformation.AttackerDamage), null);
			TextEngine.Draw(20, 120, Integer.toString(CurrentBattleInformation.Attacker == CurrentBattleInformation.AllyInFight ? CurrentBattleInformation.DefenderCriticalRate : CurrentBattleInformation.AttackerCriticalRate), null);
			
			DrawQuad(45, 113, 16, 16, null, CurrentBattleInformation.EnemyInFight.GetEquippedItem().ItemCore.Icon);
			TextEngine.Draw(63, 115, CurrentBattleInformation.EnemyInFight.GetEquippedItem().ItemCore.Name, null);
		}
		if (CurrentBattleInformation.AllyInFight.GetEquippedItem() != null)
		{
			TextEngine.Draw(217, 100, Integer.toString(CurrentBattleInformation.Defender == CurrentBattleInformation.AllyInFight ? CurrentBattleInformation.DefenderHitRate : CurrentBattleInformation.AttackerHitRate), null);
			TextEngine.Draw(217, 110, Integer.toString(CurrentBattleInformation.Defender == CurrentBattleInformation.AllyInFight ? CurrentBattleInformation.DefenderDamage : CurrentBattleInformation.AttackerDamage), null);
			TextEngine.Draw(217, 120, Integer.toString(CurrentBattleInformation.Defender == CurrentBattleInformation.AllyInFight ? CurrentBattleInformation.DefenderCriticalRate : CurrentBattleInformation.AttackerCriticalRate), null);
			
			DrawQuad(122, 113, 16, 16, null, CurrentBattleInformation.AllyInFight.GetEquippedItem().ItemCore.Icon);
			TextEngine.Draw(140, 115, CurrentBattleInformation.AllyInFight.GetEquippedItem().ItemCore.Name, null);
		}
		
		// If there is an attack switch delay, just increment the internal counter and return immediately after.
		if (CurrentBattleInformation.AttackSwitchDelayActive)
		{
			if (CurrentBattleInformation.AttackSwitchDelayTime >= 1000)
			{
				if (CurrentBattleInformation.CurrentAttacker == CurrentBattleInformation.Attacker && CurrentBattleInformation.Defender.GetEquippedItem() != null && CurrentBattleInformation.DefenderNumLaunchedAttacks < CurrentBattleInformation.DefenderNumberOfAttacks && !CurrentBattleInformation.Defender.IsDead())
				{
					CurrentBattleInformation.CurrentAttacker = CurrentBattleInformation.Defender;
				}
				// TODO if weapon durability was actually used in a fight and it actually could expire in the middle of a fight, checks for that should be made appropriately.
				else if (CurrentBattleInformation.CurrentAttacker == CurrentBattleInformation.Defender && CurrentBattleInformation.AttackerNumLaunchedAttacks < CurrentBattleInformation.AttackerNumberOfAttacks && !CurrentBattleInformation.Attacker.IsDead())
				{
					CurrentBattleInformation.CurrentAttacker = CurrentBattleInformation.Attacker;
				}
				
				CurrentBattleInformation.AttackSwitchDelayActive = false;
				CurrentBattleInformation.AttackSwitchDelayTime = 0;
				CurrentBattleInformation.CurrentAttackFrame = 0;
				CurrentBattleInformation.CurrentAttackFrameTime = 0;
			}
			else
			{
				CurrentBattleInformation.AttackSwitchDelayTime += Timer.GetElapsedMilliseconds();
				return;
			}
		}
		
		// Handle attack frame modification.
		CurrentBattleInformation.CurrentAttackFrameTime++;
		if (CurrentBattleInformation.CurrentAttackFrameTime >= 30)
		{
			CurrentBattleInformation.CurrentAttackFrame++;
			CurrentBattleInformation.CurrentAttackFrameTime = 0;
			
			// If this is true, that means the spell has finished casting and the damage should be deducted if the move hit,
			// and attack turns should be switched as necessary.
			if (CurrentBattleInformation.CurrentAttackFrame > GetSpellByID(CurrentBattleInformation.CurrentAttacker.GetEquippedItem().ItemCore.ID).AnimationFrames.size() - 2)
			{
				if (CurrentBattleInformation.CurrentAttacker == CurrentBattleInformation.Attacker)
				{
					if (CurrentBattleInformation.AttackerAttacksHit[CurrentBattleInformation.AttackerNumLaunchedAttacks])
					{
						int DamageTaken = CurrentBattleInformation.AttackerDamage;
						if (CurrentBattleInformation.AttackerAttacksCrit[CurrentBattleInformation.AttackerNumLaunchedAttacks])
						{
							DamageTaken *= 3;
						}
						CurrentBattleInformation.Defender.ModifyHP(-(DamageTaken));
					}
					CurrentBattleInformation.AttackerNumLaunchedAttacks++;
				}
				else if (CurrentBattleInformation.CurrentAttacker == CurrentBattleInformation.Defender)
				{
					if (CurrentBattleInformation.DefenderAttacksHit[CurrentBattleInformation.DefenderNumLaunchedAttacks])
					{
						int DamageTaken = CurrentBattleInformation.DefenderDamage;
						if (CurrentBattleInformation.DefenderAttacksCrit[CurrentBattleInformation.DefenderNumLaunchedAttacks])
						{
							DamageTaken *= 3;
						}
						CurrentBattleInformation.Attacker.ModifyHP(-(DamageTaken));
					}
					CurrentBattleInformation.DefenderNumLaunchedAttacks++;
				}
				CurrentBattleInformation.AttackSwitchDelayActive = true;
			}
		}
		
		// Draw skill usage as appropriate.
		if (!CurrentBattleInformation.AttackSwitchDelayActive)
		{
			Texture CurrentAttackFrame = GetSpellByID(CurrentBattleInformation.CurrentAttacker.GetEquippedItem().ItemCore.ID).AnimationFrames.get(CurrentBattleInformation.CurrentAttackFrame);
			int SpellDrawX = (CurrentBattleInformation.CurrentAttacker == CurrentBattleInformation.AllyInFight ? 20 : 110);
			DrawQuad(SpellDrawX, 0, CurrentAttackFrame.ImageWidth, CurrentAttackFrame.ImageHeight, null, CurrentAttackFrame);
		}
		
		// If the battle ends, change the game state back to the map and discard this set of battle information.
		if (CurrentBattleInformation.BattleIsOver())
		{
			Application.SetGameState(Application.GAMESTATE_MAP);
			CurrentBattleInformation = null;
		}
	}
	
	// Draw the character stats screen. Since this was one of the many things that weren't a requirement,
	// due to time constraints it was not finished and therefore simply not accessible normally in the "final" product.
	public static void DrawCharStats()
	{
		TextEngine.Draw(50, 60, "Coming Soon\n(This screen is not normally accessible)", null);
	}
	
	// Draw the game over screen in the unfortunate event that the player dies.
	public static void DrawGameOver()
	{
		int GameOverTextOpacity = 255;
		
		if (Application.CurrentGameStateTime < 2000)
		{
			GameOverTextOpacity = (int)(((double)Application.CurrentGameStateTime / 2000.0) * 255);
		}
		
		TextEngine.Draw(85, 60, "GAME OVER", new Color(255, 255, 255, GameOverTextOpacity));
	}
	
	// Drawing the help screen, it's a little flip-through kind of album thing almost.
	public static void DrawHelp()
	{
		DrawQuad(0, 0, WindowWidth, WindowHeight, null, HelpBG);
		
		if (CurrentHelpScreen == 0)
		{
			TextEngine.Draw(0, 0, "Welcome to Frost Fire Emblem!\n\nTo control the selection cursor, you use\nthe left/right/up/down arrow keys.\n\nIn order to control units, you must\nhit Z while hovering over them.", null);
		}
		else if (CurrentHelpScreen == 1)
		{
			TextEngine.Draw(0, 0, "Selecting a unit will bring up a menu with\navailable options for that unit that turn.\n\nTo select an action, hit Z. Once a\nunit is moved and waits or attacks,\nthey are done acting for that turn.\n\nTo use them again, end turns using the\nF key.", null);
		}
		else if (CurrentHelpScreen == 2)
		{
			TextEngine.Draw(0, 0, "When moving, mountain tiles are\nunmovable onto.\n\nThere are also flame tiles, which\ndamage you if you end your\nturn standing on them, and\nforts, which heal you if you\nend your turn standing on them.", null);
		}
		
		TextEngine.Draw(WindowWidth - 175, WindowHeight - 20, "Press Space to continue...", null);
	}
	
	// A core function for drawing a quad with optional color modulation and texture binding.
	public static void DrawQuad(int X, int Y, int Width, int Height, Color clr, Texture tx)
	{
		if (tx != null)
		{
			tx.Bind();
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		
		glBegin(GL_QUADS);
		
		if (clr != null)
		{
			glColor4ub((byte)clr.getRed(), (byte)clr.getGreen(), (byte)clr.getBlue(), (byte)clr.getAlpha());
		}
		else
		{
			glColor4ub((byte)255, (byte)255, (byte)255, (byte)255);
		}
		
		glTexCoord2f(0, 0);
		glVertex2f(X, Y);
		glTexCoord2f(1, 0);
		glVertex2f(X + Width, Y);
		glTexCoord2f(1, 1);
		glVertex2f(X + Width, Y + Height);
		glTexCoord2f(0, 1);
		glVertex2f(X, Y + Height);
		
		glEnd();
	}
	
	// Function for the raw drawing of a tile, given strictly the texture instance and the absolute x/y coordinate pair.
	public static void DrawTile(Texture tex, Point Location)
	{
		Point AbsTilePos = TileRelToAbsCoord(Location);
		
		tex.Bind();
		
		glBegin(GL_QUADS);
		glColor4ub((byte)255, (byte)255, (byte)255, (byte)255);
		glTexCoord2f(0, 0);
		glVertex2f(AbsTilePos.x, AbsTilePos.y);
		glTexCoord2f(1, 0);
		glVertex2f(AbsTilePos.x + TileSize, AbsTilePos.y);
		glTexCoord2f(1, 1);
		glVertex2f(AbsTilePos.x + TileSize, AbsTilePos.y + TileSize);
		glTexCoord2f(0, 1);
		glVertex2f(AbsTilePos.x, AbsTilePos.y + TileSize);
		glEnd();
	}
	
	// Function to draw a tile in it's appropriate location throughout the tile drawing iteration in the main drawing loop.
	public static void DrawMapTileFromString(Point Location, String TileString)
	{
		String TileInfo[] = TileString.split(" ");
		DrawTile(GetTileCategoryFromIndex(GetTileCategoryIndexFromName(TileInfo[0])).get(Integer.parseInt(TileInfo[1])), Location);
	}
	
	// Function to load a map's tile content from a CSV.
	public static String[][] LoadMap(String TheMapFileName)
	{
		String LoadedMap[][] = null;
		String LineSplit[] = null;
		
		try
		{
			BufferedReader in = new BufferedReader(new FileReader(TheMapFileName));
			
			// Load some map info.
			String MapInfo[] = in.readLine().split(",");
			MapName = MapInfo[0];
			MapWidth = Integer.parseInt(MapInfo[1]);
			MapHeight = Integer.parseInt(MapInfo[2]);
			SpawnPos = new Point(Integer.parseInt(MapInfo[3]), Integer.parseInt(MapInfo[4]));
			LoadedMap = new String[MapWidth][MapHeight];
			
			// Load each map tile.
			for (int y = 0; y < MapHeight; y++)
			{
				LineSplit = in.readLine().split(",");
				for (int x = 0; x < MapWidth; x++)
				{
					LoadedMap[x][y] = LineSplit[x];
				}
			}
		}
		catch(IOException e)
		{
			System.out.println("Failed to load the map: \"" + TheMapFileName + "\".");
			e.printStackTrace();
		}
		return LoadedMap;
	}
}

// This class describes an item in the item bank. This provides a template for having instances of a certain item.
class Item
{
	public int ID;
	public int MaxDurability;
	public String Name;
	public Texture Icon;
	public int MinAtkRange;
	public int MaxAtkRange;
}

// This class describes an item that is contained in someone's inventory; Basically, it's an actual instance of an item from the bank.
class InventoryItem
{
	public Item ItemCore;
	public int CurrentDurability;
	public boolean Equipped;
	
	public InventoryItem()
	{
		ItemCore = null;
		CurrentDurability = 0;
		Equipped = false;
	}
	
	public static InventoryItem FromItem(Item AnItem)
	{
		InventoryItem ret = new InventoryItem();
		
		ret.ItemCore = AnItem;
		ret.CurrentDurability = ret.ItemCore.MaxDurability;
		
		return ret;
	}
}

// This class holds attributes for every entity that can participate in a battle.
class FightableEntity
{
	public CoreCharData CharData;
	public Point Position;
	public InventoryItem Items[];
	public int Team;
	
	public int HP;
	public int MaxHP;
	public int Experience;
	public int Strength;
	public int Skill;
	public int Speed;
	public int Luck;
	public int Defense;
	public int Resistance;
	public int Move;
	public int Constitution;
	public FightableEntity Carrying;
	public int Condition;
	
	public boolean MovedThisTurn;
	public boolean ActedThisTurn;
	
	public int CurrentAnimationFrameTime;
	public int CurrentAnimationFrame;
	public int CurrentAnimationFrameType;
	
	public static final int ANIMATION_STANDING = 1;
	public static final int ANIMATION_SELECTED = 2;
	
	public static final int TEAM_PLAYER = 1;
	public static final int TEAM_ENEMY = 2;
	
	public FightableEntity()
	{
		Position = new Point(0, 0);
		Items = new InventoryItem[5];
		Team = 0;
		
		HP = 0;
		MaxHP = 0;
		Experience = 0;
		Strength = 0;
		Skill = 0;
		Speed = 0;
		Luck = 0;
		Defense = 0;
		Resistance = 0;
		Move = 0;
		Constitution = 0;
		Carrying = null;
		Condition = 0;
		
		RefreshOnTurnEnd();
		setAnimationType(ANIMATION_STANDING);
	}
	
	public static FightableEntity FindAtPos(Point Pos)
	{
		for (int i = 0; i < FrostFE.AlliedUnits.size(); i++)
		{
			if (FrostFE.AlliedUnits.get(i).Position.equals(Pos))
			{
				return FrostFE.AlliedUnits.get(i);
			}
		}
		return null;
	}
	
	// TODO Account for different tile type movement limitations.
	public boolean CanMoveToTileAtPos(Point Pos)
	{
		// Some map bound checking first.
		if (Pos.x < 0 || Pos.y < 0 || Pos.x > FrostFE.MapWidth - 1 || Pos.y > FrostFE.MapHeight - 1)
		{
			return false;
		}
		
		// Limit the movement range directly based on distance from character, taking into consideration their Move attribute.
		if (Util.Distance(Position, Pos) > Move)
		{
			return false;
		}
		
		// Further limit the movement range to tiles that can actually be moved to by counting tiles necessary to move on.
		if (Util.TileMoveDistance(Position, Pos) > Move)
		{
			return false;
		}
		
		// For simplicity purposes, outright deny movement onto mountain type tiles.
		// TODO In the real fire emblem, units can situationally step on mountains, most with just greatly limited movement.
		if (FrostFE.Map[Pos.x][Pos.y].split(" ")[0].equals("mountains"))
		{
			return false;
		}
		
		// The character cannot move to a tile any other unit is already on.
		for (int i = 0; i < FrostFE.AlliedUnits.size(); i++)
		{
			if (FrostFE.AlliedUnits.get(i).Position.equals(Pos))
			{
				return false;
			}
		}
		
		// The character cannot move to a tile an enemy is on.
		for (int i = 0; i < FrostFE.EnemyUnits.size(); i++)
		{
			if (FrostFE.EnemyUnits.get(i).Position.equals(Pos))
			{
				return false;
			}
		}
		
		// In any other case, just assume the character can move to this block.
		return true;
	}
	
	public boolean CanAttackTileAtPos(Point Pos)
	{
		if (Position.equals(Pos))
		{
			return false;
		}
		
		int AttackTileDistance = Util.TileMoveDistance(Position, Pos);
		
		if (AttackTileDistance >= GetMinAttackRange() && AttackTileDistance <= GetMaxAttackRange())
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	public void MoveToTile(Point TheTile)
	{
		Position.x = TheTile.x;
		Position.y = TheTile.y;
	}
	
	public void RefreshOnTurnEnd()
	{
		MovedThisTurn = false;
		ActedThisTurn = false;
	}
	
	public Vector<String> GetValidMenuOptions()
	{
		Vector<String> ret = new Vector();
		
		// If they have already acted, skip all checks and just return an empty string vector.
		if (!ActedThisTurn)
		{
			if (!MovedThisTurn)
			{
				ret.add("Move");
			}
			
			ret.add("Attack");
			ret.add("Wait");
		}
		
		return ret;
	}
	
	public void setAnimationType(int Type)
	{
		CurrentAnimationFrameType = Type;
		CurrentAnimationFrame = 0;
		CurrentAnimationFrameTime = 0;
	}
	
	public Vector<Texture> getAnimationVectorFromType()
	{
		switch (CurrentAnimationFrameType)
		{
			case ANIMATION_STANDING:
				return CharData.Standing;
			case ANIMATION_SELECTED:
				return CharData.Selected;
		}
		return null;
	}
	
	public Texture getAnimationTexture()
	{
		return getAnimationVectorFromType().get(CurrentAnimationFrame);
	}
	
	public void UpdateAnimationState()
	{
		// Check if the unit is selected and modify some values accordingly.
		boolean CursorOnAlly = TileCursor.Location.equals(Position);
		
		// Update the global variable indicating which character was hovered over this frame.
		if (!FrostFE.SetHoveredOverCharThisFrame)
		{
			FrostFE.CharBeingHoveredOver = this;
			FrostFE.SetHoveredOverCharThisFrame = true;
		}
		
		// TODO Change how animations are updated.
		// Modify the current character's animation state type if necessary.
		if (CurrentAnimationFrameType == ANIMATION_STANDING && CursorOnAlly)
		{
			setAnimationType(ANIMATION_SELECTED);
		}
		else if (CurrentAnimationFrameType == ANIMATION_SELECTED && !CursorOnAlly)
		{
			setAnimationType(ANIMATION_STANDING);
		}

		// Increment and modify the animation counter-related logic.
		CurrentAnimationFrameTime += Timer.GetElapsedMilliseconds();
		if (CurrentAnimationFrameTime > 500)
		{
			CurrentAnimationFrameTime = 0;
			CurrentAnimationFrame++;
			if (CurrentAnimationFrame > CharData.Standing.size() - 1)
			{
				CurrentAnimationFrame = 0;
			}
		}
	}
	
	public void Draw()
	{
		// Draw the ally.
		// Derive a position from the original because depending on the size of the texture,
		// the draw position might not be an exact translation from relative to absolute map coordinates,
		// and if it's not derived, a reference to the original point is stored, causing it to be modified (which is undesired).
		Point AllyDrawPos = new Point(Position.x, Position.y);
		AllyDrawPos = FrostFE.TileRelToAbsCoord(AllyDrawPos);
		Texture AllyDrawTex = getAnimationTexture();
		Color AllyDrawColor = (ActedThisTurn ? Color.gray : null);
		// Offset the exact position based on texture size.
		AllyDrawPos.x -= ((AllyDrawTex.ImageWidth - 15) / 2);
		AllyDrawPos.y -= (AllyDrawTex.ImageHeight - 15);
		// Finally, draw the character with refined drawing attributes.
		FrostFE.DrawQuad(AllyDrawPos.x, AllyDrawPos.y, AllyDrawTex.ImageWidth, AllyDrawTex.ImageHeight, AllyDrawColor, AllyDrawTex);
	}
	
	public void ModifyHP(int Difference)
	{
		if (Difference > 0)
		{
			// Handling HP increases.
			if (HP + Difference <= MaxHP)
			{
				HP += Difference;
			}
			else
			{
				HP = MaxHP;
			}
		}
		else
		{
			// Handling HP decreases.
			if (HP + Difference > 0)
			{
				HP += Difference;
			}
			else
			{
				HP = 0;
			}
			
			// Handling death of units.
			if (HP == 0)
			{
				if (Team == TEAM_PLAYER)
				{
					FrostFE.AlliedUnits.remove(this);
					// TODO In the real fire emblem, not ANY unit dying results in an automatic game over.
					Application.SetGameState(Application.GAMESTATE_GAMEOVER);
				}
				else
				{
					FrostFE.EnemyUnits.remove(this);
				}
			}
		}
	}
	
	public int GetMinAttackRange()
	{
		// 10 is just a high number that should be larger than any actual minimum range.
		// This is so it will later acquire the actual smallest attack range of all the weapons in the inventory.
		int RetVal = 10;
		boolean FoundMinimum = false;
		
		for (int i = 0; i < Items.length; i++)
		{
			if (Items[i] != null)
			{
				if (Items[i].ItemCore.MinAtkRange < RetVal)
				{
					RetVal = Items[i].ItemCore.MinAtkRange;
					FoundMinimum = true;
				}
			}
		}
		
		// If a minimum was found, return it. Otherwise, the person has no attack items so the minimum is 0.
		return (FoundMinimum ? RetVal : 0);
	}
	
	public int GetMaxAttackRange()
	{
		int RetVal = 0;
		
		for (int i = 0; i < Items.length; i++)
		{
			if (Items[i] != null)
			{
				if (Items[i].ItemCore.MaxAtkRange > RetVal)
				{
					RetVal = Items[i].ItemCore.MaxAtkRange;
				}
			}
		}
		
		return RetVal;
	}
	
	public int GetWeaponWeight()
	{
		// TODO Make this actually get weapon weight.
		return 1;
	}
	
	public int GetAttackSpeed()
	{
		if (GetWeaponWeight() > Constitution)
		{
			return (Speed + (Constitution - GetWeaponWeight()));
		}
		else
		{
			return Speed;
		}
	}
	
	public int GetWeaponHitRate()
	{
		// TODO Make this actually get weapon hit rate.
		return 100;
	}
	
	public int GetWeaponCritical()
	{
		// TODO Make this atually get WeaponCritical.
		return 1;
	}
	
	public int GetEffectiveWeaponMight()
	{
		// TODO Make this actually get effective weapon might.
		return 1;
	}
	
	public int GetCriticalEvade()
	{
		return Luck;
	}
	
	public int GetAvoid()
	{
		return (2 * GetAttackSpeed()) + Luck;
	}
	
	public double GetHitRate(FightableEntity Defender)
	{
		double ret = (2 * Skill) + (0.5 * Luck) - Defender.GetAvoid() + GetWeaponHitRate();
		
		return (ret <= 100 ? ret : 100);
	}
	
	public double GetCriticalHitRate(FightableEntity Defender)
	{
		return GetWeaponCritical() + (0.5 * Skill) - Defender.GetCriticalEvade();
	}
	
	public double GetAttackDamage(FightableEntity Defender)
	{
		return Strength + GetEffectiveWeaponMight() - Defender.Defense;
	}
	
	public int GetNumberOfAttacks(FightableEntity Defender)
	{
		if (GetAttackSpeed() >= (Defender.GetAttackSpeed() + 4))
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
	
	public InventoryItem GetEquippedItem()
	{
		for (int i = 0; i < Items.length; i++)
		{
			if (Items[i] != null)
			{
				if (Items[i].Equipped)
				{
					return Items[i];
				}
			}
		}
		return null;
	}
	
	public boolean IsDead()
	{
		return HP == 0;
	}
}

// A class to contain core (mainly animation) data for a certain character.
class CoreCharData
{
	public int ID;
	public String Name;
	public Vector<Texture> Standing;
	public Vector<Texture> Selected;
	public Texture InBattle;
	
	CoreCharData()
	{
		ID = 0;
		Name = "Unnamed";
		Standing = new Vector();
		Selected = new Vector();
		InBattle = null;
	}
}

// A class for some tile cursor-related functionality.
class TileCursor
{
	public static Point Location;
	public static Texture Tex;
	
	public static void Initialize()
	{
		Location = new Point(0, 0);
		Tex = Texture.Load("Res/cursor.png");
	}
}

// A small general utility class that has functions that doesn't really fit right elsewhere.
class Util
{
	public static double Distance(Point p1, Point p2)
	{
		return Math.sqrt(Math.pow(p2.x - p1.x, 2) + Math.pow(p2.y - p1.y, 2));
	}
	
	public static int TileMoveDistance(Point p1, Point p2)
	{
		// TODO This should take into account limitations from enemies being in the way...
		return (Math.abs(p2.x - p1.x) + Math.abs(p2.y - p1.y));
	}
}

// A class that holds information about the current battle instance.
class BattleInformation
{
	public FightableEntity AllyInFight;
	public FightableEntity EnemyInFight;
	public FightableEntity BattleInitiator;
	public FightableEntity Attacker;
	public FightableEntity Defender;
	public FightableEntity CurrentAttacker;
	
	public int AttackerAttackSpeed;
	public int DefenderAttackSpeed;
	public int AttackerNumberOfAttacks;
	public int DefenderNumberOfAttacks;
	public int AttackerHitRate;
	public boolean AttackerAttacksHit[];
	public boolean AttackerAttacksCrit[];
	public int DefenderHitRate;
	public boolean DefenderAttacksHit[];
	public boolean DefenderAttacksCrit[];
	public int AttackerCriticalRate;
	public int DefenderCriticalRate;
	public int AttackerDamage;
	public int DefenderDamage;
	
	public int AttackerNumLaunchedAttacks;
	public int DefenderNumLaunchedAttacks;
	public int CurrentAttackFrame;
	public int CurrentAttackFrameTime;
	public boolean AttackSwitchDelayActive;
	public int AttackSwitchDelayTime;
	
	public BattleInformation()
	{
		AllyInFight = null;
		EnemyInFight = null;
		BattleInitiator = null;
		Attacker = null;
		Defender = null;
		CurrentAttacker = null;

		AttackerAttackSpeed = 0;
		DefenderAttackSpeed = 0;
		AttackerNumberOfAttacks = 0;
		DefenderNumberOfAttacks = 0;
		AttackerHitRate = 0;
		DefenderHitRate = 0;
		AttackerCriticalRate = 0;
		DefenderCriticalRate = 0;
		AttackerDamage = 0;
		DefenderDamage = 0;

		AttackerNumLaunchedAttacks = 0;
		DefenderNumLaunchedAttacks = 0;
		CurrentAttackFrame = 0;
		CurrentAttackFrameTime = 0;
		AttackSwitchDelayActive = false;
		AttackSwitchDelayTime = 0;
	}
	
	public boolean BattleIsOver()
	{
		return AttackerNumLaunchedAttacks >= AttackerNumberOfAttacks && DefenderNumLaunchedAttacks >= DefenderNumberOfAttacks || Attacker.IsDead() || Defender.IsDead();
	}
	
	public void MakeCalculations()
	{
		Attacker = (BattleInitiator == AllyInFight ? AllyInFight : EnemyInFight);
		Defender = (BattleInitiator == AllyInFight ? EnemyInFight : AllyInFight);
		
		AttackerAttackSpeed = Attacker.GetAttackSpeed();
		DefenderAttackSpeed = Defender.GetAttackSpeed();
		AttackerNumberOfAttacks = Attacker.GetNumberOfAttacks(Defender);
		DefenderNumberOfAttacks = Defender.GetNumberOfAttacks(Attacker);
		AttackerHitRate = (int)Math.ceil(Attacker.GetHitRate(Defender));
		AttackerAttacksHit = new boolean[AttackerNumberOfAttacks];
		AttackerAttacksCrit = new boolean[AttackerNumberOfAttacks];
		DefenderHitRate = (int)Math.ceil(Defender.GetHitRate(Attacker));
		DefenderAttacksHit = new boolean[DefenderNumberOfAttacks];
		DefenderAttacksCrit = new boolean[DefenderNumberOfAttacks];
		AttackerCriticalRate = (int)Math.ceil(Attacker.GetCriticalHitRate(Defender));
		DefenderCriticalRate = (int)Math.ceil(Defender.GetCriticalHitRate(Attacker));
		for (int i = 0; i < AttackerNumberOfAttacks; i++)
		{
			AttackerAttacksHit[i] = (FrostFE.GetRandom(0, 100) <= AttackerHitRate ? true : false);
			AttackerAttacksCrit[i] = (FrostFE.GetRandom(0, 100) <= AttackerCriticalRate ? true : false);
		}
		for (int i = 0; i < DefenderNumberOfAttacks; i++)
		{
			DefenderAttacksHit[i] = (FrostFE.GetRandom(0, 100) <= DefenderHitRate ? true : false);
			DefenderAttacksCrit[i] = (FrostFE.GetRandom(0, 100) <= DefenderCriticalRate ? true : false);
		}
		AttackerDamage = (int)Math.ceil(Attacker.GetAttackDamage(Defender));
		DefenderDamage = (int)Math.ceil(Defender.GetAttackDamage(Attacker));
		
		// All the values have to be at least 0.
		AttackerHitRate = AttackerHitRate > 0 ? AttackerHitRate : 0;
		DefenderHitRate = DefenderHitRate > 0 ? DefenderHitRate : 0;
		AttackerCriticalRate = AttackerCriticalRate > 0 ? AttackerCriticalRate : 0;
		DefenderCriticalRate = DefenderCriticalRate > 0 ? DefenderCriticalRate : 0;
		AttackerDamage = AttackerDamage > 0 ? AttackerDamage : 0;
		DefenderDamage = DefenderDamage > 0 ? DefenderDamage : 0;
		
		CurrentAttacker = Attacker;
		
		// Quick fix to easily determine that a character with no weapon cannot attack at all.
		if (Defender.GetEquippedItem() == null)
		{
			DefenderNumberOfAttacks = 0;
		}
	}
}

// A class that holds data (really animations) for spells.
class SpellData
{
	public int ID;
	public Vector<Texture> AnimationFrames = new Vector();
}