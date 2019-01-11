//
// Universidad De La Sabana
// Made by Juan David Cabrera

option(Keeper)
{
	initial_state(start)
	{
		transition
		{
		  if (state_time > 1000)
			goto lookForBall;
		}

		action
		{
		  HeadControlMode(HeadControl::lookForward);
		  Stand();
		}
	}

	//TODO implement the keeper inside penalty area condition

	state(lookForBall)
	{
		transition
		{
			//if (theLibCodeRelease.keeperInsideGoal) {
				if (!theLibCodeRelease.ballInsideOwnGoal) // if the ball i not inside the penalty zone it will enter the if
				{										  // if not it will go to an emergency state 
					if (!(theBallModel.estimate.velocity.x() < -150 && theBallModel.estimate.position.x() < 1000)) // if the ball is not coming to the robot it will enter the if 
					{                                                                                              // if not it will make a dive, inside the if nao will mantain align with the ball 
						if (std::abs(theLibCodeRelease.angleToOwnGoal) > 10)									   //  inside the penalty zone 
						{
							goto alignToGoal;
						}
						else
						{
							if (-150 > theBallModel.estimate.position.y())
							{
								goto walkRight;
							}

							if (-150 > theBallModel.estimate.position.y())
							{
								goto walkLeft;
							}
						}
					}
					else
					{
						if (-150 > theBallModel.estimate.position.y())
						{
							goto rightDive;
						}

						if (150 > theBallModel.estimate.position.y())
						{
							goto leftDive;
						}
					}
				}
				else
				{
					goto emergency;
				}
			//}
		}

		action
		{
			LookAround();
			SpecialAction(SpecialActionRequest::sumo);
		}
	}

	state(emergency)
	{
		transition
		{
			if (-150 > theBallModel.estimate.position.y() && theBallModel.estimate.velocity.x() < -150) // nao will make a ground punch depending of the ball location
			{											   // or will try to kick it if its steady 
				goto rightPunch;
			}
			else
			{
				if (150 > theBallModel.estimate.position.y() && theBallModel.estimate.velocity.x() - 150)
					goto leftPunch;
				goto alignBehindBall;
			}

		}

		action
		{
			HeadControlMode(HeadControl::lookAtBall);
			SpecialAction(SpecialActionRequest::sumo);
		}
	}

	state(walkToBall)
	{
		transition
		{
			if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto lookForBall;
			if (theBallModel.estimate.position.norm() < 500.f)
				goto alignBehindBall;
		}

		action
		{
			 HeadControlMode(HeadControl::lookAtBall);
			 WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), theBallModel.estimate.position);
		}
	}

	// aligns to own goal for preventing the ball
	state(alignToGoal)
	{
		transition
		{
			if (std::abs(theLibCodeRelease.angleToOwnGoal) < 10_deg)
				goto lookForBall;
		}

		action
		{
			WalkToTarget(Pose2f(100.f, 100.f, 100.f),Pose2f(theLibCodeRelease.angleToOwnGoal,100.f,100.f));
		}
	}

	state(alignBehindBall)
	{
		transition
		{
			if (theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
				goto lookForBall;

			 if (theLibCodeRelease.between(theBallModel.estimate.position.y(), 20.f, 50.f) 
				&& theLibCodeRelease.between(theBallModel.estimate.position.x(), 140.f, 170.f)
				 && std::abs(theLibCodeRelease.angleToOppGoal) < 10_deg)
				goto kick;
		}

		action
		{
			theHeadControlMode = HeadControl::lookForward;
			WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f(theLibCodeRelease.angleToOppGoal, theBallModel.estimate.position.x() - 150.f, theBallModel.estimate.position.y() - 30.f));
		}
	}

	state(kick)
	{
		transition
		{
			if (state_time > 3000 || (state_time > 10 && action_done))
			{
				goto start;
			}
		}

		action
		{
			//TODO call the kick
		}
	}

	state(rightPunch)
	{
		transition
		{
			if (state_time > 100 && action_done)
				goto start;
		}

		action
		{
			SpecialAction(SpecialActionRequest::groundPunchRight);
		}
	}

	state(rightDive)
	{
		transition
		{
			if (state_time > 2000 || (action_done && state_time > 100))
				goto start;
		}

		action
		{
			SpecialAction(SpecialActionRequest::diveRight);
		}
	}

	state(leftPunch)
	{
		transition
		{
			if (state_time > 100 && action_done)
				goto start;
		}

		action
		{
			SpecialAction(SpecialActionRequest::groundPunchLeft);
		}
	}

	state(leftDive)
	{
		transition
		{
			if (state_time > 13000 || (action_done && state_time > 15000))
				goto start;
		}

		action
		{
			SpecialAction(SpecialActionRequest::diveLeft);
		}
	}

	state(walkRight)
	{
		transition
		{
			if (action_done && state_time > 500)
				goto lookForBall;

		}

		action
		{
			WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theLibCodeRelease.angleToOwnGoal, 0.f, theBallModel.estimate.position.y()));
		}
	}

	state(walkLeft)
	{
		transition
		{
			if (action_done && state_time > 500)
				goto lookForBall;
		}

		action
		{
			WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theLibCodeRelease.angleToOwnGoal, 0.f, theBallModel.estimate.position.y()));
		}
	}
}
