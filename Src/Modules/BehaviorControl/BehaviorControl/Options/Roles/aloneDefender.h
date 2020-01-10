option(aloneDefender)
{
  Vector2f relativeBall = theRobotPose.inversePose*theLibCodeRelease.ball;

  initial_state(geneticDefense)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto whereIsIt;

      if(!theLibCodeRelease.theRivalIsCloserToTheBall)
        goto despejando;

      if(theBallModel.estimate.position.x() > 500)
        goto followFromDistance;
    }
    action
    {
      float ballWeight = 0.75f;

      theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = ArmKeyFrameRequest::back;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;

      TrackBall();
      theMotionRequest = thePathPlanner.plan(theGeneticLocator.activation(false, (int)theFrameInfo.time,ballWeight),Pose2f(0.6f, 0.6f, 0.6f),false,true);
    }
  }

  state(followFromDistance)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto whereIsIt;

      if(!theLibCodeRelease.theRivalIsCloserToTheBall)
        goto despejando;

      if(theBallModel.estimate.position.x() < 450)
        goto geneticDefense;

    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      Vector2f going2 = {-1100,theBallModel.estimate.position.y()};

      if(theRobotInfo.number == 2 && theRobotPose.translation.y() < 2700 && theRobotPose.translation.y() > -1700)
        going2 = {-1100.f,theBallModel.estimate.position.y() + 150.f};
      else if(theRobotInfo.number == 3 && theRobotPose.translation.y() > -2700 && theRobotPose.translation.y() < 1700)
        going2 = {-1100.f, theBallModel.estimate.position.y() - 150.f};

      theMotionRequest = thePathPlanner.plan(Pose2f(theBallModel.estimate.position.angle(), going2),Pose2f(0.6f, 0.6f, 0.6f),false,true);
    }
  }

  state(whereIsIt)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen < theBehaviorParameters.ballNotSeenTimeOut)
        goto geneticDefense;
    }
    action
    {
      TrackBall();
    }
  }

  state(despejando)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto whereIsIt;

      if(relativeBall.norm() < 500.f)
        goto shoot;

      if(theLibCodeRelease.theRivalIsCloserToTheBall)
        goto geneticDefense;
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

  state(shoot)
  {
    transition
    {
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto whereIsIt;

      if(theBallModel.estimate.position.norm()>600 || action_done || action_aborted )
        goto geneticDefense;
    }
    action
    {
      Shoot();
    }
  }
}
