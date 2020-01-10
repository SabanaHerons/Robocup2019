option(Goalie)
{
  initial_state(start)
  {
    transition
    {
      if(state_time > 1000)
        goto Walk;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
    }
  }

  state(lookBall)
  {
    transition
    {
      if( -100 > theBallModel.estimate.position.y() && theBallModel.estimate.velocity.x() < -90)
      goto jumpRight;
      if( 100 < theBallModel.estimate.position.y() && theBallModel.estimate.velocity.x() < -90)
      goto jumpLeft;
      if( 100 > theBallModel.estimate.position.y() && -100 < theBallModel.estimate.position.y()
      && theBallModel.estimate.velocity.x() < -90 && (theBallModel.estimate.position.x() < 400))
      goto goDown;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      SpecialAction(SpecialActionRequest::sitDown);
    }
  }
  state(Walk)
  {
    transition
    {
      if(state_time > 1700)
        goto lookBall;
    }
    action
    {
      WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(0.f, 300.f, 0.f));

    }
  }

  state(jumpRight)
  {
    transition
    {

    }
    action
    {
      SpecialAction(SpecialActionRequest::rightDive);
    }
  }
  state(jumpLeft)
  {
    transition
    {

    }
    action
    {
      SpecialAction(SpecialActionRequest::leftDive);
    }
  }
  state(goDown)
  {
    transition
    {

    }
    action
    {
      SpecialAction(SpecialActionRequest::preventBall);
    }
  }
  }
