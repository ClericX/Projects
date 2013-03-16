package com.wizardskettle.dungeonblazer;

import java.util.ArrayList;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

public class GameView extends SurfaceView
{
	//Properties
	private SurfaceHolder holder;
	private GameLoopThread gameLoopThread;

	//Constructor
	public GameView(Context context, AttributeSet attrs)
	{
		super(context, attrs);
		
		//The Main Game Thread
		gameLoopThread = new GameLoopThread(this);
		
		//Canvas Holder
		holder = getHolder();
		holder.addCallback(new Callback() {
			public void surfaceDestroyed(SurfaceHolder holder) {
				boolean retry = true;
				gameLoopThread.setRunning(false);
				while(retry){
					try{
						gameLoopThread.join();
						retry = false;
					} catch(InterruptedException e){}
				}
			}
			
            public void surfaceCreated(SurfaceHolder holder) {
            	gameLoopThread.setRunning(true);
            	try{
            		gameLoopThread.start();
            	}catch(IllegalThreadStateException e){}
            }
            
            public void surfaceChanged(SurfaceHolder holder, int format,
            int width, int height) {
            }
		});
		
		Game.gameView = this;
		
		Game.Init();
	}
	
	//Draw Function
	@Override
	protected void onDraw(Canvas mainCanvas)
	{
		Game.Draw(mainCanvas);
	}
	
	//Override size of view (Ensure 5:3 dimensions)
	@Override
	protected void onMeasure(int width, int height)
	{
		if(MeasureSpec.getSize(width)*Game.gameSizeY > MeasureSpec.getSize(height)*Game.gameSizeX)
		{
			height = MeasureSpec.getSize(height);
			width = MeasureSpec.getSize(height)*Game.gameSizeX/Game.gameSizeY;
		}
		else
		{
			width = MeasureSpec.getSize(width);
			height = MeasureSpec.getSize(width)*Game.gameSizeY/Game.gameSizeX;
		}
		setMeasuredDimension(width,height);
	}

	//Touch Listener
	@Override
	public boolean onTouchEvent(MotionEvent evt)
	{
		Game.ScreenTouched(evt);
		return true;
		/*
		else
		{
			return super.onTouchEvent(evt);
		}
		*/
	}
}