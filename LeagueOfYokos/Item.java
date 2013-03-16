package league.of.yokos;

import java.util.Vector;
import javax.swing.ImageIcon;

public class Item
{
	public int ID;
	public ImageIcon icon;
	public String Name;
	public int HP;
	public int MP;
	public int HPRegen;
	public int MPRegen;
	public int Armor;
	public int MagicResist;
	public int ArmorPenetrationFlat;
	public int ArmorPenetrationPercent;
	public int MagicPenetrationFlat;
	public int MagicPenetrationPercent;
	public int AttackDamage;
	public int AbilityPower;
	public float AttackSpeedFlat;
	public int AttackSpeedPercent;
	public int LifeStealPercent;
	public int SpellVampPercent;
	public int MovementSpeedFlat;
	public int MovementSpeedPercent;
	public int CriticalChance;
	public int CooldownReduction;
	public int Tenacity;
	public int GoldPer10;
	public Vector<Integer> BuildsFrom;
	public int Cost;
	
	public Item()
	{
		ID = 0;
		icon = null;
		Name = "N/A";
		HP = 0;
		MP = 0;
		HPRegen = 0;
		MPRegen = 0;
		Armor = 0;
		MagicResist = 0;
		ArmorPenetrationFlat = 0;
		ArmorPenetrationPercent = 0;
		MagicPenetrationFlat = 0;
		MagicPenetrationPercent = 0;
		AttackDamage = 0;
		AbilityPower = 0;
		AttackSpeedFlat = 0;
		AttackSpeedPercent = 0;
		LifeStealPercent = 0;
		SpellVampPercent = 0;
		MovementSpeedFlat = 0;
		MovementSpeedPercent = 0;
		CriticalChance = 0;
		CooldownReduction = 0;
		Tenacity = 0;
		GoldPer10 = 0;
		BuildsFrom = new Vector();
		Cost = 0;
	}
	
	public int getCost()
	{
		if (!BuildsFrom.isEmpty())
		{
			Vector<Item> AllRequiredItems = new Vector();
		
			Vector<Item> CurrentItems;
			Vector<Item> NextItems;
			boolean DonePyramid = false;
			
			CurrentItems = new Vector();
			CurrentItems.add(this);
			
			while (!DonePyramid)
			{
				NextItems = new Vector();
				
				for (int curItem = 0; curItem < CurrentItems.size(); curItem++)
				{
					Item thisItem = CurrentItems.get(curItem);
					AllRequiredItems.add(thisItem);
					
					for (int NewItem = 0; NewItem < thisItem.BuildsFrom.size(); NewItem++)
					{
						NextItems.add(LeagueOfYokos.getItemById(thisItem.BuildsFrom.get(NewItem)));
					}
				}
				
				if (NextItems.isEmpty())
				{
					DonePyramid = true;
				}
				else
				{
					CurrentItems = NextItems;
					NextItems = null;
				}
			}
			
			int ret = 0;
			
			for (int i = 0; i < AllRequiredItems.size(); i++)
			{
				ret += AllRequiredItems.get(i).Cost;
			}
			
			return ret;
		}
		else
		{
			return Cost;
		}
	}
}