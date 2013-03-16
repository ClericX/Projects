/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

import java.awt.Color;
import java.awt.Point;

/**
 * A general-purpose class that contains various global application and game-related variables and functionality.
 */
public class Application
{
	// Game state variables.
	public static double RunTime;
	public static int GameState;
	public static int CurrentGameStateTime;
	public static int CurrentTeamTurn;
	
	// Team switching variables.
	public static boolean SwitchingTurn;
	public static int SwitchingTurnTime;
	public static int TeamSwitchTargetTeam;
	
	// Application constants.
	public static final int GAMESTATE_INTRO = 0;
	public static final int GAMESTATE_MAINMENU = 1;
	public static final int GAMESTATE_MAP = 2;
	public static final int GAMESTATE_BATTLE = 3;
	public static final int GAMESTATE_STATS = 4;
	public static final int GAMESTATE_GAMEOVER = 5;
	public static final int GAMESTATE_HELP = 6;
	
	public static final int TEAM_PLAYER = 1;
	public static final int TEAM_ENEMY = 2;
	
	// A general thread delay function.
	public static void Sleep(int MS)
	{
		try
		{
			Thread.sleep(MS);
		}
		catch(InterruptedException e)
		{
			System.out.println(e.toString());
		}
	}
	
	public static void Initialize()
	{
		RunTime = 0;
		GameState = 0;
		CurrentGameStateTime = 0;
		CurrentTeamTurn = 0;
		SwitchingTurn = false;
		SwitchingTurnTime = 0;
		TeamSwitchTargetTeam = 0;
	}
	
	public static void SetGameState(int NewGameState)
	{
		GameState = NewGameState;
		CurrentGameStateTime = 0;
	}
	
	public static void SwitchTurn(int NewTeamTurn)
	{
		CurrentTeamTurn = 0;
		SwitchingTurn = true;
		SwitchingTurnTime = 0;
		TeamSwitchTargetTeam = NewTeamTurn;
		
		// Refresh allied unit turn-based variables.
		for (int i = 0; i < FrostFE.AlliedUnits.size(); i++)
		{
			FrostFE.AlliedUnits.get(i).RefreshOnTurnEnd();
		}
		
		// Refresh enemy unit turn-based variables.
		for (int i = 0; i < FrostFE.EnemyUnits.size(); i++)
		{
			FrostFE.EnemyUnits.get(i).RefreshOnTurnEnd();
		}
		
		// Check for and apply effect of stat modifying tiles (HP Gain/Loss).
		for (int MapX = 0; MapX < FrostFE.Map.length; MapX++)
		{
			for (int MapY = 0; MapY < FrostFE.Map[MapX].length; MapY++)
			{
				// If a unit is standing on a fort (healing tile), heal them.
				if (FrostFE.Map[MapX][MapY].equals("buildings 35"))
				{
					if (FrostFE.GetUnitAtPos(new Point(MapX, MapY)) != null)
					{
						// TODO Maybe make an animation for the healing.
						FrostFE.GetUnitAtPos(new Point(MapX, MapY)).ModifyHP(10);
					}
				}
				// If a unit is standing on a damage tile, damage them.
				else if (FrostFE.Map[MapX][MapY].equals("basicoutdoor 143"))
				{
					if (FrostFE.GetUnitAtPos(new Point(MapX, MapY)) != null)
					{
						// TODO Maybe make an animation for the damage.
						FrostFE.GetUnitAtPos(new Point(MapX, MapY)).ModifyHP(-10);
					}
				}
			}
		}
	}
	
	public static boolean TeamSwitchOccurring()
	{
		return SwitchingTurn;
	}
	
	public static void UpdateTeamSwitchingState()
	{
		if (TeamSwitchOccurring())
		{
			SwitchingTurnTime += Timer.GetElapsedMilliseconds();

			String TargetText = null;
			Color TargetColor = null;
			switch (TeamSwitchTargetTeam)
			{
				case TEAM_PLAYER:
					TargetText = "Player Turn";
					TargetColor = Color.blue;
					break;
				case TEAM_ENEMY:
					TargetText = "Enemy Turn";
					TargetColor = Color.red;
					break;
				default:
					TargetText = "Invalid turn";
					TargetColor = Color.black;
					break;
			}
			
			Point MessageSize = TextEngine.getSize(TargetText);
			int DrawX = (FrostFE.WindowWidth / 2) - (MessageSize.x / 2);
			
			if (SwitchingTurnTime < 500)
			{
				int Movement = SwitchingTurnTime / 10;
				TargetColor = new Color(TargetColor.getRed(), TargetColor.getGreen(), TargetColor.getBlue(), (int)(((float)Movement / 50.0f) * 255));
				DrawX += (50 - Movement);
			}
			else if (SwitchingTurnTime > 1000)
			{
				int Movement = (SwitchingTurnTime - 1000) / 10;
				TargetColor = new Color(TargetColor.getRed(), TargetColor.getGreen(), TargetColor.getBlue(), 255 - (int)(((float)Movement / 50.0f) * 255));
				DrawX -= Movement;
			}
			
			FrostFE.DrawQuad(0, (FrostFE.WindowHeight / 2) - 10, FrostFE.WindowWidth, 30, new Color(0, 0, 0, Math.max(0, TargetColor.getAlpha() - 100)), null);
			TextEngine.Draw(DrawX, FrostFE.WindowHeight / 2, TargetText, TargetColor);
			
			if (SwitchingTurnTime >= 1500)
			{
				// The turn switch delay has ended.
				CurrentTeamTurn = TeamSwitchTargetTeam;
				SwitchingTurn = false;
			}
		}
	}
}