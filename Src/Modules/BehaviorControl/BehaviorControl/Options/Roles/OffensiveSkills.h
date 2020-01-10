//
//  OffensiveSkills.h
//
//  Created by Omar Sanchez on 31/05/19.
//
option(OffensiveSkills)
{
	Vector2f relativeBall =theRobotPose.inversePose*theLibCodeRelease.ball;

	common_transition{

		/*
		if(theRobotPose.validity<0.7f){
			goto findYourSelf;
		}
		*/
	}

	initial_state(start) {
		transition
		{
			if(state_time > 100)
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
			if(theLibCodeRelease.timeSinceBallWasSeen < 300){
				goto wait;
			}

		}
		action
		{
			TrackBall();
		}
	}
	state(wait){

		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(theLibCodeRelease.closerToTheBall){
				goto goForBall;
			}
			if((theLibCodeRelease.desiredPos-theRobotPose.translation).norm() > 200.f && !theLibCodeRelease.areShooting){
					goto goToPlace;
			}
		}
		action
		{
			StopBall();
			LookAround();
		}
	}

	state(goToPlace)
	{
		transition
		{
		  	if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(std::abs(theLibCodeRelease.desiredPos.x()-theRobotPose.translation.x()) < 100. || theLibCodeRelease.areShooting)
				goto wait;

			if(theLibCodeRelease.closerToTheBall){
				if(theLibCodeRelease.theRivalHasTheBall && std::abs((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle())<30_deg){
					goto goToSteal;
				} else {
					goto goForBall;
				}
			}
		}
		action
		{
			//SetHeadTargetOnGround(Vector3f(theLibCodeRelease.desiredPos.x(),theLibCodeRelease.desiredPos.y(),0));
			DynamicLook();
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.desiredPos),Pose2f(0.8f,1.f,1.f),false,true);
		}
	}

	state(goForBall)
	{
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(relativeBall.norm() < theBehaviorParameters.distanceToGoShoot && theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f)){
				if(!theLibCodeRelease.shootToGoal){
					Vector2f target=theLibCodeRelease.bestTeammateForPass();
					if(target.x()==-100000){
						goto dribble;
					}else{
						goto shoot;
					}
				}
				goto shoot;
			}
			if(!theLibCodeRelease.closerToTheBall)
				goto wait;
			if(theLibCodeRelease.theRivalHasTheBall && std::abs((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle())<30_deg)
				goto goToSteal;
		}
		action
		{
			DynamicLook();
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.ball),Pose2f(0.6f,1.f,1.f),false,true);
		}
	}

	state(shoot){
		transition{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(action_done){
				goto wait;
			}

			if(theLibCodeRelease.theRivalHasTheBall && std::abs((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle())<30_deg){
					goto goToSteal;
			}else{
				if(relativeBall.norm() > (theBehaviorParameters.distanceToGoShoot+100.f)|| action_aborted || !theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f) ){
					goto goForBall;
				}
			}

			if(!theLibCodeRelease.closerToTheBall)
				goto wait;
		}
		action{
			//En caso de no encontrar aliados intenta disparar al arco
			Vector2f target=theLibCodeRelease.whereToShoot;
			if(!theLibCodeRelease.shootToGoal){
				Vector2f teami=theLibCodeRelease.bestTeammateForPass();
				if(teami.x()!=-100000)
					target=teami;
			}

			Vector2f relTarget= theRobotPose.inversePose*target;
			ShootAt(target,false);
			LookBallTarget(relTarget);
		}
	}


	state(dribble){
		transition{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(action_done){
				goto wait;
			}
			Vector2f target=theLibCodeRelease.bestTeammateForPass();
			if(theLibCodeRelease.shootToGoal || target.x()!=-100000){
				goto shoot;
			}

			if(relativeBall.norm() > (theBehaviorParameters.distanceToGoShoot+100.f)|| action_aborted || !theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f) ){
				goto goForBall;
			}

			if(!theLibCodeRelease.closerToTheBall)
				goto wait;
		}
		action{
			float target;
			if(theLibCodeRelease.ball.x()<theFieldDimensions.xPosOpponentGroundline*0.93f && std::abs(theLibCodeRelease.ball.y())<600.f){
				target= theFieldDimensions.xPosOpponentGroundline+250.f;
			}else{
				target= theFieldDimensions.xPosOpponentGroundline*0.90f;
			}
			DribbleBall(Vector2f(target, 0.f));
		}
	}


	state(goToSteal)
	{
		transition
		{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(relativeBall.norm() < theBehaviorParameters.distanceToGoShoot && theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f))
				goto steal;
			if(!theLibCodeRelease.closerToTheBall)
				goto wait;
			if(!theLibCodeRelease.theRivalHasTheBall || std::abs((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle())>40_deg)
				goto goForBall;
		}
		action
		{
			TrackBall();
			float offset= -100.f;
			if(theLibCodeRelease.ball.y()>=0)
				offset*=-1;

			Vector2f positionForSteal = theLibCodeRelease.rotateCoordenates((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle(), Vector2f(-300.f,offset))+theLibCodeRelease.ball;

			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,positionForSteal),Pose2f(0.6f,1.f,1.f),false,true);
		}
	}


	state(steal){
		transition{
			if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut){
				goto searchForBall;
			}
			if(action_done){
				goto goBack;
			}
			if(relativeBall.norm() > (theBehaviorParameters.distanceToGoShoot+100.f)|| action_aborted || !theLibCodeRelease.isThatClear(theLibCodeRelease.ball,700.f)){
					goto goToSteal;
			}else{
				if(!theLibCodeRelease.theRivalHasTheBall || std::abs((theLibCodeRelease.nearestEnemy-theLibCodeRelease.ball).angle())>40_deg){
					goto goForBall;
				}
			}
			if(!theLibCodeRelease.closerToTheBall)
				goto wait;
		}
		action{
			//Patadita pal ladin

			bool conIzquierda= theLibCodeRelease.ball.y()>=0;
			TrackBall();
			StealBallWith(conIzquierda);
		}
	}

	state(goBack)
	{
	    transition
	    {
	      if(state_time<1000)
	        goto wait;
	    }
	    action
	    {

	      LookForward();
	      WalkAtRelativeSpeed(Pose2f(0.f, -1.f, 0.f));
	    }
	}

}
