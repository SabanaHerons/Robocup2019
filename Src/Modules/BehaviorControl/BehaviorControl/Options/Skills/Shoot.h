option(Shoot,(Vector2f) (Vector2f(0,0)) target)
{
  Vector2f relativeBall =theRobotPose.inversePose*theLibCodeRelease.ball;
  common_transition{  
    if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
  }

  initial_state(initial)
  {
    transition
    {
      goto goForBall;
    }
    action
    {
      Stand();
    }
  }

  aborted_state(searchForBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 300){
        goto goForBall;
      }
    }
    action
    {
      TrackBall();
    }
  }

  state(goForBall)
  {
    transition
    {
      if(relativeBall.norm() < theBehaviorParameters.distanceToGoShoot && theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f))
        goto shoot;
    }
    action
    {
      DynamicLook();
      theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.ball),Pose2f(0.6f,0.6f,0.6f),false,true);
    }
  }

  state(shoot){
    transition{
      if(action_done){
        goto completed;
      }

       if(relativeBall.norm() > (theBehaviorParameters.distanceToGoShoot+100.f)|| action_aborted || !theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f))
          goto goForBall;
    }
    action{
      
      //Si no tiene target definido
      if(target.x()==0 && target.y()==0){
        //En caso de no encontrar aliados intenta disparar al arco
        target=theLibCodeRelease.whereToShoot;
        if(!theLibCodeRelease.shootToGoal){
          target=theLibCodeRelease.bestTeammateForPass();
        }
      }
      //target =Vector2f(target.x(), target.y());
      Vector2f relTarget= theRobotPose.inversePose*target;
      ShootAt(target,false);
      LookBallTarget(relTarget);
      
    }
  }
  target_state(completed){
    transition{
      goto initial;
    }
  }
}