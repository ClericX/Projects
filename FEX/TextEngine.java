/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

import java.awt.Color;
import java.awt.Point;
import java.io.File;

public class TextEngine
{
	public static Texture FontLetters[] = null;
	
	public static Texture GetLetter(char Letter)
	{
		if (FontLetters[Letter] != null)
		{
			return FontLetters[Letter];
		}
		else
		{
			return FontLetters[0];
		}
	}
	
	public static Point DrawCore(int X, int Y, String text, Color clr, boolean ActuallyDraw)
	{
		Point ret = new Point();
		
		int CurrentX = X;
		int CurrentY = Y;
		
		for (int i = 0; i < text.length(); i++)
		{
			char CurrentLetter = text.charAt(i);
			
			if (CurrentLetter == '\n')
			{
				CurrentX = X;
				CurrentY += 13;
			}
			else if (CurrentLetter == ' ')
			{
				CurrentX += 6;
			}
			else
			{
				Texture TheLetter = GetLetter(CurrentLetter);
				if (ActuallyDraw)
				{
					FrostFE.DrawQuad(CurrentX, CurrentY, TheLetter.ImageWidth, TheLetter.ImageHeight, clr, TheLetter);
				}
				CurrentX += TheLetter.ImageWidth;
			}
			
			int CurrentWidth = CurrentX - X;
			int CurrentHeight = CurrentY - Y;
			
			if (CurrentWidth > ret.x)
			{
				ret.x = CurrentWidth;
			}
			if (CurrentHeight > ret.y)
			{
				ret.y = CurrentHeight;
			}
		}
		
		return ret;
	}
	
	public static void Draw(int X, int Y, String text, Color clr)
	{
		DrawCore(X, Y, text, clr, true);
	}
	
	public static Point getSize(String text)
	{
		return DrawCore(0, 0, text, null, false);
	}
	
	public static void Initialize()
	{
		FontLetters = new Texture[256];
		
		FontLetters[0] = Texture.Load("Res/Font/NA.png");
		for (int i = 1; i < FontLetters.length; i++)
		{
			String LetterPath = "Res/Font/";
			
			if ((char)i >= 'a' && (char)i <= 'z')
			{
				LetterPath += "l";
			}
			
			if ((char)i == ':')
			{
				LetterPath += "Colon";
			}
			else if ((char)i == '.')
			{
				LetterPath += "Dot";
			}
			else if ((char)i == '?')
			{
				LetterPath += "QuestionMark";
			}
			else if ((char)i == '/')
			{
				LetterPath += "Slash";
			}
			else if ((char)i == '\\')
			{
				LetterPath += "Backslash";
			}
			else
			{
				LetterPath += Character.toString((char)i);
			}
			
			LetterPath += ".png";
			
			if (new File(LetterPath).exists())
			{
				FontLetters[i] = Texture.Load(LetterPath);
			}
		}
	}
}
