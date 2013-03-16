package com.wizardskettle.dungeonblazer;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;

public class Unit
{
	private int posX;
	private int posZ;
	Bitmap img;
	Tile LocatedTile; // A handle to the tile this unit is located on.
	
	public Unit(int x, int z)
	{
		setLocation(x, z);
	}

	//Getters and setters for position
	public int getX()
	{
		return posX;
	}

	public void setX(int posX)
	{
		setLocation(posX, this.posZ);
	}

	public int getZ()
	{
		return posZ;
	}

	public void setZ(int posZ)
	{
		setLocation(this.posX, posZ);
	}
	
	public void setLocation(int x, int z)
	{
		if (LocatedTile != null)
		{
			LocatedTile.setContainedUnit(null);
			LocatedTile = null;
		}
		
		this.posX = x;
		this.posZ = z;
		
		LocatedTile = Game.getTile(x, z);
		LocatedTile.setContainedUnit(this);
	}
}