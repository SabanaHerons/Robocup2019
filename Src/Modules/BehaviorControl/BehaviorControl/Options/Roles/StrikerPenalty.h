/** A test striker option without common decision */
option(StrikerPenalty)
{
  initial_state(start)
  {
    transition
    {
      if(state_time > 5000)
        goto walkToBall;
    }
    action
    {
      //HeadControlMode(HeadControl::lookForward);
      LookAround();
      Stand();
    }
  }

  state(walkToBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto searchForBall;
      if(theBallModel.estimate.position.x() < 250.f)
        goto alignBehindBall;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), theBallModel.estimate.position);
    }
  }


  state(alignBehindBall)
  {
    transition
    {
      if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -200.f, 50.f)
         && theLibCodeRelease.between(theBallModel.estimate.position.x(), 50.f, 170.f))
        goto kick;
    }
    action
    {
      theHeadControlMode = HeadControl::lookForward;
      WalkToTarget(Pose2f(0.2f, 0.2f, 0.2f), Pose2f(theLibCodeRelease.angleToOppGoal, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() + 100.f));
    }
  }

  state(kick)
  {
    transition
    {
      //if(state_time > 3000 || (state_time > 10 && action_done))
        //goto start;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      //InWalkKick(WalkKickVariant(WalkKicks::forward, Legs::left), Pose2f(theLibCodeRelease.angleToOppGoal, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
	  theMotionRequest.motion = MotionRequest::kick;
	  theMotionRequest.kickRequest.kickMotionType = KickRequest::kickForward;
	  //theMotionRequest.kickRequest.mirror = true;
    }
  }

  state(searchForBall)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < 300)
        goto walkToBall;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      WalkAtRelativeSpeed(Pose2f(1.f, 0.f, 0.f));
    }
  }
}
