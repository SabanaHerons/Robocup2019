//
//  ObstacleAvoidance.h
//
//  Created by Andres on 6/28/18.
//

option(ObstacleAvoidance)
{
	initial_state(start) {
		transition
		{
			if(state_time > 15000)
				goto searchForBall;
		}

		action {
	    
			LookAround();
			Stand();
		}
	}

	state(searchForBall)
	{
		transition
		{
			if(theTeamBallModel.isValid){//theTeamBallModel.isValid){
				goto goForBall;
			}
		}
		action
		{
			LookAround();
			Stand();
		}
	}

	state(goForBall)
	{
		transition
		{
			if(/*theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)*/!theTeamBallModel.isValid)
				goto searchForBall;
			Vector2f relativeBall =theTeamBallModel.position-theRobotPose.translation;
			if(relativeBall.norm() < 500.f)
				goto llego;
		}
		action
		{
			LookForward();
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theTeamBallModel.position.x(),theTeamBallModel.position.y()),Pose2f(0.6f,0.6f,0.6f),true);
		}
	}
	state(llego){
		transition{
			Vector2f relativeBall =theTeamBallModel.position-theRobotPose.translation;
			if(relativeBall.norm() > 500.f)
				goto goForBall;
		}
		action{
			LookAround();
			Stand();
		}
	}

}
