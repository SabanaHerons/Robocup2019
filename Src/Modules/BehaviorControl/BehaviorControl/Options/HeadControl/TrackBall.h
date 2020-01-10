
// We May need 2 state machine for looking at the ball and looking at a point because
// The way we calculate the target may vary from a point and a ball

option(TrackBall, (float)(pi) speed, (bool)(false) stopAndGoMode)
{
  initial_state(initial)
  {
    transition
    {
      // If the intent is to track the ball, we start looking for it
      if(theLibCodeRelease.timeSinceBallWasSeen>theBehaviorParameters.ballNotSeenTimeOut)
        goto startSearchingBall;
      goto startTracking;
    }
  }

  // Starts tracking the Point
  state(startTracking)
  {
    transition
    {
      // If the intent is to track the ball, we start looking for it
      if(theLibCodeRelease.timeSinceBallWasSeen>theBehaviorParameters.ballNotSeenTimeOut)
        goto startSearchingBall;
    }
    action
    {
      theHeadMotionRequest.mode = HeadMotionRequest::targetOnGroundMode;
      theHeadMotionRequest.cameraControlMode = HeadMotionRequest::autoCamera;
      theHeadMotionRequest.speed = speed;

      Pose2f pose = theRobotPose.inversePose * theLibCodeRelease.ball;
      theHeadMotionRequest.target = Vector3f(pose.translation.x(), pose.translation.y(), 0.f);
      theHeadMotionRequest.stopAndGoMode = stopAndGoMode;
    }
  }

  /** This state waits until the SearchBall control finds the ball */
  state(startSearchingBall)
  {
    transition
    {
      //if(theTeamBallModel.isValid || theBallModel.seenPercentage>20)
      if (action_done)
        goto startTracking;
    }
    action
    {
      //LookAround();
      SearchBall();
    }
  }
}
