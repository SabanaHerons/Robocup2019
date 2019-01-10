/** A test striker option without common decision */
option(HandNoVision)
{
  initial_state(start)
  {
    transition
    {
      if(state_time > 20000)
        goto A;
    }
    action
    {
	  LookAround();
	SpecialAction(SpecialActionRequest::standHigh);
	}
      
    }
	state(A)
	{
		action
		{
			if(theRobotInfo.number == 1)
			{
				if(theTeamBallModel.position.y() > 0)
				{
				        SpecialAction(SpecialActionRequest::rightArm);
				}
				else
				{
				        SpecialAction(SpecialActionRequest::leftArm);
				}
			}
			else
			{
				LookForward();
				SpecialAction(SpecialActionRequest::standHigh);
			}
		}
	}
  
}
