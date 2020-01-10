option(Supporter)
{
  Vector2f relativeBall =theRobotPose.inversePose*theLibCodeRelease.ball;

  initial_state(stealer)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 800)
        goto whereIsItNow;
      if(theTeamBallModel.position.x() < (theRobotPose.translation.x() - 350))
        goto back2Defender;

      if(theBallModel.estimate.position.norm() < 370.f)
        goto steal;
    }
    action
    {
      TrackBall();
      theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.ball),Pose2f(0.6f,0.6f,0.6f),false,true);
    }
  }

  state(whereIsItNow)
  {
    transition
    {
      if(theTeamBallModel.position.x() < (theRobotPose.translation.x() - 350))
        goto back2Defender;

      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) < 500)
        goto stealer;
    }
    action
    {
      TrackBall();
    }
  }
  state(steal)
  {
    transition
    {
      if(theTeamBallModel.position.x() < (theRobotPose.translation.x() - 350)  || action_aborted)
        goto back2Defender;

      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 800)
        goto whereIsItNow;

      if(action_done)
        goto hasStolen;
    }
    action
    {
      StealBall();
    }
  }

  state(hasStolen)
  {
    transition
    {
      if(theTeamBallModel.position.x() < (theRobotPose.translation.x() - 350))
        goto back2Defender;

      if(state_time > 1400 && theLibCodeRelease.closerToTheBall)
        goto despejando;

      if(state_time > 1400 && !theLibCodeRelease.closerToTheBall)
        goto back2Defender;

    }
    action
    {
      TrackBall();
      WalkAtRelativeSpeed(Pose2f(0.85f, -0.6f, 0.f));
    }
  }

  state(despejando)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 450.f)
        goto whereIsIt;

      if(relativeBall.norm()  < 350.f)
        goto shoot;

      if(!theLibCodeRelease.closerToTheBall || theLibCodeRelease.theRivalIsCloserToTheBall)
        goto back2Defender;
    }
    action
    {
      TrackBall();
      theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.ball),Pose2f(0.6f,0.6f,0.6f),false,true);

      theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = ArmKeyFrameRequest::back;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
    }
  }

  state(whereIsIt)
  {
    transition
    {
      if(theTeamBallModel.position.x() < (theRobotPose.translation.x() - 350))
        goto back2Defender;

      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) < 500)
        goto despejando;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      WalkAtRelativeSpeed(Pose2f(0.85f, 0.f, 0.f));
    }
  }

  state(shoot)
  {
    transition
    {
      if(theTeamBallModel.position.x() < (theRobotPose.translation.x() - 350) || theTeamBallModel.position.y() > 1000.f)
        goto back2Defender;

      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 800 || action_done || action_aborted)
        goto whereIsItNow;
    }
    action
    {
      Vector2f target = Vector2f(4500.f, 0.f);

      if(!theLibCodeRelease.shootToGoal && false)
        target = theLibCodeRelease.bestTeammateForPass();


      LookBallTarget(theRobotPose.inversePose*target);
      ShootAt(target,false);
    }
  }

  target_state(back2Defender)
  {
    action
    {
      theBehaviorStatus.role = Role::defender;
    }
  }
}
