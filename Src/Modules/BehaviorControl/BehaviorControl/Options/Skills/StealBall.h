/**
Va a hacer el movimiento de robo de la pelota desde donde sea que se encuentre
*/


option(StealBall)
{

  Vector2f relativeBall =theRobotPose.inversePose*theLibCodeRelease.ball;

  initial_state(initial)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      goto goToSteal;
    }
    action{}
  }

  state(goToSteal)
    {
      transition
      {
        if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
          goto searchForBall;
        }
        if(relativeBall.norm() < theBehaviorParameters.distanceToGoShoot && theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f))
          goto steal;
      }
      action
      {
        TrackBall();
        float offset= -100.f;
        if(theLibCodeRelease.ball.y()>=0)
          offset*=-1;
        Vector2f positionForSteal = theLibCodeRelease.rotateCoordenates((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle(), Vector2f(-300.f,offset))+theLibCodeRelease.ball;
        theMotionRequest = thePathPlanner.plan(Pose2f(0.f,positionForSteal),Pose2f(0.6f,0.6f,0.6f),false,true);
      }
    }

    state(steal){
      transition{
        if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
          goto searchForBall;
        }
        if(action_done){
          goto goBack;
        }
        if(relativeBall.norm() > (theBehaviorParameters.distanceToGoShoot+100.f)|| action_aborted || !theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f)){
            goto goToSteal;
        }
      }
      action{
        //Patadita pal ladin

        bool conIzquierda= theLibCodeRelease.ball.y()>=0;
        TrackBall();
        StealBallWith(conIzquierda);
      }
    }

    target_state(goBack)
    {
        transition
        {
          if(state_time<1000)
            goto initial;
        }
        action
        {
          LookForward();
          WalkAtRelativeSpeed(Pose2f(0.f, -1.f, 0.f));
        }
    }

    aborted_state(searchForBall)
    {
      transition
      {
        if(theLibCodeRelease.timeSinceBallWasSeen < 300){
          goto goToSteal;
        }
      }
      action
      {
        TrackBall();
      }
    }
}
