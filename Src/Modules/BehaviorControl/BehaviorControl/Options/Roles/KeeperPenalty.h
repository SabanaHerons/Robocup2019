//valentina's PenaltyKeeper
option(KeeperPenalty)
{
	
	initial_state(start)
	{
		transition
		{
		  if (state_time > 1000)
			goto lookBall;
		}
			action
		{
		  HeadControlMode(HeadControl::lookForward);
		}
	}

	state(lookBall)
	{
		transition
		{
		  if (-100 > theBallModel.estimate.position.y() && theBallModel.estimate.velocity.x() < -90)
		  goto jumpRight;
		  if (100 < theBallModel.estimate.position.y() && theBallModel.estimate.velocity.x() < -90)
		  goto jumpLeft;
		  if (100 > theBallModel.estimate.position.y() && -100 < theBallModel.estimate.position.y()
		  && theBallModel.estimate.velocity.x() < -90)
		  goto goDown;
		}
			action
		{
		  theHeadControlMode = HeadControl::lookAtBall;
		  SpecialAction(SpecialActionRequest::sitDown);
		}
	}
	state(jumpRight)
	{
		transition
		{

		}
		action
		{
		  SpecialAction(SpecialActionRequest::rightDive);
		}
	}
	state(jumpLeft)
	{
		transition
		{

		}
		action
		{
		  SpecialAction(SpecialActionRequest::leftDive);
		}
	}
	state(goDown)
	{
		transition
		{

		}
		action
		{
		  SpecialAction(SpecialActionRequest::preventBall);
		}
	}
}
