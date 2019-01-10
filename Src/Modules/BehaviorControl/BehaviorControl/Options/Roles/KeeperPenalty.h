/** A test striker option without common decision */
option(KeeperPenalty)
{
	
  common_transition
  {
	if (theLibCodeRelease.between(theBallModel.estimate.position.y(), -10.f, 10.f)
    && theLibCodeRelease.between(theBallModel.estimate.velocity.x(), -1200.f, -100.f)
    && theLibCodeRelease.between(theBallModel.estimate.position.x(), 0.f, 800.f))
       goto sumo;

	if(theLibCodeRelease.between(theBallModel.estimate.position.x(), 0, 1000.f)
    && theLibCodeRelease.between(theBallModel.estimate.position.y(), 10.f, 600.f)
    && theLibCodeRelease.between(theBallModel.estimate.velocity.x(), -2000.f, -5.f))
        goto groundPunchLeft;


	if(theLibCodeRelease.between(theBallModel.estimate.position.x(), 0, 1000.f)
    && theLibCodeRelease.between(theBallModel.estimate.position.y(), -600.f, -10.f)
    && theLibCodeRelease.between(theBallModel.estimate.velocity.x(), -2000.f, -5.f))
        goto groundPunchRight;

 }
  initial_state(start)
  {
    transition
    {

    }
    action
    {
      LookForward();
      SpecialAction(SpecialActionRequest::sitDown);
    }
  }
  
    state(sumo)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
      {
        goto start;
      }
    }
    action
    {
        SpecialAction(SpecialActionRequest::sumo);
        
    }
  }

state(groundPunchLeft)
 {
   transition
   {
     if(state_time > 10 && action_done)
     {
       goto start;
     }
   }
   action
   {
    SpecialAction(SpecialActionRequest::diveLeft);
   }
 }
  
  state(groundPunchRight)
 {
   transition
   {
     if(state_time > 10 && action_done)
     {
       goto start;
     }
   }
   action
   {
    SpecialAction(SpecialActionRequest::diveRight);
   }
 }
}
