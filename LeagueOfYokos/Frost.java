package league.of.yokos;

import java.awt.Point;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import javax.swing.ImageIcon;

/**
 *
 * A utility library called Frost.
 * It holds many functions for different purposes that overall make it easier to perform certain tasks.
 * 
 */
public class Frost
{
	public static void Sleep(int Milliseconds)
	{
		try
		{
			Thread.sleep(Milliseconds);
		}
		catch(InterruptedException e)
		{
		}
	}
	
	public static BufferedImage LoadTexture(String TextureName)
	{
		BufferedImage ret = null;
		try
		{
			ret = ImageIO.read(new File(TextureName));
		}
		catch (IOException e)
		{
			e.printStackTrace();
		}
		return ret;
	}
	
	public static Champion LoadChampion(String Filename)
	{
		Champion ret = new Champion();
		
		ret.X = 0.0f;
		ret.Y = 0.0f;
		ret.Level = 18;
		ret.HP = 300;
		ret.MaxHP = 500;
		ret.MP = 250;
		ret.MaxMP = 300;
		ret.EXP = 1000;
		ret.HPRegen = 5;
		ret.MPRegen = 5;
		ret.Armor = 10;
		ret.MagicResist = 10;
		ret.ArmorPenetration = 0;
		ret.MagicPenetration = 0;
		ret.AttackDamage = 60;
		ret.AbilityPower = 0;
		ret.AttackSpeed = 0.6f;
		ret.MovementSpeed = 100;
		ret.AutoAttackRange = 200;
		ret.VisionRadius = 400;
		ret.GoldPer10 = 13;
		
		ret.Name = "Zin Xhao";
		
		return ret;
	}
	
	public static Item LoadItem(int id)
	{
		XmlDocument xml = new XmlDocument();
		xml.Load("Items/" + id + ".xml");
		
		Item ret = new Item();
		
		ret.ID = id;
		
		XmlElement NameElem = xml.getElement("Name");
		XmlElement HPElem = xml.getElement("HP");
		XmlElement MPElem = xml.getElement("MP");
		XmlElement HPRegenElem = xml.getElement("HPRegen");
		XmlElement MPRegenElem = xml.getElement("MPRegen");
		XmlElement ArmorElem = xml.getElement("Armor");
		XmlElement MagicResistElem = xml.getElement("MagicResist");
		XmlElement ArmorPenetrationElem = xml.getElement("ArmorPenetration");
		XmlElement MagicPenetrationElem = xml.getElement("MagicPenetration");
		XmlElement AttackDamageElem = xml.getElement("AttackDamage");
		XmlElement AbilityPowerElem = xml.getElement("AbilityPower");
		XmlElement AttackSpeedElem = xml.getElement("AttackSpeed");
		XmlElement LifeStealElem = xml.getElement("LifeSteal");
		XmlElement SpellVampElem = xml.getElement("SpellVamp");
		XmlElement MovementSpeedElem = xml.getElement("MovementSpeed");
		XmlElement CriticalChanceElem = xml.getElement("CriticalChance");
		XmlElement CooldownReductionElem = xml.getElement("CooldownReduction");
		XmlElement TenacityElem = xml.getElement("Tenacity");
		XmlElement GoldPer10Elem = xml.getElement("GoldPer10");
		XmlElement BuildsFromElem = xml.getElement("BuildsFrom");
		XmlElement CostElem = xml.getElement("Cost");
		
		if (NameElem != null)
		{
			ret.Name = NameElem.Text;
		}
		
		if (HPElem != null)
		{
			ret.HP = Integer.parseInt(HPElem.Text);
		}
		
		if (MPElem != null)
		{
			ret.MP = Integer.parseInt(MPElem.Text);
		}
		
		if (HPRegenElem != null)
		{
			ret.HPRegen = Integer.parseInt(HPRegenElem.Text);
		}
		
		if (MPRegenElem != null)
		{
			ret.MPRegen = Integer.parseInt(MPRegenElem.Text);
		}
		
		if (ArmorElem != null)
		{
			ret.Armor = Integer.parseInt(ArmorElem.Text);
		}
		
		if (MagicResistElem != null)
		{
			ret.MagicResist = Integer.parseInt(MagicResistElem.Text);
		}
		
		if (ArmorPenetrationElem != null)
		{
			ret.ArmorPenetrationFlat = Integer.parseInt(ArmorPenetrationElem.getElement("flat").Text);
			ret.ArmorPenetrationPercent = Integer.parseInt(ArmorPenetrationElem.getElement("percent").Text);
		}
		
		if (MagicPenetrationElem != null)
		{
			ret.MagicPenetrationFlat = Integer.parseInt(MagicPenetrationElem.getElement("flat").Text);
			ret.MagicPenetrationPercent = Integer.parseInt(MagicPenetrationElem.getElement("percent").Text);
		}
		
		if (AttackDamageElem != null)
		{
			ret.AttackDamage = Integer.parseInt(AttackDamageElem.Text);
		}
		
		if (AbilityPowerElem != null)
		{
			ret.AbilityPower = Integer.parseInt(AbilityPowerElem.Text);
		}
		
		if (AttackSpeedElem != null)
		{
			ret.AttackSpeedFlat = Float.parseFloat(AttackSpeedElem.getElement("flat").Text);
			ret.AttackSpeedPercent = Integer.parseInt(AttackSpeedElem.getElement("percent").Text);
		}
		
		if (LifeStealElem != null)
		{
			ret.LifeStealPercent = Integer.parseInt(LifeStealElem.Text);
		}
		
		if (SpellVampElem != null)
		{
			ret.SpellVampPercent = Integer.parseInt(SpellVampElem.Text);
		}
		
		if (MovementSpeedElem != null)
		{
			ret.MovementSpeedFlat = Integer.parseInt(MovementSpeedElem.getElement("flat").Text);
			ret.MovementSpeedPercent = Integer.parseInt(MovementSpeedElem.getElement("percent").Text);
		}
		
		if (CriticalChanceElem != null)
		{
			ret.CriticalChance = Integer.parseInt(CriticalChanceElem.Text);
		}
		
		if (CooldownReductionElem != null)
		{
			ret.CooldownReduction = Integer.parseInt(CooldownReductionElem.Text);
		}
		
		if (TenacityElem != null)
		{
			ret.Tenacity = Integer.parseInt(TenacityElem.Text);
		}
		
		if (GoldPer10Elem != null)
		{
			ret.GoldPer10 = Integer.parseInt(GoldPer10Elem.Text);
		}
		
		if (BuildsFromElem != null)
		{
			String[] idList = BuildsFromElem.Text.split(",");
			
			for (int i = 0; i < idList.length; i++)
			{
				ret.BuildsFrom.add(new Integer(Integer.parseInt(idList[i])));
			}
		}
		
		if (CostElem != null)
		{
			ret.Cost = Integer.parseInt(CostElem.Text);
		}
		
		ret.icon = new ImageIcon("Items/img/" + ret.ID + ".png", ret.Name);
		
		return ret;
	}
	
	public static SkillInfo LoadSkillInfo(int id)
	{
		XmlDocument xml = new XmlDocument();
		xml.Load("Skills/" + id + ".xml");
		
		SkillInfo ret = new SkillInfo();
		
		ret.ID = id;
		ret.Name = xml.getElement("Name").Text;
		ret.Description = xml.getElement("Description").Text;
		String CDStrings[] = xml.getElement("Cooldown").Text.split(",");
		for (int i = 0; i < 5; i++)
		{
			ret.Cooldown[i] = Float.parseFloat(CDStrings[i]);
		}
		ret.CastTime = Float.parseFloat(xml.getElement("CastTime").Text);
		ret.Duration = Float.parseFloat(xml.getElement("Duration").Text);
		ret.Range = Integer.parseInt(xml.getElement("Range").Text);
		ret.Radius = Integer.parseInt(xml.getElement("Radius").Text);
		String HPCostStrings[] = xml.getElement("HPCost").Text.split(",");
		for (int i = 0; i < 5; i++)
		{
			ret.HPCost[i] = Integer.parseInt(HPCostStrings[i]);
		}
		String MPCostStrings[] = xml.getElement("MPCost").Text.split(",");
		for (int i = 0; i < 5; i++)
		{
			ret.MPCost[i] = Integer.parseInt(MPCostStrings[i]);
		}
		String PhysicalDamageStrings[] = xml.getElement("PhysicalDamage").Text.split(",");
		for (int i = 0; i < 5; i++)
		{
			ret.PhysicalDamage[i] = Integer.parseInt(PhysicalDamageStrings[i]);
		}
		String MagicDamageStrings[] = xml.getElement("MagicDamage").Text.split(",");
		for (int i = 0; i < 5; i++)
		{
			ret.MagicDamage[i] = Integer.parseInt(MagicDamageStrings[i]);
		}
		String TrueDamageStrings[] = xml.getElement("TrueDamage").Text.split(",");
		for (int i = 0; i < 5; i++)
		{
			ret.TrueDamage[i] = Integer.parseInt(TrueDamageStrings[i]);
		}
		ret.ADRatio = Float.parseFloat(xml.getElement("ADRatio").Text);
		ret.APRatio = Float.parseFloat(xml.getElement("APRatio").Text);
		ret.CanMoveWhileCasting = Boolean.parseBoolean(xml.getElement("CanMoveWhileCasting").Text);
		ret.RequiresTargetEntity = Boolean.parseBoolean(xml.getElement("RequiresTargetEntity").Text);
		ret.RequiresTargetLocation = Boolean.parseBoolean(xml.getElement("RequiresTargetLocation").Text);
		
		return ret;
	}
	
	// y1 is a coordinate (x,y,z) of the first point
	// y2 is a coordinate (x,y,z) of the second point
	// mu is the distance from the first line to the other which indicates the point desired to be interpolated.
	public static float LinearInterpolate(float y1, float y2, float mu)
	{
		return (y1*(1-mu)+y2*mu);
	}

	public static float Distance(float x1, float y1, float x2, float y2)
	{
		float dx = x1 - x2;
		float dy = y1 - y2;

		return (float)Math.sqrt( (dx * dx) + (dy * dy) );
	}
	
	public static float Distance(Point p1, Point p2)
	{
		return Distance(p1.x, p1.y, p2.x, p2.y);
	}
	
	public static int ClientToWorldX(int x)
	{
		return x - (int)LeagueOfYokos.CamX;
	}
	
	public static int ClientToWorldY(int y)
	{
		return y - (int)LeagueOfYokos.CamY;
	}
	
	public static Point ClientToWorld(Point p)
	{
		Point ret = new Point(p);
		ret.x = ClientToWorldX(p.x);
		ret.y = ClientToWorldY(p.y);
		return ret;
	}
	
	public static int ScreenToWorldX(int x)
	{
		return x + (int)LeagueOfYokos.CamX;
	}
	
	public static int ScreenToWorldY(int y)
	{
		return y + (int)LeagueOfYokos.CamY;
	}
	
	public static Point ScreenToWorld(Point p)
	{
		Point ret = new Point(p);
		ret.x = ScreenToWorldX(p.x);
		ret.y = ScreenToWorldY(p.y);
		return ret;
	}
	
	public static int WorldToTileX(int x)
	{
		return (x / LeagueOfYokos.TileSize);
	}
	
	public static int WorldToTileY(int y)
	{
		return (y / LeagueOfYokos.TileSize);
	}
	
	public static Point WorldToTile(Point p)
	{
		Point ret = new Point(p);
		ret.x = WorldToTileX(p.x);
		ret.y = WorldToTileY(p.y);
		return ret;
	}
	
	public static int TileToWorldX(int x)
	{
		return (x * LeagueOfYokos.TileSize);
	}
	
	public static int TileToWorldY(int y)
	{
		return (y * LeagueOfYokos.TileSize);
	}
	
	public static Point TileToWorld(Point p)
	{
		Point ret = new Point(p);
		ret.x = TileToWorldX(p.x);
		ret.y = TileToWorldY(p.y);
		return ret;
	}
}