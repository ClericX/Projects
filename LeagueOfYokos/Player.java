package league.of.yokos;

public class Player
{
	public Champion getChampion()
	{
		return champ;
	}
	
	public void setChampion(Champion c)
	{
		champ = c;
	}
	
	private Champion champ;
	public String Username;
}