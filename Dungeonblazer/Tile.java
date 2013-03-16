package com.wizardskettle.dungeonblazer;

public class Tile
{
	int height;
	int rn1;
	int rn2;
	int theme;
	Unit containedUnit; // A handle to the unit on this tile.
	
	public Tile(int theme)
	{
		setTheme(theme);
		
		//Generates random tile type
		setRn1((int)(Math.random()*3));
		setRn2((int)(Math.random()*3));
		
		//Generates Height
		setHeight((int)(Math.random()*3));
	}

	public int getRn1()
	{
		return rn1;
	}

	public void setRn1(int rn)
	{
		this.rn1 = rn;
	}
	
	public int getRn2()
	{
		return rn2;
	}

	public void setRn2(int rn)
	{
		this.rn2 = rn;
	}
	
	public int getTheme()
	{
		return theme;
	}
	
	public void setTheme(int theme)
	{
		this.theme = theme;
	}

	public int getHeight()
	{
		return height;
	}

	public void setHeight(int height)
	{
		this.height = height;
	}
	
	public Unit getContainedUnit()
	{
		return containedUnit;
	}

	public void setContainedUnit(Unit containedUnit)
	{
		this.containedUnit = containedUnit;
	}
}