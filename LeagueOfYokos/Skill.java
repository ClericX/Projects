package league.of.yokos;

import java.awt.Color;
import java.awt.Point;
import java.awt.Rectangle;

public class Skill
{
	public SkillInfo info;
	
	public int CurrentLevel;
	public float CurrentCooldownTime;
	public long CurrentCastTime;
	public boolean FinishedCast;
	public long DurationTime;
	public boolean FinishedDuration;
	
	public GradualMovementPath MovePath;
	public FightingEntity Sender;
	public Point TargetLocation;
	public FightingEntity TargetEntity;
	public float OriginalDistance;
	
	public Skill(FightingEntity owner, int id)
	{
		info = LeagueOfYokos.getSkillInfoById(id);
		
		if (info == null)
		{
			System.out.println("Tried to use a skill that wasn't loaded! ID: " + id);
		}
		
		CurrentLevel = 0;
		CurrentCooldownTime = 0;
		CurrentCastTime = 0;
		FinishedCast = false;
		DurationTime = 0;
		FinishedDuration = true;
		
		MovePath = new GradualMovementPath();
		Sender = owner;
		TargetLocation = null;
		TargetEntity = null;
		OriginalDistance = 0;
	}
	
	/** Process this entity initiating a Skill cast of some sort. **/
	public boolean cast(Point targetLocation, FightingEntity targetEntity)
	{
		if (info.RequiresTargetEntity && targetEntity == null || info.RequiresTargetLocation && targetLocation == null || !canCast())
		{
			return false;
		}
		
		if (info.RequiresTargetLocation)
		{
			TargetLocation = targetLocation;
		}
		
		if (info.RequiresTargetEntity)
		{
			TargetEntity = targetEntity;
			
			Rectangle bb = Sender.getBoundingBox();
			// TODO: Do something about specializing this for auto attacks. Right now AA's have 1000 radius.
			if (Frost.Distance(bb.x + (bb.width / 2), bb.y + (bb.height / 2), TargetEntity.X, TargetEntity.Y) > info.Radius)
			{
				return false;
			}
		}
		
		boolean ret = false;
		
		switch (info.ID)
		{
			case 0:
			{
				MovePath.reset();
				MovePath.setStartPoint(Sender.X, Sender.Y);
				MovePath.setEndPoint(TargetEntity.X, TargetEntity.Y);

				OriginalDistance = Frost.Distance(MovePath.CurX, MovePath.CurY, MovePath.EndX, MovePath.EndY);
				ret = true;
			}
			break;
				
			case 1:
			{
				MovePath.reset();
				MovePath.setStartPoint(Sender.X, Sender.Y);
				MovePath.setEndPoint(TargetEntity.X, TargetEntity.Y);

				OriginalDistance = Frost.Distance(MovePath.CurX, MovePath.CurY, MovePath.EndX, MovePath.EndY);
				ret = true;
			}
			break;
		}
		
		if (ret)
		{
			Sender.setHP(Sender.HP - getHPCost());
			Sender.setMP(Sender.MP - getMPCost());
			CurrentCooldownTime = getCooldown();
			
			FinishedDuration = false;
			Sender.ActiveSkills.add(this);
			
			if (!info.CanMoveWhileCasting)
			{
				Sender.MovePath.reset();
			}
		}
		
		return ret;
	}
	
	/** Process various Skill "tick" procedures. **/
	public void tick()
	{
		// TODO: add a check for isFinishedExecution() sort of thing.
		if (FinishedDuration)
		{
			System.out.println("Useless skill tick.");
			return;
		}
		
		switch (info.ID)
		{
			case 0:
			{
				MovePath.setEndPoint(TargetEntity.X, TargetEntity.Y);
				MovePath.Move(Sender.AttackSpeed * OriginalDistance, LeagueOfYokos.ElapsedTimeThisFrame);

				LeagueOfYokos.BackBuffer.setColor(Color.orange);
				LeagueOfYokos.FillRect((int)MovePath.CurX, (int)MovePath.CurY, 5, 5);

				if (MovePath.isCompleted())
				{
					TargetEntity.takeDamage(this);
					
					Sender.ActiveSkills.remove(this);
					FinishedDuration = true;
				}
			}
			break;
				
			case 1:
			{
				MovePath.setEndPoint(TargetEntity.X, TargetEntity.Y);
				MovePath.Move(5 * OriginalDistance, LeagueOfYokos.ElapsedTimeThisFrame);

				Sender.setLocation(MovePath.CurX, MovePath.CurY);

				if (MovePath.isCompleted() || Sender.getBoundingBox().intersects(TargetEntity.getBoundingBox()))
				{
					TargetEntity.takeDamage(this);
					
					Sender.ActiveSkills.remove(this);
					FinishedDuration = true;
				}
			}
			break;
		}
	}
	
	public boolean canCast()
	{
		return (CurrentCooldownTime == 0 && Sender.MP >= getMPCost() && Sender.HP > getHPCost());
	}
	
	public float getPhysicalDamage()
	{
		return info.PhysicalDamage[CurrentLevel] + (info.ADRatio * Sender.AttackDamage);
	}
	
	public float getMagicDamage()
	{
		return info.MagicDamage[CurrentLevel] + (info.APRatio * Sender.AbilityPower);
	}
	
	public float getTrueDamage()
	{
		return info.TrueDamage[CurrentLevel];
	}
	
	public float getCooldown()
	{
		return info.Cooldown[CurrentLevel];
	}
	
	public void lowerCooldown(float amount)
	{
		if (CurrentCooldownTime > 0)
		{
			CurrentCooldownTime -= amount;

			if (CurrentCooldownTime < 0)
			{
				CurrentCooldownTime = 0;
			}
		}
	}
	
	public int getHPCost()
	{
		return info.HPCost[CurrentLevel];
	}
	
	public int getMPCost()
	{
		return info.MPCost[CurrentLevel];
	}
}