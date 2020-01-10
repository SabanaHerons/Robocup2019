option(aloneKeeper)
{
	initial_state(whereami)
	{
		transition
		{
			if(state_time > 12000)
				goto returnToArea;
		}
		action
		{
			LookAround();
		}
	}
	state(lookForBall)
	{
		transition
		{
			if(!theLibCodeRelease.keeperInsideGoal && (theFrameInfo.time - theBallModel.timeWhenLastSeen) < 500)
				goto returnToArea;

			if(theBallModel.estimate.velocity.x() > -300 && theLibCodeRelease.ballInsideOwnGoal && (theFrameInfo.time - theBallModel.timeWhenLastSeen) < 500)
				goto despejar;

			if(theLibCodeRelease.keeperInsideGoal && (theFrameInfo.time - theBallModel.timeWhenLastSeen) < 500)
				goto alignToBall;
  	}
		action
		{
			if(state_time > 10000)
			{
				Vector2f area = {theFieldDimensions.xPosOwnGroundline + 200, 0};

				if(std::abs(theRobotPose.translation.x() - (area[0]))  + std::abs(theRobotPose.translation.y() - (area[1])) < 120)
					Stand();
				else
					WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f),Pose2f((theRobotPose.inversePose * ((Vector2f){0.f,0.f})).angle(),theRobotPose.inversePose * area));

				LookAround();
			}
			else
				LookAround();
		}
	}

	state(alignToBall)
	{
	  transition
	  {
			// if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobalRelative, (theRobotPose.translation.y() - 300), (theRobotPose.translation.y() + 300)))
			// 	goto goDown;
			// else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobalRelative, (theRobotPose.translation.y() - 800), (theRobotPose.translation.y() - 300)))
			// 	goto PreventRight;
			// else if(theLibCodeRelease.between(theLibCodeRelease.intersectoEnYGlobalRelative, (theRobotPose.translation.y() + 300), (theRobotPose.translation.y() + 800)))
			// 	goto PreventLeft;

			if((theFrameInfo.time - theBallModel.timeWhenLastSeen) > 2500)
  			goto lookForBall;

			if(!theLibCodeRelease.keeperInsideGoal)
				goto returnToArea;

			if(theBallModel.estimate.velocity.x() > -300 && theLibCodeRelease.ballInsideOwnGoal)
				goto despejar;
	  }

	  action
		{
			if(state_time < 1100)
				HeadControlMode(HeadControl::lookForward);
			else
				TrackBallA();

			Vector2f toFollow;
			float limite = 400.f;
			//3750
			if((theRobotPose.translation.y() + theBallModel.estimate.position.y()) > limite){
				toFollow = {theFieldDimensions.xPosOwnGroundline + 200, limite};
			}else if((theRobotPose.translation.y() + theBallModel.estimate.position.y()) < -limite){
				toFollow = {theFieldDimensions.xPosOwnGroundline + 200, -limite};
			}else{
				Vector2f global = theRobotPose * theBallModel.estimate.position;
				toFollow = {theFieldDimensions.xPosOwnGroundline + 200, global.y()};
			}

			if(std::abs(theRobotPose.translation.x() - (toFollow[0]))  + std::abs(theRobotPose.translation.y() - (toFollow[1])) < 100){
				if(std::abs(theBallModel.estimate.position.angle()) < 5_deg)
					Stand();
				else
					WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theBallModel.estimate.position.angle(), 0.f, 0.f));
			}else
				WalkToTarget(Pose2f(0.8f, 0.6f, 0.6f),Pose2f(((Vector2f){theBallModel.estimate.position.x(),theBallModel.estimate.position.y()}).angle(),theRobotPose.inversePose * toFollow));
		}
	}

	state(returnToArea)
  {
    transition
    {
			if((theFrameInfo.time - theBallModel.timeWhenLastSeen) > 2500)
  			goto lookForBall;

			if(theLibCodeRelease.keeperInsideGoal)
				goto alignToBall;
    }
    action
    {
			Vector2f area = {theFieldDimensions.xPosOwnGroundline + 200, 0};
			theMotionRequest = thePathPlanner.plan(Pose2f(theBallModel.estimate.position.angle(),area),Pose2f(0.6f, 0.6f, 0.6f),false,true);
		}
  }

	// state(PreventRight)
	// {
	// 	transition
	// 	{
	// 		if (state_time > 1000)
	// 			goto lookForBall;
	// 	}
	// 	action
	// 	{
	// 		SpecialAction(SpecialActionRequest::rightDive);
	// 	}
	// }
	// state(PreventLeft)
	// {
	// 	transition
	// 	{
	// 		if (state_time > 1000)
	// 			goto lookForBall;
	// 	}
	// 	action
	// 	{
	// 		SpecialAction(SpecialActionRequest::leftDive);
	// 	}
	// }
	//
	// state(goDown)
	// {
	// 	transition
	// 	{
	// 		if(state_time > 1000)
	// 			goto lookForBall;
	// 	}
	// 	action
	// 	{
	// 		SpecialAction(SpecialActionRequest::preventBall);
	// 	}
	// }

	state(despejar)
  {
    transition
    {
			if(action_done || action_aborted)
				goto returnToArea;
			if((theFrameInfo.time - theBallModel.timeWhenLastSeen) > 2500)
  			goto lookForBall;

			if((!theLibCodeRelease.keeperInsideGoal && theLibCodeRelease.theRivalIsCloserToTheBall) ||
				(!theLibCodeRelease.keeperInsideGoal && !theLibCodeRelease.closerToTheBall))
				goto returnToArea;
    }
    action
    {
			Shoot();
    }
  }
}
