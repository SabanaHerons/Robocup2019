option(Poste)
{
  initial_state(waiting)
  {
    transition
    {
      if(theBallModel.estimate.position.norm() <= 1000.f && theLibCodeRelease.timeSinceBallWasSeen < 7000)
        goto shoot;
	  if(option_time < 1000)
	        goto caminar;
    }
    action
    {
      LookAround();
      Stand();
    }
  }

  state(shoot)
  {
    transition{
      if(action_done || action_aborted){
            goto waiting;
          }
    if(theBallModel.estimate.position.norm() > 650.f || theLibCodeRelease.timeSinceBallWasSeen > 4000 || state_time>8000)
        goto waiting;

    }
    action
    {
      Shoot();
    }
  }
  /*
  //estado para postes V5, aunque no creo que haya :vvvv
  state(alPunto)
  {
    transition{
    if((theRobotPose.translation-Vector2f(theFieldDimensions.xPosOwnPenaltyMark,50.f)).norm() < 20.f)
        goto waiting;
    }
    action
    {
      theMotionRequest = thePathPlanner.plan(Pose2f(theLibCodeRelease.angleToOwnGoal,Vector2f(theFieldDimensions.xPosOwnPenaltyMark,50.f)),Pose2f(0.7f,0.7f,0.7f),false,true);

    }
  }*/
  state(caminar)
  {
    transition{
    if(state_time>3000)
        goto rotando;
    }
    action
    {
     WalkAtRelativeSpeed(Pose2f(0.f, 0.6f, 0.f));
    }
  }
  state(rotando)
  {
    transition{
    if(state_time>2000)
        goto waiting;
    }
    action
    {
     WalkAtRelativeSpeed(Pose2f(theLibCodeRelease.angleToOppGoal, 0.f, 0.f));

    }
  }


}
