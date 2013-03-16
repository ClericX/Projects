package league.of.yokos;

/**
 *
 * A gradual movement path handler that can get from one point to the other gradually
 * and interpolate points between, given elapsed time periods.
 * 
 */
public class GradualMovementPath
{
	public float StartX;
	public float StartY;
	public float CurX;
	public float CurY;
	public float EndX;
	public float EndY;
	public boolean AlreadyReachedTargetX;
	public boolean AlreadyReachedTargetY;
	public boolean StartSet;
	public boolean EndSet;
	
	public GradualMovementPath()
	{
		reset();
	}
	
	public void reset()
	{
		StartX = 0.0f;
		StartY = 0.0f;
		CurX = 0.0f;
		CurY = 0.0f;
		EndX = 0.0f;
		EndY = 0.0f;
		AlreadyReachedTargetX = false;
		AlreadyReachedTargetY = false;
		StartSet = false;
		EndSet = false;
	}
	
	public boolean isSetup()
	{
		return StartSet && EndSet;
	}
	
	public void setStartPoint(float sX, float sY)
	{
		StartX = sX;
		StartY = sY;
		CurX = StartX;
		CurY = StartY;
		StartSet = true;
	}
	
	public void setEndPoint(float eX, float eY)
	{
		EndX = eX;
		EndY = eY;
		EndSet = true;
	}
	
	public void Move(float Speed, long ElapsedTime)
	{
		float RawChange = (float)(Speed * ((float) ElapsedTime / 1000.0f));
		float PercentageChange = RawChange / Frost.Distance(CurX, CurY, EndX, EndY);
		
		if (!ReachedX())
		{
			CurX = Frost.LinearInterpolate(CurX, EndX, PercentageChange);
		}
		
		if (!ReachedY())
		{
			CurY = Frost.LinearInterpolate(CurY, EndY, PercentageChange);
		}
	}
	
	public boolean ReachedX()
	{
		if (AlreadyReachedTargetX)
		{
			return true;
		}
		else
		{
			boolean Potential = (EndX < StartX ? CurX < EndX : CurX > EndX);
			
			if (Potential)
			{
				AlreadyReachedTargetX = true;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	public boolean ReachedY()
	{
		if (AlreadyReachedTargetY)
		{
			return true;
		}
		else
		{
			boolean Potential = (EndY < StartY ? CurY < EndY : CurY > EndY);
			
			if (Potential)
			{
				AlreadyReachedTargetY = true;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	public boolean isCompleted()
	{
		return AlreadyReachedTargetX && AlreadyReachedTargetY;
	}
}