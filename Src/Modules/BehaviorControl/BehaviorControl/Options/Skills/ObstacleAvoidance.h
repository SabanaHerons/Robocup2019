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
		if(state_time > 15000 && theRobotInfo.number == 2)
		goto A;
	}

        action {
	    //theMotionRequest = thePathPlanner.plan(Pose2f(3500,0,0),Pose2f(0.2f,0.2f,0.2f),true);
            LookAround();
            Stand();
        }
    }

    state(A)
	{
		transition
		{
			if(state_time > 6000)
			goto B;
		}
		action
		{
			LookForward();
			if(theTeamBallModel.isValid)
		{
		    theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theTeamBallModel.position.x(),theTeamBallModel.position.y()),Pose2f(0.1f,0.1f,0.1f),true);
		}
		}
	}

	state(B)
	{
	transition
		{
		if(state_time > 5000)
		goto A;
		else if((theRobotPose.inversePose*theTeamBallModel.position).norm()< 500)
			goto C;
		}
	action
	{
		LookAround();
		Stand();
	}
}
	state(C)
	{
		action
		{
		Striker();
		}
	}	

}
