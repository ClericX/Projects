/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package league.of.yokos;

public class StatusModifier
{
	public FightingEntity.Stat TargetStat; // The stat that the modifier modifies. Picked from an enumerated list.
	public int ChangeAmountInt; // The value change if it is stored as an int.
	public float ChangeAmountFloat; // The value change if it is stored as a float.
	public boolean PercentageChange; // True = % change, False = flat change.
	
	public StatusModifier(FightingEntity.Stat targetStat, int Change)
	{
		TargetStat = targetStat;
		ChangeAmountInt = Change;
	}
	
	public StatusModifier(FightingEntity.Stat targetStat, int Change, boolean Percent)
	{
		TargetStat = targetStat;
		ChangeAmountInt = Change;
		PercentageChange = Percent;
	}
	
	public StatusModifier(FightingEntity.Stat targetStat, float Change)
	{
		TargetStat = targetStat;
		ChangeAmountFloat = Change;
	}
}
