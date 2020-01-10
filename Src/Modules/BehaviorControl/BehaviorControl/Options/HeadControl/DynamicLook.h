option(DynamicLook, (float) (0.38f) tilt)
{
  initial_state(initial)
  {
    transition
    {
        if(state_time > 500)
        goto lookBall;
    }
    action
    {
      SetHeadPanTilt(0.f, -0.1f, 150_deg);
    }
  }

  state(lookBall)
  {
    transition
    {
      if(state_time > 6000)
        goto lookMiddle;
    }
    action
    {
      Vector2f relativeBall = theRobotPose.inversePose * theLibCodeRelease.ball;
      theHeadMotionRequest.mode = HeadMotionRequest::targetOnGroundMode;
      theHeadMotionRequest.target = Vector3f(relativeBall.x(), relativeBall.y(), 50);
      theHeadMotionRequest.speed = Angle::fromDegrees(150);
    }
  }

  state(lookMiddle)
  {
    transition
    {
      if(state_time > 1500)
        goto lookLeft;
      //if(state_time > 3000 && (int)state_time % 2 == 0)
        //goto lookRight;
      //if(state_time > 3000 && (int)state_time % 2 == 0)
        //goto lookBall;
    }
    action
    {
      SetHeadPanTilt(0.f, -0.1f, -theMotionInfo.walkRequest.target.translation.angle());
      //theHeadMotionRequest.target = Vector3f(theMotionRequest.walkRequest.target.translation.x(), theMotionRequest.walkRequest.target.translation.y(), 50);
    }
  }

  state(lookLeft)
  {
    transition
    {
      if(state_time > 1500)
        goto lookRight;
    }
    action
    {
      SetHeadPanTilt(0.2f, -0.1f, -theMotionInfo.walkRequest.target.translation.angle() + 20_deg);
    }
  }

  state(lookRight)
  {
    transition
    {
      if(state_time > 1500)
        goto lookBall;
    }
    action
    {
      SetHeadPanTilt(-0.2f, -0.1f, -theMotionInfo.walkRequest.target.translation.angle() + 20_deg);
    }
  }

}
