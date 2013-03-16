package league.of.yokos;

import java.awt.Point;
import java.awt.Rectangle;
import java.util.List;
import java.util.Vector;

/**
 * Contains attributes and functionality that relate to an entity within the game that is capable of taking and/or receiving damage
 * at any given point in time.
 */
public class FightingEntity
{
	public enum Stat
	{
		/** A constant defining the Level stat. **/
		LEVEL,
		/** A constant defining the HP stat. **/
		HP,
		/** A constant defining the maximum HP stat. **/
		MAX_HP,
		/** A constant defining the MP stat. **/
		MP,
		/** A constant defining the maximum MP stat. **/
		MAX_MP,
		/** A constant defining the HP regeneration stat. **/
		HP_REGEN,
		/** A constant defining the MP regeneration stat. **/
		MP_REGEN,
		/** A constant defining the armor stat. **/
		ARMOR,
		/** A constant defining the magic resistance stat. **/
		MAGIC_RESIST,
		/** A constant defining the armor penetration stat. **/
		ARMOR_PENETRATION,
		/** A constant defining the magic penetration stat. **/
		MAGIC_PENETRATION,
		/** A constant defining the attack damage stat. **/
		ATTACK_DAMAGE,
		/** A constant defining the ability power stat. **/
		ABILITY_POWER,
		/** A constant defining the attack speed stat. **/
		ATTACK_SPEED,
		/** A constant defining the life steal stat. **/
		LIFE_STEAL,
		/** A constant defining the spell vamp stat. **/
		SPELL_VAMP,
		/** A constant defining the critical chance stat. **/
		CRITICAL_CHANCE,
		/** A constant defining the cooldown reduction stat. **/
		COOLDOWN_REDUCTION,
		/** A constant defining the tenacity stat. **/
		TENACITY,
		/** A constant defining the movement speed stat. **/
		MOVEMENT_SPEED,
		/** A constant defining the auto attack range stat. **/
		AUTO_ATTACK_RANGE,
		/** A constant defining the vision radius stat. **/
		VISION_RADIUS,
		/** A constant defining the gold per 10 second stat. **/
		GOLD_PER_10
	}
	
	/** A constant defining neutral-side entities, mainly for jungle creeps. **/
	public static final int NEUTRAL_SIDE = 0;
	/** A constant defining the blue side. **/
	public static final int BLUE_SIDE = 1;
	/** A constant defining the purple side. **/
	public static final int PURPLE_SIDE = 2;
	
	/** Stores which side of the game the entity is on. **/
	public int Side;
	/** The X coordinate of the entity. **/
	public float X;
	/** The Y coordinate of the entity. **/
	public float Y;
	/** The level of the entity. **/
	public int Level;
	/** The amount of hit points the entity currently has. **/
	public float HP;
	/** The base hit points that the entity has. **/
	public float BaseHP;
	/** The additional hit points (from items or buffs, for example) that the entity has. **/
	public float ExtraHP;
	/** The maximum amount of hit points the entity has. **/
	public float MaxHP;
	/** The amount of mana points the entity currently has. **/
	public float MP;
	/** The base mana points that the entity has. **/
	public float BaseMP;
	/** The additional mana points (from items or buffs, for example) that the entity has. **/
	public float ExtraMP;
	/** The maximum amount of mana points the entity has. **/
	public float MaxMP;
	/** The amount of EXP this entity currently has. **/
	public int EXP;
	/** The HP regeneration per 5 seconds this entity has. **/
	public int HPRegen;
	/** The MP regeneration per 5 seconds this entity has. **/
	public int MPRegen;
	/** The amount of armor this entity has. This reduces physical damage. **/
	public int Armor;
	/** The amount of magic resist this entity has. This reduces magic damage. **/
	public int MagicResist;
	/** The amount of armor penetration this entity has. This amplifies physical damage. **/
	public int ArmorPenetration;
	/** The amount of magic penetration this entity has. This amplifies magic damage. **/
	public int MagicPenetration;
	/** The amount of attack damage this entity has. This increases physical damage. **/
	public int AttackDamage;
	/** The amount of ability power this entity has. This increases magic damage. **/
	public int AbilityPower;
	/** The attack speed of the unit, in number of attacks per second. Maximum of 2.5. **/
	public float AttackSpeed;
	/** The percentage of life steal this unit has. Life steal heals a percentage of physical damage dealt back to the damage dealer. **/
	public int LifeStealPercent;
	/** The percentage of spell vamp this unit has. Spell vamp heals a percentage of magic damage dealt back to the damage dealer. **/
	public int SpellVampPercent;
	/** The percentage of critical chance for auto attacks on the entity. Maximum of 100%. **/
	public int CriticalChance;
	/** The percentage of cooldown reduction applied on the entity. Maximum of 40%. **/
	public int CooldownReduction;
	/** The amount of tenacity (CC reduction) the entity has. **/
	public int Tenacity;
	/** The movement speed of the unit, in pixels per second. The higher the number, the faster movement across the map is. **/
	public float MovementSpeed;
	/** The auto attack range (radius) that the entity has. The larger the number, the further away auto attacks can be launched from. **/
	public int AutoAttackRange;
	/** The vision radius of the entity. This clears the Fog of War near the entity within this radius for it's allied side. **/
	public int VisionRadius;
	/** The amount of gold the entity has to make purchases. **/
	public float Gold;
	/** The amount of increased gold gain (in per 10 seconds). **/
	public float GoldPer10;
	/** Whether the entity is targetable (for attacking) or not. **/
	public boolean Targetable;
	/** Whether or not the entity is in stealth or not. **/
	public boolean Stealth;
	/** The items the entity is carrying with them. There is a limit of 6 items per entity. **/
	public Vector<Item> Items;
	/** The buffs/debuffs that are currently applied to the entity. **/
	public Vector<Buff> Buffs;
	/** The current auto attack being launched from this unit (if they have performed one). **/
	public Skill AutoAttack = null;
	public Skill Skill1 = null;
	public Skill Skill2 = null;
	public Skill Skill3 = null;
	public Skill Skill4 = null;
	public Skill SelectedSkill = null;
	/** The stack of skills currently being cast/currently in effect by this entity. **/
	public Vector<Skill> ActiveSkills;
	/** The target movement path of the entity. **/
	public GradualMovementPath MovePath;
	
	/** The default constructor. This simply initializes all the values in the class to defaults, which are later modified. **/
	public FightingEntity()
	{
		Side = NEUTRAL_SIDE;
		X = 0;
		Y = 0;
		Level = 0;
		HP = 0;
		MaxHP = 0;
		MP = 0;
		MaxMP = 0;
		EXP = 0;
		HPRegen = 0;
		MPRegen = 0;
		Armor = 0;
		MagicResist = 0;
		ArmorPenetration = 0;
		MagicPenetration = 0;
		AttackDamage = 0;
		AbilityPower = 0;
		AttackSpeed = 0;
		LifeStealPercent = 0;
		CriticalChance = 0;
		CooldownReduction = 0;
		Tenacity = 0;
		MovementSpeed = 0;
		AutoAttackRange = 0;
		VisionRadius = 0;
		Gold = 0;
		GoldPer10 = 0;
		Targetable = true;
		Stealth = false;
		Items = new Vector();
		Buffs = new Vector();
		
		AutoAttack = new Skill(this, 0);
		Skill1 = new Skill(this, 1);
		Skill2 = new Skill(this, 1);
		Skill3 = new Skill(this, 1);
		Skill4 = new Skill(this, 1);
		
		ActiveSkills = new Vector();
		MovePath = new GradualMovementPath();
	}
	
	/** Determines if the passed entity is an enemy to this one. Returns false if the target is on the same side, true otherwise. **/
	public boolean IsEnemy(FightingEntity fe)
	{
		if (Side == BLUE_SIDE)
		{
			return fe.Side == PURPLE_SIDE || fe.Side == NEUTRAL_SIDE;
		}
		else if (Side == PURPLE_SIDE)
		{
			return fe.Side == BLUE_SIDE || fe.Side == NEUTRAL_SIDE;
		}
		else
		{
			return true;
		}
	}
	
	/** Returns the entity's X coordinate. **/
	public float getX()
	{
		return X;
	}
	
	/** Returns the entity's Y coordinate. **/
	public float getY()
	{
		return Y;
	}
	
	/** Sets the entity's X coordinate. **/
	public void setX(float _x)
	{
		X = _x;
	}
	
	/** Sets the entity's Y coordinate. **/
	public void setY(float _y)
	{
		Y = _y;
	}
	
	/** Convenience function to set the location of the entity to a passed point. **/
	public void setLocation(Point loc)
	{
		setLocation(loc.x, loc.y);
	}
	
	/** Convenience function to set the location of the entity on both the X and Y axis. **/
	public void setLocation(float x, float y)
	{
		setX(x);
		setY(y);
	}
	
	/** Retrieves the bounding box of the entity. **/
	public Rectangle getBoundingBox()
	{
		Rectangle ret = new Rectangle();
		
		ret.x = (int)X;
		ret.y = (int)Y;
		ret.width = 25;
		ret.height = 50;
		
		return ret;
	}
	
	/** Returns the entity's level. **/
	public int getLevel()
	{
		return Level;
	}
	
	/** Sets the entity's level. **/
	public void setLevel(int lvl)
	{
		Level = lvl;
	}
	
	/** Returns the entity's hit points. **/
	public float getHP()
	{
		return HP;
	}
	
	/** Sets the entity's hit points. Ensures the current hit points are not set above the maximum. **/
	public void setHP(float _hp)
	{
		if (_hp > getMaxHP())
		{
			HP = getMaxHP();
		}
		else if (_hp < 0)
		{
			HP = 0;
		}
		else
		{
			HP = _hp;
		}
	}
	
	/** Returns the entity's max hit points. **/
	public float getMaxHP()
	{
		return MaxHP;
	}
	
	/** Sets the entity's maximum hit points. **/
	public void setMaxHP(int _maxHP)
	{
		MaxHP = _maxHP;
	}
	
	/** Returns the entity's current mana points. **/
	public float getMP()
	{
		return MP;
	}
	
	/** Sets the entity's mana points. Ensures the current mana points are not set above the maximum. **/
	public void setMP(float _mp)
	{
		if (_mp > getMaxMP())
		{
			MP = getMaxMP();
		}
		else if (_mp < 0)
		{
			MP = 0;
		}
		else
		{
			MP = _mp;
		}
	}
	
	/** Returns the entity's maximum mana points. **/
	public float getMaxMP()
	{
		return MaxMP;
	}
	
	/** Sets the entity's maximum mana points. **/
	public void setMaxMP(float _maxMP)
	{
		MaxMP = _maxMP;
	}
	
	/** Returns the entity's current experience amount. **/
	public int getEXP()
	{
		return EXP;
	}
	
	/** Returns the amount of experience the entity currently needs to level up. **/
	public int getLevelUpEXP()
	{
		return Level * 117;
	}
	
	/** Handles gaining experience points, leveling up the entity as necessary when enough is obtained. **/
	public void gainEXP(int amount)
	{
		int NewEXP = EXP + amount;
		boolean StillLeveling = true;
		
		while (StillLeveling)
		{
			int LevelUpEXP = getLevelUpEXP();

			if (NewEXP >= LevelUpEXP)
			{
				NewEXP -= LevelUpEXP;
				LevelUp();
			}
			else
			{
				EXP = NewEXP;
				StillLeveling = false;
			}
		}
	}
	
	/** Levels up the entity, handling modification of entity attributes upon leveling up. **/
	public void LevelUp()
	{
		EXP = 0;
		Level++;
		MaxHP += (50 * Level);
		MaxMP += (30 * Level);
		HPRegen += 1;
		MPRegen += 1;
		Armor += 3;
		AttackDamage += 10;
		AttackSpeed += 0.05f;
	}
	
	/** Returns the hit point regeneration per 5 second amount of this entity. **/
	public int getHPRegen()
	{
		return HPRegen;
	}
	
	/** Sets the hit point regeneration per 5 second amount of this entity. **/
	public void setHPRegen(int _hpRegen)
	{
		HPRegen = _hpRegen;
	}
	
	/** Returns the mana point regeneration per 5 second amount of this entity. **/
	public int getMPRegen()
	{
		return MPRegen;
	}
	
	/** Sets the mana point regeneration per 5 second amount of this entity. **/
	public void setMPRegen(int _mpRegen)
	{
		MPRegen = _mpRegen;
	}
	
	/** Returns the armor of this entity. **/
	public int getArmor()
	{
		return Armor;
	}
	
	/** Sets the armor of this entity. **/
	public void setArmor(int _armor)
	{
		Armor = _armor;
	}
	
	public void useItemActive(int itemSlot)
	{
		//
	}
	
	/** Process this entity taking damage from a given Skill. **/
	public void takeDamage(Skill s)
	{
		setHP(HP - s.getTrueDamage());
		
		float ArmorMultiplier = 0.0f;
		
		if (Armor >= 0)
		{
			ArmorMultiplier = 100.0f / (100.0f + Armor);
		}
		else
		{
			ArmorMultiplier = 2 - (100.0f / (100.0f - Armor));
		}
		
		setHP(HP - (s.getPhysicalDamage() * ArmorMultiplier));
		
		float MResMultiplier = 0.0f;
		
		if (MagicResist >= 0)
		{
			MResMultiplier = 100.0f / (100.0f + MagicResist);
		}
		else
		{
			MResMultiplier = 2 - (100.0f / (100.0f - MagicResist));
		}
		
		setHP(HP - (s.getMagicDamage() * MResMultiplier));
	}
	
	/** Returns whether or not this entity has vision of a given map coordinate. **/
	public boolean hasVisionOfLocation(int x, int y)
	{
		return Frost.Distance(getX(), getY(), x, y) < VisionRadius;
	}
	
	/** Returns whether or not this entity has vision of a given map tile. **/
	public boolean hasVisionOfTile(int x, int y)
	{
		return hasVisionOfLocation(Frost.TileToWorldX(x), Frost.TileToWorldY(y));
	}
	
	public void setTargetMoveLocation(int x, int y)
	{
		MovePath.reset();
		MovePath.setStartPoint(X, Y);
		MovePath.setEndPoint(x, y);
	}
}