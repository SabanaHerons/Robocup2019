option(LookAtBallD)
{
	initial_state(start)
	{
		action
		{
		  theHeadMotionRequest.mode = HeadMotionRequest::targetOnGroundMode;
		  theHeadMotionRequest.target = Vector3f(theBallModel.estimate.position.x(), theBallModel.estimate.position.y(), 50);
		  theHeadMotionRequest.speed = Angle::fromDegrees(150);
		}
	}
}
