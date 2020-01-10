option(StealBallWith, (const bool) conIzquierda)
{
  initial_state(alignBehindBall)
  {
    transition
    {
      if(theBallModel.estimate.position.norm() > theBehaviorParameters.distanceToGoShoot)
        goto abortedState;

      if(theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f - conIzquierda*70, 50.f-conIzquierda*70)
        && theLibCodeRelease.between(theBallModel.estimate.position.x(), 150.f, 160.f)
        && std::abs(theRobotPose.rotation) < 2_deg)
        goto kick;
    }
    action
    {
      WalkToTarget(Pose2f(0.5f,0.5f, 0.5f), Pose2f(-theRobotPose.rotation, theBallModel.estimate.position.x() - 155.f, theBallModel.estimate.position.y() -36.f +72*conIzquierda));
    }
  }

  aborted_state(abortedState)
  {
    action
    {
      TrackBall();
    }
  }

  state(kick)
  {
    transition
    {
      if(action_done || action_aborted)
       goto stealed;
    }
    action
    {
      if (conIzquierda)
        InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::left), Pose2f(0.f, theBallModel.estimate.position.x() , theBallModel.estimate.position.y() - 30.f));
      else
        InWalkKick(WalkKickVariant(WalkKicks::sidewardsInner, Legs::right), Pose2f(0.f, theBallModel.estimate.position.x() , theBallModel.estimate.position.y() + 30.f));
    }
  }
  target_state(stealed)
  {
    action
    {
      TrackBall();
    }
  }
}
