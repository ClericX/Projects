/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package league.of.yokos;

/**
 *
 * A container class for skill information which is used within skill instances to make calculations.
 * 
 */
public class SkillInfo
{
	public int ID;
	public String Name;
	public String Description;
	public float Cooldown[];
	public float CastTime;
	public float Duration;
	public long Range;
	public long Radius;
	public int HPCost[];
	public int MPCost[];
	public int PhysicalDamage[];
	public int MagicDamage[];
	public int TrueDamage[];
	public float ADRatio;
	public float APRatio;
	public boolean CanMoveWhileCasting;
	public boolean RequiresTargetEntity;
	public boolean RequiresTargetLocation;
	
	public SkillInfo()
	{
		ID = 0;
		Name = "";
		Description = "";
		Cooldown = new float[5];
		CastTime = 0;
		Duration = 0;
		Range = 0;
		Radius = 0;
		HPCost = new int[5];
		MPCost = new int[5];
		PhysicalDamage = new int[5];
		MagicDamage = new int[5];
		TrueDamage = new int[5];
		ADRatio = 0;
		APRatio = 0;
		CanMoveWhileCasting = false;
		RequiresTargetEntity = false;
		RequiresTargetLocation = false;
	}
}