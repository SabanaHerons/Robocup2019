// Put some changes when implementi two the three naos
option(Defender)
{
  initial_state(start)
  {
    transition
    {
      if(state_time > 1000)
        goto searchForBall;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      Stand();
    }
  }

  state(lookAtBall)
    {
      transition
      {
       if(theLibCodeRelease.ballInOppField){
        goto walkToTarget; 
       }
       else
       {
        goto walkToBall;
       }
      }
      action
      {
        HeadControlMode(HeadControl::lookAtBall);
        WalkToTarget(Pose2f(0.5f,0.5f,0.5f), Pose2f(theLibCodeRelease.angleToOppGoal,0.f,0.f));
      }
    }

  state(alignBehindBall)
  {
    transition
    {
      if((theBallModel.estimate.position.x() - 100.f) > 50.f && (theBallModel.estimate.position.y()) < 10.f)
      {
        goto kick;
      }
      else
      {
        goto walkToBall;
      }
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(theLibCodeRelease.angleToOppGoal, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() - 30.f));

    }
  }

  state(alignToOppGoal)
  {
    transition
    {
      if(action_done && state_time > 100)
        goto start;
    }
    action
    {
      LookAround();
      WalkToTarget(Pose2f(100.f, 100.f, 100.f),Pose2f(theLibCodeRelease.angleToOppGoal,100.f,100.f));
    }
  }

  state(kick)
  {
    transition
    {
      if (state_time > 3000 || (state_time > 10 && action_done))
      {
        goto start;
      }
    }
    action
    {
	  theMotionRequest.motion = MotionRequest::kick;
	  theMotionRequest.kickRequest.kickMotionType = KickRequest::kickForward;
    }
  }

  // TODO have to make a method that changes this walktotarget depending the defender number  (3)
  state(walkToTarget)
  {
    transition
    {
      if (state_time > 2000 || (state_time > 10 && action_done))
        goto alignToOppGoal;
    }
    action
    {
      LookAround();
      WalkToTarget(Pose2f(100.f, 100.f, 100.f),Pose2f(theLibCodeRelease.angleToOppGoal, theFieldDimensions.xPosOwnGroundline + 500.f, theFieldDimensions.yPosLeftPenaltyArea + 150.f));
    } 
  }

  state(searchForBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < theBehaviorParameters.ballNotSeenTimeOut)
        goto lookAtBall;
    }
    action
    {
      LookAround();
      Stand(); 
    }
  }

  state(walkToBall)
    {
      transition
      {
        if (state_time > 1000 || (state_time > 10 && action_done))
          goto alignBehindBall;
      }
      action
      {
        HeadControlMode(HeadControl::lookAtBall);
        WalkToTarget(Pose2f(0.5f,0.5f,0.5f), theBallModel.estimate.position);
      }
    }
}
