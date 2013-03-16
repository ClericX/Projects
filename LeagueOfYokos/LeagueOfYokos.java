package league.of.yokos;

import javax.swing.*;
import java.io.File;
import java.io.IOException;
import java.awt.image.*;
import javax.imageio.*;
import java.io.FileReader;
import java.io.BufferedReader;
import java.awt.Point;
import java.awt.event.*;
import java.awt.*;
import java.io.*;
import java.util.HashMap;
import java.util.Vector;

/**
 *
 * The main game class. Most functionality is delegated to subclasses and handled there.
 * 
 */
public class LeagueOfYokos extends JPanel implements MouseListener, MouseMotionListener, KeyListener, ActionListener
{
	public static Timer FrameRenderTimer = null;
	public static long LastRenderTime = 0;
	public static long ElapsedTimeThisFrame = 0;
	public static long CurrentTimeMillis = 0;
	public static BufferedImage BackBufferImage = null;
	public static Graphics2D BackBuffer = null;
	public static JFrame frame = null;
	public static JPanel GamePanel = null;
	
	// Resource banks.
	public static Vector<Player> Players = new Vector();
	public static Vector<Champion> Champions = new Vector();
	public static Vector<Item> Items = new Vector();
	public static Vector<SkillInfo> SkillInfos = new Vector();
	public static Vector<Buff> Buffs = new Vector();
	
	public static Player ThisPlayer = null;
	public static Font DefaultFont = new Font("Monospaced", Font.PLAIN, 13);
	public static Font ShopTitleFont = new Font("Monospaced", Font.BOLD, 20);
	public static FontMetrics metrics = null;
	public static float CamX = 0.0f;
	public static float CamY = 0.0f;
	public static float CamMoveSpeed = 500.0f; // Number of pixels the cam moves per sec if applicable.
	public static Point MousePos = new Point();
	
	public static String Map[][] = null;
	public static String MapName = "";
	public static int MapWidth = 536;
	public static int MapHeight = 431;
	public static int MapWidthPixels = 5364;
	public static int MapHeightPixels = 4310;
	public static int TileSize = 10;
	public static BufferedImage MapBG = null;
	
	public static int WindowWidth = 1024;
	public static int WindowHeight = 768;
	
	public static boolean DrawMap = true;
	public static boolean DrawMapImage = true;
	public static HashMap<String,Color> ColorMap = new HashMap();
	public static Vector<String> ColorMapNames = new Vector();
	
	public static int CurrentMapEditorColor = 0;
	public static boolean SettingMapColors = false;
	
	public static int ScreenWidth = 0;
	public static int ScreenHeight = 0;
	
	public Shop TheShop;

	public LeagueOfYokos()
	{
		// The magic starts.
		Init();

		Champions.add(Frost.LoadChampion("f3n"));
		Champions.add(Frost.LoadChampion("f33n"));

		Player p = new Player();

		p.setChampion(Champions.get(0));
		p.getChampion().Side = FightingEntity.BLUE_SIDE;
		p.Username = "YokoOzawa";
		
		p.getChampion().setLocation(10, MapHeightPixels - 300);

		Players.add(p);

		ThisPlayer = p;
		
		Player d = new Player();
		
		d.setChampion(Champions.get(1));
		d.getChampion().Side = FightingEntity.PURPLE_SIDE;
		d.Username = "f3n";
		
		d.getChampion().setLocation(400, MapHeightPixels - 300);
		
		Players.add(d);
		
		// Set the camera location based on player side.
		if (ThisPlayer.getChampion().Side == FightingEntity.BLUE_SIDE)
		{
			CamX = 0;
			CamY = MapHeightPixels - WindowHeight;
		}
		else if (ThisPlayer.getChampion().Side == FightingEntity.PURPLE_SIDE)
		{
			CamX = MapWidthPixels - WindowWidth;
			CamY = 0;
		}
	}

	public void Init()
	{
		GamePanel = this;
		
		// Full screen window init.
		ScreenWidth = Toolkit.getDefaultToolkit().getScreenSize().width;
		ScreenHeight = Toolkit.getDefaultToolkit().getScreenSize().height;
		
		WindowWidth = ScreenWidth;
		WindowHeight = ScreenHeight;
		
		// Oh really?
		frame = new JFrame();

		// Need dat juicy input.
		GamePanel.addKeyListener(this);
		GamePanel.addMouseListener(this);
		GamePanel.addMouseMotionListener(this);
		GamePanel.setFocusable(true);
		GamePanel.transferFocus();

		// Gotta make teh window sexy.
		frame.setUndecorated(true);
		frame.setAlwaysOnTop(true);
		
		frame.setTitle("League of Yokos Client - v1");
		frame.setLocation((ScreenWidth / 2) - (WindowWidth / 2), (ScreenHeight / 2) - (WindowHeight / 2));
		GamePanel.setLayout(null);
		GamePanel.setLocation(0, 0);
		GamePanel.setSize(WindowWidth, WindowHeight);
		frame.setContentPane(this);
		GamePanel.setPreferredSize(this.getSize());
		frame.pack();
		
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		//frame.setIgnoreRepaint(true);
		frame.setResizable(false);
		frame.setVisible(true);

		// Need vsync so it doesn't look like a 5 yr old made it.
		BackBufferImage = new BufferedImage(WindowWidth, WindowHeight, BufferedImage.TYPE_INT_ARGB);
		BackBufferImage.setAccelerationPriority(1.0f);
		BackBuffer = BackBufferImage.createGraphics();
		
		// Dat HD map f3n.
		MapBG = Frost.LoadTexture("map.jpg");
		LoadColorMap("colormap.xml");
		
		// CSV map loading.
		Map = LoadMap("map.csv");
		
		// Load resources.
		LoadItems();
		LoadSkills();
		
		// Load the shop.
		TheShop = new Shop();
		
		// Custom font ftw.
		BackBuffer.setFont(DefaultFont);
		metrics = BackBuffer.getFontMetrics();

		// Dat 60 fps.
		CurrentTimeMillis = System.currentTimeMillis();
		LastRenderTime = CurrentTimeMillis;
		
		// Start the timer loop.
		FrameRenderTimer = new Timer(0, this);
		FrameRenderTimer.start();
	}
	
	public static void LoadColorMap(String path)
	{
		if (!ColorMap.isEmpty())
		{
			ColorMap.clear();
		}
		
		XmlDocument colorMapDoc = new XmlDocument();
		colorMapDoc.Load(path);
		
		for (int i = 0; i < colorMapDoc.SubElements.size(); i++)
		{
			XmlElement elem = colorMapDoc.SubElements.get(i);
			String clrStr[] = elem.Text.split(",");
			int R = Integer.parseInt(clrStr[0]);
			int G = Integer.parseInt(clrStr[1]);
			int B = Integer.parseInt(clrStr[2]);
			Color generatedColor = new Color(R, G, B, 150);
			Color generatedFogColor = new Color(Math.max(0, R - 30), Math.max(0, G - 30), Math.max(0, B - 30), 150);
			
			ColorMapNames.add(elem.Name);
			ColorMap.put(elem.Name, generatedColor);
			ColorMap.put(elem.Name + "FOG", generatedFogColor);
		}
	}
	
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
			setMapSize(Integer.parseInt(MapInfo[1]), Integer.parseInt(MapInfo[2]));
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
			
			in.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
		return LoadedMap;
	}
	
	public static void SaveMap(String TheMapFileName)
	{
		try
		{
			BufferedWriter in = new BufferedWriter(new FileWriter(TheMapFileName));
			
			// Save some map info.
			in.write(MapName + "," + MapWidth + "," + MapHeight);
			in.newLine();
			
			// Load each map tile.
			for (int y = 0; y < MapHeight; y++)
			{
				for (int x = 0; x < MapWidth; x++)
				{
					in.write(Map[x][y]);
					
					if (x < MapWidth - 1)
					{
						in.write(",");
					}
				}
				in.newLine();
			}
			
			in.close();
		}
		catch(IOException e)
		{
			e.printStackTrace();
		}
	}
	
	public static void setMapSize(int w, int h)
	{
		MapWidth = w;
		MapHeight = h;
		MapWidthPixels = w * TileSize;
		MapHeightPixels = h * TileSize;
	}
	
	public static Item getItemById(int id)
	{
		for (int i = 0; i < Items.size(); i++)
		{
			Item item = Items.get(i);
			
			if (item.ID == id)
			{
				return item;
			}
		}
		
		return null;
	}
	
	public static SkillInfo getSkillInfoById(int id)
	{
		for (int i = 0; i < SkillInfos.size(); i++)
		{
			SkillInfo info = SkillInfos.get(i);
			
			if (info.ID == id)
			{
				return info;
			}
		}
		
		return null;
	}
	
	public static void LoadItems()
	{
		try
		{
			boolean StillReadingItems = true;
			int CurrentItemID = 0;
			
			while (StillReadingItems)
			{
				File f = new File("Items/" + CurrentItemID + ".xml");
				
				if (f.exists())
				{
					Items.add(Frost.LoadItem(CurrentItemID));
					CurrentItemID++;
				}
				else
				{
					StillReadingItems = false;
				}
			}
		}
		catch (Exception e)
		{
			System.out.println("An exception was raised while attempting to load items.");
		}
		
		System.out.println("Loaded " + Items.size() + " items.");
	}
	
	public static void LoadSkills()
	{
		try
		{
			boolean StillReadingSkills = true;
			int CurrentSkillID = 0;
			
			while (StillReadingSkills)
			{
				File f = new File("Skills/" + CurrentSkillID + ".xml");
				
				if (f.exists())
				{
					SkillInfos.add(Frost.LoadSkillInfo(CurrentSkillID));
					CurrentSkillID++;
				}
				else
				{
					StillReadingSkills = false;
				}
			}
		}
		catch (Exception e)
		{
			System.out.println("An exception was raised while attempting to load skills.");
		}
		
		System.out.println("Loaded " + SkillInfos.size() + " skills.");
	}
	
	// Draw function-related wrappers for ease-of-use and client-to-world coordinate system transforms.
	public static void DrawString(String str, int x, int y)
	{
		BackBuffer.drawString(str, Frost.ClientToWorldX(x), Frost.ClientToWorldY(y));
	}
	
	public static void DrawImage(BufferedImage img, int x, int y)
	{
		BackBuffer.drawImage(img, null, Frost.ClientToWorldX(x), Frost.ClientToWorldY(y));
	}
	
	public static void FillRect(int x, int y, int w, int h)
	{
		BackBuffer.fillRect(Frost.ClientToWorldX(x), Frost.ClientToWorldY(y), w, h);
	}
	
	public static void DrawRect(int x, int y, int w, int h)
	{
		BackBuffer.drawRect(Frost.ClientToWorldX(x), Frost.ClientToWorldY(y), w, h);
	}
	
	public static void DrawOval(int x, int y, int width, int height)
	{
		BackBuffer.drawOval(Frost.ClientToWorldX(x), Frost.ClientToWorldY(y), width, height);
	}
	public static void FillOval(int x, int y, int width, int height)
	{
		BackBuffer.fillOval(Frost.ClientToWorldX(x), Frost.ClientToWorldY(y), width, height);
	}
	
	public static void DrawPlayer(Player ply)
	{
		BackBuffer.setColor(Color.magenta);
		FillRect((int) ply.getChampion().X, (int) ply.getChampion().Y, 25, 50); // Actual champ
		BackBuffer.setColor(Color.lightGray);
		DrawString(ply.Username, (int)(ply.getChampion().X + ((100 - metrics.stringWidth(ply.Username)) / 2)), (int) (ply.getChampion().Y - 30)); // Summoner name
		BackBuffer.setColor(Color.yellow);
		DrawString(Integer.toString(ply.getChampion().Level), (int)ply.getChampion().X, (int)(ply.getChampion().Y - 13));
		BackBuffer.setColor(DarkRed);
		FillRect((int) ply.getChampion().X + 20, (int) ply.getChampion().Y - 25, (int) (((float) ply.getChampion().HP / (float) ply.getChampion().MaxHP) * 80), 5);
		BackBuffer.setColor(Color.red);
		DrawRect((int) ply.getChampion().X + 20, (int) ply.getChampion().Y - 25, 80, 5);
		BackBuffer.setColor(DarkBlue);
		FillRect((int) ply.getChampion().X + 20, (int) ply.getChampion().Y - 15, (int) (((float) ply.getChampion().MP / (float) ply.getChampion().MaxMP) * 80), 5);
		BackBuffer.setColor(Color.blue);
		DrawRect((int) ply.getChampion().X + 20, (int) ply.getChampion().Y - 15, 80, 5);
	}
	
	public static Color DarkRed = new Color(50, 0, 0);
	public static Color DarkBlue = new Color(0, 0, 50);
	public static Color DarkGreen = new Color(0, 50, 0);
	public static Color DarkYellow = new Color(100, 100, 50);
	
	public static Color SelectedRadiusFill = new Color(51, 204, 255, 100);
	public static Color SelectedRadiusBorder = new Color(0, 134, 179, 200);
	
	public static Color GrassColor = new Color(0, 255, 0, 100);
	public static Color BlackColor = new Color(0, 0, 0, 100);
	
	public static Color CooldownDisplayColor = new Color (0, 0, 0, 100);
	
	public static Color getMappedColor(String name)
	{
		if (ColorMap.containsKey(name))
		{
			return ColorMap.get(name);
		}
		else
		{
			return BlackColor;
		}
	}
	
	public static void DrawMapTile(int x, int y)
	{
		if (ThisPlayer.getChampion().hasVisionOfTile(x, y))
		{
			BackBuffer.setColor(getMappedColor(Map[x][y]));
		}
		else
		{
			BackBuffer.setColor(getMappedColor(Map[x][y] + "FOG"));
		}
		FillRect(x * 10, y * 10, 10, 10);
	}

	public void RenderFrame()
	{
		BackBuffer.setBackground(Color.white);
		BackBuffer.clearRect(0, 0, WindowWidth, WindowHeight);
		
		if (DrawMapImage)
		{
			BackBuffer.drawImage(MapBG, null, Frost.ClientToWorldX(0), Frost.ClientToWorldY(0));
		}
		
		if (DrawMap)
		{
			int CamXTile = Frost.WorldToTileX((int)CamX);
			int CamYTile = Frost.WorldToTileY((int)CamY);

			for (int TileX = CamXTile; TileX < Math.min(CamXTile + (WindowWidth / TileSize) + 2, MapWidth); TileX++)
			{
				for (int TileY = CamYTile; TileY < Math.min(CamYTile + (WindowHeight / TileSize) + 2, MapHeight); TileY++)
				{
					DrawMapTile(TileX, TileY);
				}
			}
		}

		BackBuffer.setColor(Color.black);
		BackBuffer.drawString("CamX: " + CamX + " | CamY: " + CamY + " | MouseX: " + MousePos.x + " | MouseY: " + MousePos.y + " | Map Edit Color: " + ColorMapNames.get(CurrentMapEditorColor), 50, 590);

		for (int i = 0; i < Players.size(); i++)
		{
			Player ply = Players.get(i);
			
			// Handle this player's possible champ movement; handling movement 'tick'.
			if (ply.getChampion().MovePath.isSetup())
			{
				ply.getChampion().MovePath.Move(ThisPlayer.getChampion().MovementSpeed, ElapsedTimeThisFrame);

				ply.getChampion().X = ply.getChampion().MovePath.CurX;
				ply.getChampion().Y = ply.getChampion().MovePath.CurY;

				// Stop movement if it's complete.
				if (ply.getChampion().MovePath.isCompleted())
				{
					ply.getChampion().MovePath.reset();
				}
			}
			
			// Handle health regen.
			if (ply.getChampion().HP < ply.getChampion().MaxHP)
			{
				ply.getChampion().setHP(ply.getChampion().HP + (ply.getChampion().HPRegen * ((float)ElapsedTimeThisFrame / (float)5000)));
			}
			
			// Handle mana regen.
			if (ply.getChampion().MP < ply.getChampion().MaxMP)
			{
				ply.getChampion().setMP(ply.getChampion().MP + (ply.getChampion().MPRegen * ((float)ElapsedTimeThisFrame / (float)5000)));
			}
			
			// Give gold per second.
			ply.getChampion().Gold += (ply.getChampion().GoldPer10 * ((float)ElapsedTimeThisFrame / 10000.0f));
			
			if (ply.getChampion().SelectedSkill != null)
			{
				int sz = (int)ply.getChampion().SelectedSkill.info.Radius * 2;
				int hsz = sz / 2;
				Rectangle bb = ply.getChampion().getBoundingBox();
				int x = bb.x + (bb.width / 2) - hsz;
				int y = bb.y + (bb.height / 2) - hsz;
				BackBuffer.setColor(SelectedRadiusFill);
				FillOval(x, y, sz, sz);
				
				for (int grad = 0; grad < 30; grad++)
				{
					int subFac = (150 - (grad*5));
					int r = Math.min(Math.max(SelectedRadiusFill.getRed() - subFac, 0), 255);
					int g = Math.min(Math.max(SelectedRadiusFill.getGreen() - subFac, 0), 255);
					int b = Math.min(Math.max(SelectedRadiusFill.getBlue() - subFac, 0), 255);
					Color gradClr = new Color(r, g, b, SelectedRadiusFill.getAlpha());
					BackBuffer.setColor(gradClr);
					DrawOval(x + grad, y + grad, sz - (grad * 2), sz - (grad * 2));
				}
				
				BackBuffer.setColor(SelectedRadiusBorder);
				DrawOval(x, y, sz, sz);
			}
			
			// Draw the player-related stuff.
			DrawPlayer(ply);
			
			// Tick all active skills.
			for (int activeSkill = 0; activeSkill < ply.getChampion().ActiveSkills.size(); activeSkill++)
			{
				ply.getChampion().ActiveSkills.get(activeSkill).tick();
			}
			
			// Lower all skill cooldowns.
			ply.getChampion().Skill1.lowerCooldown((float)ElapsedTimeThisFrame / 1000.0f);
			ply.getChampion().Skill2.lowerCooldown((float)ElapsedTimeThisFrame / 1000.0f);
			ply.getChampion().Skill3.lowerCooldown((float)ElapsedTimeThisFrame / 1000.0f);
			ply.getChampion().Skill4.lowerCooldown((float)ElapsedTimeThisFrame / 1000.0f);
		}
		
		// Draw the 2D UI elements.
		
		// Middle stat + skill area
		BackBuffer.setColor(Color.gray);
		BackBuffer.fillRect(330, WindowHeight - 120, 400, 120);
		
		// Skill images and cooldown display.
		BackBuffer.setColor(DarkRed);
		BackBuffer.fillRect(350, WindowHeight - 100, 40, 40);
		BackBuffer.setColor(CooldownDisplayColor);
		BackBuffer.fillRect(350, WindowHeight - 100, (int)((ThisPlayer.getChampion().Skill1.CurrentCooldownTime / ThisPlayer.getChampion().Skill1.getCooldown()) * 40), 40);
		BackBuffer.setColor(DarkRed);
		BackBuffer.fillRect(400, WindowHeight - 100, 40, 40);
		BackBuffer.setColor(CooldownDisplayColor);
		BackBuffer.fillRect(350, WindowHeight - 100, (int)((ThisPlayer.getChampion().Skill2.CurrentCooldownTime / ThisPlayer.getChampion().Skill2.getCooldown()) * 40), 40);
		BackBuffer.setColor(DarkRed);
		BackBuffer.fillRect(450, WindowHeight - 100, 40, 40);
		BackBuffer.setColor(CooldownDisplayColor);
		BackBuffer.fillRect(350, WindowHeight - 100, (int)((ThisPlayer.getChampion().Skill3.CurrentCooldownTime / ThisPlayer.getChampion().Skill3.getCooldown()) * 40), 40);
		BackBuffer.setColor(DarkRed);
		BackBuffer.fillRect(500, WindowHeight - 100, 40, 40);
		BackBuffer.setColor(CooldownDisplayColor);
		BackBuffer.fillRect(350, WindowHeight - 100, (int)((ThisPlayer.getChampion().Skill4.CurrentCooldownTime / ThisPlayer.getChampion().Skill4.getCooldown()) * 40), 40);
		// HP Bar
		BackBuffer.setColor(Color.white);
		BackBuffer.drawString("+" + ThisPlayer.getChampion().HPRegen, 620, WindowHeight - 30);
		BackBuffer.setColor(Color.red);
		BackBuffer.fillRect(350, WindowHeight - 40, (int)((ThisPlayer.getChampion().HP / ThisPlayer.getChampion().MaxHP) * 300), 15);
		BackBuffer.setColor(DarkRed);
		BackBuffer.drawRect(350, WindowHeight - 40, 300, 15);
		BackBuffer.setColor(Color.white);
		BackBuffer.drawString("HP: " + (int)ThisPlayer.getChampion().HP + " / " + (int)ThisPlayer.getChampion().MaxHP, 400, WindowHeight - 30);
		// MP Bar
		BackBuffer.setColor(Color.white);
		BackBuffer.drawString("+" + ThisPlayer.getChampion().MPRegen, 620, WindowHeight - 10);
		BackBuffer.setColor(Color.blue);
		BackBuffer.fillRect(350, WindowHeight - 20, (int)((ThisPlayer.getChampion().MP / ThisPlayer.getChampion().MaxMP) * 300), 15);
		BackBuffer.setColor(DarkBlue);
		BackBuffer.drawRect(350, WindowHeight - 20, 300, 15);
		BackBuffer.setColor(Color.white);
		BackBuffer.drawString("MP: " + (int)ThisPlayer.getChampion().MP + " / " + (int)ThisPlayer.getChampion().MaxMP, 400, WindowHeight - 10);
		
		// General stat box area
		BackBuffer.setColor(Color.gray);
		BackBuffer.fillRect(0, WindowHeight - 140, 300, 140);
		// Stats
		BackBuffer.setColor(DarkGreen);
		BackBuffer.drawString(ThisPlayer.Username + " - " + ThisPlayer.getChampion().Name + " - " + "Lv " + ThisPlayer.getChampion().Level, 10, WindowHeight - 120);
		BackBuffer.drawString("AtkDmg: " + ThisPlayer.getChampion().AttackDamage, 10, WindowHeight - 105);
		BackBuffer.drawString("AblPwr: " + ThisPlayer.getChampion().AbilityPower, 10, WindowHeight - 90);
		BackBuffer.drawString("AtkSpd: " + ThisPlayer.getChampion().AttackSpeed, 10, WindowHeight - 75);
		BackBuffer.drawString("MovSpd: " + ThisPlayer.getChampion().MovementSpeed, 10, WindowHeight - 60);
		BackBuffer.drawString("Armor : " + ThisPlayer.getChampion().Armor, 10, WindowHeight - 45);
		BackBuffer.drawString("MRes  : " + ThisPlayer.getChampion().MagicResist, 10, WindowHeight - 30);
		// Items
		BackBuffer.setColor(DarkRed);
		BackBuffer.fillRect(150, WindowHeight - 110, 25, 25);
		BackBuffer.fillRect(180, WindowHeight - 110, 25, 25);
		BackBuffer.fillRect(210, WindowHeight - 110, 25, 25);
		BackBuffer.fillRect(150, WindowHeight - 80, 25, 25);
		BackBuffer.fillRect(180, WindowHeight - 80, 25, 25);
		BackBuffer.fillRect(210, WindowHeight - 80, 25, 25);
		// Gold
		BackBuffer.drawString("G: " + (int)ThisPlayer.getChampion().Gold, 150, WindowHeight - 35);
		// EXP Bar
		BackBuffer.setColor(Color.yellow);
		BackBuffer.fillRect(10, WindowHeight - 20, (int)(((float)ThisPlayer.getChampion().EXP / (float)ThisPlayer.getChampion().getLevelUpEXP()) * 250), 10);
		BackBuffer.setColor(DarkYellow);
		BackBuffer.drawRect(10, WindowHeight - 20, 250, 10);
		BackBuffer.setColor(DarkRed);
		BackBuffer.drawString("EXP: " + ThisPlayer.getChampion().EXP + " / " + ThisPlayer.getChampion().getLevelUpEXP(), 50, WindowHeight - 10);
		
		// 2D camera movement.
		float FreeCamMoveModifier = CamMoveSpeed * ((float)ElapsedTimeThisFrame / 1000.0f);
		
		if (MousePos.x <= 50 && CamX > 0)
		{
			// move cam left
			CamX -= FreeCamMoveModifier;
			
			// Lock to map bounds.
			if (CamX < 0)
			{
				CamX = 0;
			}
		}
		else if (MousePos.x >= frame.getWidth() - 50 && CamX < MapWidthPixels - frame.getWidth())
		{
			// move cam right
			CamX += FreeCamMoveModifier;
			
			// Lock to map bounds.
			if (CamX > MapWidthPixels - frame.getWidth())
			{
				CamX = MapWidthPixels - frame.getWidth();
			}
		}
		
		if (MousePos.y <= 50 && CamY > 0)
		{
			// move cam up
			CamY -= FreeCamMoveModifier;
			
			// Lock to map bounds.
			if (CamY < 0)
			{
				CamY = 0;
			}
		}
		else if (MousePos.y >= frame.getHeight() - 50 && CamY < MapHeightPixels - frame.getHeight())
		{
			// move cam down
			CamY += FreeCamMoveModifier;
			
			// Lock to map bounds.
			if (CamY > MapHeightPixels - frame.getHeight())
			{
				CamY = MapHeightPixels - frame.getHeight();
			}
		}

		// Draw the buffer to the window.
		repaint();
	}

	public static void main(String[] args)
	{
		LeagueOfYokos inst = new LeagueOfYokos();
	}

	@Override
	public void paintComponent(Graphics g)
	{
		Graphics2D g2d = (Graphics2D) g;
		g2d.drawImage(BackBufferImage, null, 0, 0);
	}

	/////////////////////////////////////
	// Implemented interface functions //
	/////////////////////////////////////
	public void mouseClicked(MouseEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
	}
	
	public static FightingEntity getEntityAtMousePos()
	{
		for (int i = 0; i < Players.size(); i++)
		{
			Player d = Players.get(i);
			if (d.getChampion().getBoundingBox().contains(Frost.ScreenToWorld(MousePos)))
			{
				return d.getChampion();
			}
		}
		return null;
	}

	public void mousePressed(MouseEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");

		// Handle right clicks to move somewhere.
		if (e.getButton() == MouseEvent.BUTTON3)
		{
			FightingEntity target = getEntityAtMousePos();
			
			if (target != null && !ThisPlayer.getChampion().IsEnemy(target))
			{
				target = null;
			}
			
			if (target == null)
			{
				ThisPlayer.getChampion().setTargetMoveLocation(Frost.ScreenToWorldX(e.getPoint().x), Frost.ScreenToWorldY(e.getPoint().y));
			}
			else if (ThisPlayer.getChampion().AutoAttack.canCast())
			{
				ThisPlayer.getChampion().AutoAttack.cast(e.getPoint(), target);
			}
			
			//ThisPlayer.champ.Stats.setLocation(e.getPoint());
		}
		
		/*
		// Map editor stuff
		// TODO: Do someting about the map editor functionality.
		if (e.getButton() == MouseEvent.BUTTON1)
		{
			SettingMapColors = true;
		}
		*/
		
		if (e.getButton() == MouseEvent.BUTTON1)
		{
			if (ThisPlayer.getChampion().SelectedSkill != null)
			{
				FightingEntity target = getEntityAtMousePos();
				
				if (target != null && !ThisPlayer.getChampion().IsEnemy(target))
				{
					target = null;
				}
				
				if (target != null)
				{
					if (ThisPlayer.getChampion().SelectedSkill.cast(MousePos, target))
					{
						ThisPlayer.getChampion().SelectedSkill = null;
					}
				}
			}
		}
	}

	public void mouseReleased(MouseEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
		
		if (e.getButton() == MouseEvent.BUTTON1)
		{
			SettingMapColors = false;
		}
	}

	public void mouseEntered(MouseEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	public void mouseExited(MouseEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	public void mouseDragged(MouseEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
		MousePos.setLocation(e.getPoint());
		
		if (SettingMapColors)
		{
			Point t = Frost.WorldToTile(Frost.ScreenToWorld(MousePos));
			Map[t.x][t.y] = ColorMapNames.get(CurrentMapEditorColor);
		}
	}

	public void mouseMoved(MouseEvent e)
	{
		MousePos.setLocation(e.getPoint());
	}

	public void keyTyped(KeyEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	public void keyPressed(KeyEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
		
		if (e.getKeyChar() == 'm')
		{
			DrawMap = !DrawMap;
		}
		
		if (e.getKeyChar() == 'n')
		{
			DrawMapImage = !DrawMapImage;
		}
		
		if (e.getKeyChar() == 'b')
		{
			Map = LoadMap("map.csv");
		}
		
		if (e.getKeyChar() == 'v')
		{
			LoadColorMap("colormap.xml");
		}
		
		if (e.getKeyChar() == 'c')
		{
			if (CurrentMapEditorColor < ColorMapNames.size() - 1)
			{
				CurrentMapEditorColor++;
			}
			else
			{
				CurrentMapEditorColor = 0;
			}
		}
		
		if (e.getKeyChar() == 'x')
		{
			SaveMap("map.csv");
		}
		
		// Legit LoY hotkeys and stuff.
		
		if (e.getKeyChar() == 'q')
		{
			if (ThisPlayer.getChampion().Skill1.canCast())
			{
				ThisPlayer.getChampion().SelectedSkill = ThisPlayer.getChampion().Skill1;
			}
		}
		
		if (e.getKeyChar() == 'w')
		{
			if (ThisPlayer.getChampion().Skill2.canCast())
			{
				ThisPlayer.getChampion().SelectedSkill = ThisPlayer.getChampion().Skill2;
			}
		}
		
		if (e.getKeyChar() == 'e')
		{
			if (ThisPlayer.getChampion().Skill3.canCast())
			{
				ThisPlayer.getChampion().SelectedSkill = ThisPlayer.getChampion().Skill3;
			}
		}
		
		if (e.getKeyChar() == 'r')
		{
			if (ThisPlayer.getChampion().Skill4.canCast())
			{
				ThisPlayer.getChampion().SelectedSkill = ThisPlayer.getChampion().Skill4;
			}
		}
		
		if (e.getKeyChar() == 'p')
		{
			if (frame.isAncestorOf(TheShop))
			{
				frame.remove(TheShop);
			}
			else
			{
				frame.add(TheShop);
			}
		}
		
		if (e.getKeyCode() == KeyEvent.VK_ESCAPE)
		{
			FrameRenderTimer.stop();
			frame.dispose();
			//System.exit(0);
		}
	}

	public void keyReleased(KeyEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
	}

	public void actionPerformed(ActionEvent e)
	{
		//throw new UnsupportedOperationException("Not supported yet.");
		if (e.getSource() == FrameRenderTimer)
		{
			if (System.currentTimeMillis() >= (LastRenderTime + 15))
			{
				CurrentTimeMillis = System.currentTimeMillis();
				ElapsedTimeThisFrame = CurrentTimeMillis - LastRenderTime;
				LastRenderTime = CurrentTimeMillis;
				RenderFrame();
			}
		}
	}
}