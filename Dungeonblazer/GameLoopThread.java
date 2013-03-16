package com.wizardskettle.dungeonblazer;

import android.graphics.Canvas;

public class GameLoopThread extends Thread
{
	static final long FPS = 24;
	
	private GameView view;
	private boolean running = false;
	
	public GameLoopThread(GameView view)
	{
		this.view = view;
		view.getHolder().setFixedSize(Game.gameSizeX,Game.gameSizeY);
		view.getHolder().setFormat(0xfffffffd);
	}
	
	public void setRunning(boolean run)
	{
		running = run;
	}
	
	@Override
	public void run()
	{
		long ticksPS = 1000 / FPS;
		long startTime;
		long sleepTime;
		while(running)
		{
			Canvas c = null;
			startTime = System.currentTimeMillis();
			try
			{
				c = view.getHolder().lockCanvas();
				synchronized(view.getHolder())
				{
					view.onDraw(c);
				}
			}
			finally
			{
				if (c != null)
				{
					view.getHolder().unlockCanvasAndPost(c);
				}
			}
			
			sleepTime = ticksPS-(System.currentTimeMillis() - startTime);
			
			try
			{
				if(sleepTime > 0)
				{
					sleep(sleepTime);
				}
				else
				{
					sleep(10);
				}
			}
			catch(Exception e){}
		}
	}
}
