option(Keeper)
{
	initial_state(lookForBall)
	{
		transition
		{
			if(!theLibCodeRelease.keeperInsideGoal && theLibCodeRelease.theRivalIsCloserToTheBall && (theFrameInfo.time - theBallModel.timeWhenLastSeen) < 500)
				goto returnToArea;

			if(theBallModel.estimate.velocity.x() > -100 && !theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.closerToTheBall && (theFrameInfo.time - theBallModel.timeWhenLastSeen) < 500)
				goto despejar;

			if((theLibCodeRelease.keeperInsideGoal && (theFrameInfo.time - theBallModel.timeWhenLastSeen) < 500 && theLibCodeRelease.theRivalIsCloserToTheBall) ||
				 theTeamBallModel.position.x() > -3000)
				goto alignToBall;
  	}
		action
		{
			Vector2f area = {theFieldDimensions.xPosOwnGroundline + 200, 0};
		  if((std::abs(theRobotPose.translation.x() - area[0]) + std::abs(theRobotPose.translation.y() - area[1])) < 150){
				LookAround();
				Stand();
			}
			else{
				HeadControlMode(HeadControl::lookForward);
				WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f),Pose2f((theRobotPose.inversePose * ((Vector2f){0.f,0.f})).angle(),theRobotPose.inversePose * area));
			}
		}
	}
	state(alignToBall)
	{
	  transition
	  {
			if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() - 300), (theRobotPose.translation.y() + 300)))
				goto goDown;
			else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() - 3000), (theRobotPose.translation.y() - 300)))
				goto PreventRight;
			else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() + 300), (theRobotPose.translation.y() + 3000)))
				goto PreventLeft;

			if((theFrameInfo.time - theBallModel.timeWhenLastSeen) > 2500 && theTeamBallModel.position.x() < -3150)
  			goto lookForBall;

			if((!theLibCodeRelease.keeperInsideGoal && theLibCodeRelease.theRivalIsCloserToTheBall) ||
				(!theLibCodeRelease.keeperInsideGoal && !theLibCodeRelease.closerToTheBall))
				goto returnToArea;

			if(theBallModel.estimate.velocity.x() > -100 && !theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.closerToTheBall)
				goto despejar;
	  }
	  action
		{
			TrackBallA();

			Vector2f toFollow;
			float limite = 350.f;
			//3750
			if(theTeamBallModel.position.y() > limite){
				toFollow = {theFieldDimensions.xPosOwnGroundline + 200, limite};
			}else if(theTeamBallModel.position.y() < -limite){
				toFollow = {theFieldDimensions.xPosOwnGroundline + 200, -limite};
			}else{
				toFollow = {theFieldDimensions.xPosOwnGroundline + 200, theTeamBallModel.position.y()};
			}

			if(std::abs(theRobotPose.translation.x() - (toFollow[0]))  + std::abs(theRobotPose.translation.y() - (toFollow[1])) < 100){
				Stand();
			}
			else{
				WalkToTarget(Pose2f(0.8f, 0.6f, 0.6f),Pose2f((theRobotPose.inversePose * ((Vector2f){theTeamBallModel.position.x(),theTeamBallModel.position.y()})).angle(),theRobotPose.inversePose * toFollow));
			}
		}
	}

	state(returnToArea)
  {
    transition
    {
			if(theLibCodeRelease.keeperInsideGoal)
			{
				if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() - 300), (theRobotPose.translation.y() + 300)))
					goto goDown;
				else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() - 3000), (theRobotPose.translation.y() - 300)))
					goto PreventRight;
				else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() + 300), (theRobotPose.translation.y() + 3000)))
					goto PreventLeft;
			}
			if((theFrameInfo.time - theBallModel.timeWhenLastSeen) > 2500)
				goto lookForBall;

			if(theLibCodeRelease.keeperInsideGoal)
				goto alignToBall;
    }
    action
    {
			Vector2f area = {theFieldDimensions.xPosOwnGroundline + 200, 0};
			// WalkToTarget(Pose2f(0.6f, 0.6f, 0.6f),Pose2f((theRobotPose.inversePose * area).angle(),theRobotPose.inversePose * area));
			theMotionRequest = thePathPlanner.plan(Pose2f((theRobotPose.inversePose * ((Vector2f){theTeamBallModel.position.x(),theTeamBallModel.position.y()})).angle(),area),Pose2f(0.6f, 0.6f, 0.6f),false,true);
		}
  }

  state(PreventRight)
  {
    transition
    {
      if (state_time > 1000)
				goto lookForBall;
    }
    action
    {
      SpecialAction(SpecialActionRequest::rightDive);
    }
  }
	state(PreventLeft)
	{
		transition
		{
			if (state_time > 1000)
				goto lookForBall;
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
			if(state_time > 1000)
				goto lookForBall;
		}
		action
		{
			SpecialAction(SpecialActionRequest::preventBall);
		}
	}

	state(despejar)
  {
    transition
    {
			if(theLibCodeRelease.keeperInsideGoal)
			{
				if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() - 300), (theRobotPose.translation.y() + 300)))
					goto goDown;
				else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() - 1000), (theRobotPose.translation.y() - 300)))
					goto PreventRight;
				else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobal, (theRobotPose.translation.y() + 300), (theRobotPose.translation.y() + 1000)))
					goto PreventLeft;
			}
			if((theFrameInfo.time - theBallModel.timeWhenLastSeen) > 2500)
				goto lookForBall;

			if((!theLibCodeRelease.keeperInsideGoal && theLibCodeRelease.theRivalIsCloserToTheBall) ||
				(!theLibCodeRelease.keeperInsideGoal && !theLibCodeRelease.closerToTheBall))
				goto returnToArea;
    }
    action
    {
      // TrackBallA();
			Shoot();
    }
  }
}
