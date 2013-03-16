package com.wizardskettle.dungeonblazer;

import java.util.ArrayList;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.view.MotionEvent;

public class Game
{
	public static GameView gameView;
	
	// Constants
	// Game's Screen Size
	public static final int gameSizeX = 266;
	public static final int gameSizeY = 160;

	// Tile Sizes
	public static final int TILE_SIZE_X = 25;
	public static final int TILE_SIZE_Y = 25;
	public static final int TILE_DISP_X = 9;
	public static final int TILE_DISP_Y = 9;
	public static final int TILE_DISP_HEIGHT = 11;
	public static final int TILE_HORZ_OFFSET = 20; // Width of a second tile (horizontally) subtract the previous tile's intersection with it
	public static final int TILE_VERT_OFFSET = 4; // Height of a second tile (vertically) subtract the previous tile's intersection with it
	public static final int TILE_PART_DISTANCE = 9;
	public static final int MAX_TILES_ON_SCREEN_X = (gameSizeX/TILE_DISP_X)+1;
	public static final int MAX_TILES_ON_SCREEN_Y = (gameSizeY/TILE_DISP_Y)+1;

	public static final int UNIT_PART_DISTANCE = 12; //Amount of pixels unit will be drawn lower to fit nicely on a tile.
	
	// Touch Listener; Starting Point of Drag
	public static Point startPosition = new Point();

	// Map
	public static ArrayList<ArrayList<Tile>> map = new ArrayList<ArrayList<Tile>>();

	// Map's Position on the screen
	public static int mapPosX = 0;
	public static int mapPosY = 0;
	public static int mapUpdateX = 0;
	public static int mapUpdateY = 0;

	// Map Size
	public static int MapWidth = 5;
	public static int MapHeight = 5;

	// Graphics
	public static Bitmap tileBMP[][] = new Bitmap[16][3];
	public static Paint text = new Paint();
		
	// Units
	public static ArrayList<Unit> units = new ArrayList<Unit>();
	// Move Ticker
	public static int moveTicker;
	
	public static void Init()
	{
		// Loading graphical resources
		LoadTiles();
		
		// Map Initialization
		Tile defaultTile;
		for(int i=0; i < MapWidth; i++){
			map.add(new ArrayList<Tile>());
			for(int i2=0; i2 < MapHeight; i2++){
				defaultTile = new Tile(0);
				map.get(i).add(defaultTile);
			}
		}
		
		// Units initialisation
		Unit jacob = new Unit(0,0);
		units.add(jacob);
		jacob.img = BitmapFactory.decodeResource(gameView.getResources(), R.drawable.character);
		moveTicker = 0;
	}
	
	public static void ScreenTouched(MotionEvent evt)
	{
		if(evt.getAction() == MotionEvent.ACTION_DOWN)
		{
			startPosition.x = (int)evt.getX();
			startPosition.y = (int)evt.getY();
		}
		if(evt.getAction() == MotionEvent.ACTION_MOVE)
		{
			int x = 0,y = 0;
			x = (int)evt.getX() - startPosition.x;
			y = (int)evt.getY() - startPosition.y;
			Game.mapPosX += x/2;
			Game.mapPosY += y/2;
			
			startPosition.x = (int)evt.getX();
			startPosition.y = (int)evt.getY();
			
			// This is what causes a redraw every touch event.
			//invalidate();
		}
	}
	
	// Gets a tile at a certain location.
	public static Tile getTile(int x, int z)
	{
		return map.get(x).get(z);
	}
	
	// Checks to see if a sprite is on the screen
	public static boolean onScreenCheck(int x, int y, int sizeX, int sizeY)
	{
		return !(x < -(sizeX * 2) || x > gameSizeX || y < -(sizeY * 2) || y > gameSizeY);
	}
	
	// Loads tiles into tileBMP array
	public static void LoadTiles()
	{
		Canvas tileCan;
		// Splitting of sprites from sprite sheet
		Rect src = new Rect(0,0,TILE_SIZE_X,TILE_SIZE_Y);
		Rect dst = new Rect(0,0,TILE_SIZE_X,TILE_SIZE_Y);
		
		// Tiles Setup
		for(int tRow=0; tRow < tileBMP.length; tRow++)
		{
			for(int tCol=0; tCol < tileBMP[tRow].length; tCol++)
			{
				src = new Rect(tRow*TILE_SIZE_X,tCol*TILE_SIZE_X,(tRow+1)*TILE_SIZE_X,(tCol+1)*TILE_SIZE_Y);
				tileBMP[tRow][tCol] = Bitmap.createBitmap(TILE_SIZE_X,TILE_SIZE_Y,Bitmap.Config.ARGB_8888);
				tileCan = new Canvas(tileBMP[tRow][tCol]);
				tileCan.drawBitmap(BitmapFactory.decodeResource(gameView.getResources(), R.drawable.tiles), src, dst, null);
			}
		}
	}
	
	// Drawing Functions
	// Draws Units to the screen
	public static void DrawUnit(Canvas mainCanvas, Unit unit, int x, int z, int y)
	{
		// Calculates how to center the unit on the tile
		int unit_x_offset = (TILE_SIZE_X+TILE_PART_DISTANCE-unit.img.getWidth())/2;
		// Calculates absolute unit positions on the screen
		int AbsX = mapUpdateX+(z*TILE_HORZ_OFFSET)+(x*TILE_DISP_Y)+unit_x_offset;
		int AbsY = mapUpdateY+(x*TILE_DISP_Y)-(z*TILE_VERT_OFFSET)-(y*TILE_DISP_HEIGHT)-unit.img.getHeight()+UNIT_PART_DISTANCE;
		
		// Don't draw if it is off the screen
		if(!onScreenCheck(AbsX, AbsY, unit.img.getWidth(), unit.img.getHeight())){
			return;
		}
		mainCanvas.drawBitmap(unit.img, AbsX, AbsY, null);
	}
	
	// Draws Tiles to the screen
	public static void DrawTile(Canvas mainCanvas, int x, int z, int y, boolean filler)
	{
		// Calculate absolute tile positions on the screen based on camera offset and tile placement by given coordinate parameters.
		int AbsTileX = mapUpdateX+(z*TILE_HORZ_OFFSET)+(x*TILE_DISP_Y);
		int AbsTileY = mapUpdateY+(x*TILE_DISP_Y)-(z*TILE_VERT_OFFSET)-(y*TILE_DISP_HEIGHT);
		
		// Don't bother drawing the tile if it's off screen.
		if(!onScreenCheck(AbsTileX, AbsTileY, TILE_SIZE_X, TILE_SIZE_Y)){
			return;
		}
		
		// Chooses what kind of tile to be drawn
		// Constant (Outter edge tile height)
		int outTile = -1;
		
		// Paint to be used on tile drawing for transparency
		Paint paint = new Paint();
		
		// Type of tile for part 1 of tile and part 2 of tile
		int type1 = 0;
		int type2 = 0;
		int lTileH = 0;
		int rTileH = 0;
		int uTileH = 0;
		int dTileH = 0;
		
		// Neighboring tile height check(If invalid, set to outter tile height)
		try
		{
			lTileH = map.get(x).get(z-1).getHeight();
		}
		catch(IndexOutOfBoundsException e)
		{
			lTileH = outTile;
		}
		try
		{
			dTileH = map.get(x+1).get(z).getHeight();
		}
		catch(IndexOutOfBoundsException e)
		{
			dTileH = outTile;
		}
		if(filler == false)
		{
			try
			{
				rTileH = map.get(x).get(z+1).getHeight();
				
			}
			catch(IndexOutOfBoundsException e)
			{
				rTileH = outTile;
			}
			try
			{
				uTileH = map.get(x-1).get(z).getHeight();
			}
			catch(IndexOutOfBoundsException e)
			{
				uTileH = outTile;
			}
		}
		else 
		{
			//Filler tile types
			//part 1
			if(lTileH < y-1)
			{
				type1 = 2;
			}
			else if(lTileH == y-1)
			{
				type1 = 4;
			}
			else
			{
				type1 = 2;
			}
			//part 2
			if(dTileH < y-1)
			{
				type2 = 3;
			}
			else if(dTileH == y-1)
			{
				type2 = 5;
			}
			else
			{
				type2 = 3;
			}
			if(type1 != -1 && type2 != -1)
			{
				mainCanvas.drawBitmap(tileBMP[type1][(map.get(x).get(z).getRn1()+y)%3], AbsTileX, AbsTileY, paint);
				mainCanvas.drawBitmap(tileBMP[type2][(map.get(x).get(z).getRn1()+y)%3], AbsTileX + TILE_PART_DISTANCE, AbsTileY, paint);
			}
			return;
		}
		
		// Main tile types
		// part 1
		if(lTileH < y)
		{
			if(lTileH < y-1 && uTileH < y)
			{
				type1 = 12;
			}
			else if(lTileH < y-1 && uTileH >= y)
			{
				type1 = 6;
			}
			if(lTileH == y-1 && uTileH < y)
			{
				type1 = 14;
			}
			else if(lTileH == y-1 && uTileH >= y)
			{
				type1 = 8;
			}
		}
		else 
		{
			if(uTileH < y)
			{
				type1 = 10;
			} 
			else 
			{
				type1 =	 0;
			}
		}
		//part 2
		if(dTileH < y){
			if(dTileH < y-1 && rTileH < y)
			{
				type2 = 13;
			} 
			else if(dTileH < y-1 && rTileH >= y)
			{
				type2 = 7;
			} 
			else if(dTileH == y-1 && rTileH < y)
			{
				type2 = 15;
			} 
			else if(dTileH == y-1 && rTileH >= y)
			{
				type2 = 9;
			}
		}
		else 
		{
			if(rTileH < y)
			{
				type2 = 11;
			}
			else 
			{
				type2 = 1;
			}
		}
		
		// Draw the tile parts where they need to be.
		mainCanvas.drawBitmap(tileBMP[type1][getTile(x, z).getRn1()], AbsTileX, AbsTileY, paint);
		mainCanvas.drawBitmap(tileBMP[type2][getTile(x, z).getRn1()], AbsTileX + TILE_PART_DISTANCE, AbsTileY, paint);
		
		// Checks for units to draw/if they're above set tile's transparency to 50%
		// Draws Unit
		if(getTile(x, z).getContainedUnit() != null)
		{
			DrawUnit(mainCanvas, getTile(x, z).getContainedUnit(), x, z, y);
		}
	}
	
	public static void Draw(Canvas mainCanvas)
	{
		mapUpdateX = mapPosX;
		mapUpdateY = mapPosY;
		
		// BG Color
		mainCanvas.drawColor(0xFF4d7419);
		// Text
		text.setColor(Color.BLACK);
		text.setStyle(Paint.Style.STROKE);
		text.setStrokeWidth(1);
		text.setTextSize(20);
		
		// Draws Game Screen	
		// Draw all the tiles.
		for (int TileDrawX = 0; TileDrawX < MapHeight; TileDrawX++) // The x-axis is the "up-down" kind of axis. If you exclude an X, you exclude a whole row.
		{
			for (int TileDrawZ = MapWidth - 1; TileDrawZ >= 0; TileDrawZ--) // The z-axis is the "left-right" depth axis. Individual tiles on a row lie on the z.
			{
				int TileHeight = map.get(TileDrawX).get(TileDrawZ).getHeight();
				// Fill all the tile area underneath tiles from 0 to the height -1.
				for (int HeightFiller = 0; HeightFiller < TileHeight; HeightFiller++)
				{
					DrawTile(mainCanvas, TileDrawX, TileDrawZ, HeightFiller, true);
				}
				// Draws top tile
				DrawTile(mainCanvas, TileDrawX, TileDrawZ, TileHeight, false);
			}
		}
		
		// Debugs
		if(moveTicker > 24){
			moveTicker = 0;
			int rand;
			
			while(true)
			{
				rand = (int)(Math.random()*4);
				if(units.get(0).getX()-1 > 1 && rand == 0)
				{
					units.get(0).setX(units.get(0).getX()-1);
					return;
				} 
				else if(units.get(0).getX()+1 < MapWidth && rand == 1)
				{
					units.get(0).setX(units.get(0).getX()+1);
					return;
				}
				else if(units.get(0).getZ()-1 > 1 && rand == 2)
				{
					units.get(0).setZ(units.get(0).getZ()-1);
					return;
				}
				else if(units.get(0).getZ()+1 < MapHeight && rand == 3)
				{
					units.get(0).setZ(units.get(0).getZ()+1);
					return;
				}
			}
		}
		moveTicker++;
	}
}