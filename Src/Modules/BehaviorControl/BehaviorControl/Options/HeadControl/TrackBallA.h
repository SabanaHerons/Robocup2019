option(TrackBallA, (float)(pi) speed, (bool)(false) stopAndGoMode)
{
  // Starts tracking the Point
  initial_state(startTracking)
  {
    transition
    {
      // If the intent is to track the ball, we start looking for it
      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto startSearchingBall;
    }
    action
    {
      theHeadMotionRequest.mode = HeadMotionRequest::targetOnGroundMode;
      theHeadMotionRequest.cameraControlMode = HeadMotionRequest::autoCamera;
      theHeadMotionRequest.speed = speed;

      Pose2f pose = theRobotPose.inversePose * theBallModel.estimate.position;
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
      if(theTeamBallModel.isValid || theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen)<=theBehaviorParameters.ballNotSeenTimeOut)
        goto startTracking;
    }
    action
    {
      LookAround();
      //SearchBall();
    }
  }
}
