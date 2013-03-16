/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package frostfe;

public class Timer
{
	public static double LastTime = 0;
	public static double CurrentTime = 0;
	public static double ElapsedTime = 0;
	
	public static double GetTimeMilliseconds()
	{
		return System.nanoTime() / 1000000.0;
	}
	
	public static double GetElapsedMilliseconds()
	{
		return ElapsedTime;
	}
	
	public static void Update()
	{
		CurrentTime = GetTimeMilliseconds();
		ElapsedTime = (CurrentTime - LastTime);
		LastTime = CurrentTime;
	}
	
	public static void Initialize()
	{
		Update();
		ElapsedTime = 0;
	}
}
