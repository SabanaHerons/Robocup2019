//
//  Goalie.h
//  Universidad de la Sabana
//
//  Created by Juan David Cabrera on 6/27/18.
//
option(Goalie){
    initial_state(start){
        transition{
            if(state_time > 1000)
                goto lookBall;
        }
        action{
            //WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(0.f, 0.f, 0.f));
            theHeadControlMode = HeadControl::lookForward;
        }
    }
    
    state(lookBall){
        transition{
            //if(state_time>5000){
              //if(theLibCodeRelease.alignToGoal > -1_deg &&
							//theLibCodeRelease.angleToGoal < 1_deg){
							
							
			//makes the goalie go right or jump right depending the ball speed
				if( -150 > theBallModel.estimate.position.y()){
                    if(theBallModel.estimate.velocity.x() < -150 && theBallModel.estimate.position.x() < 1000){
                    goto jumpRight;
                  }else{
                    goto walkRight;
                  }
                }

								//Makes the goalie go left or jump left depending the ball speed
                	if( 150 < theBallModel.estimate.position.y()){
						if(theBallModel.estimate.velocity.x() < -150 && theBallModel.estimate.position.x() < 1000){
							goto jumpLeft;
                    }else{
                        goto walkLeft;
                    }
									}

							  //If the ball is near and it's not moving, nao goes and kick it 
									if(std::abs(theBallModel.estimate.velocity.x()) < 10  && std::abs(theBallModel.estimate.velocity.y())	< 10 && (theBallModel.estimate.position.x()) < 400 && std::abs(theBallModel.estimate.position.y()) < 150){
										goto kickNear;
									}

                //If the ball goes in a straight line to the robot
								//it stays still or sits down.
								
                if(-150 <= theBallModel.estimate.position.y() && theBallModel.estimate.position.y() <= 150){
                    if(theBallModel.estimate.velocity.x() < -150 && theBallModel.estimate.position.x() < 1000){
                        goto goDown;
                    }else{
                      goto stayInPlace;
                    }
                }

								//TODO make nao search for the ball 
								//TODO make nao not pass the goal lines
              //}else{
               // goto alignToGoal;
             //}
           //}
        }
        action{
					  //looks at ball
            theHeadControlMode = HeadControl::lookAtBall;
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(0.f, 0.f, 0.f));
        }
    }
    
		//jumps left 
    state(jumpLeft){
        transition{
            if(state_time > 13000 || ((action_done && state_time > 10000)))
                goto lookBall;
        }
        action{
            SpecialAction(SpecialActionRequest::demoLeft);
            
        }
    }
    
		// jumps right
    state(jumpRight){
        transition{
            if(state_time > 13000 || (action_done && state_time > 15000))
                goto lookBall;
        }
        action{
            SpecialAction(SpecialActionRequest::demoRight);
        }
    }
    
		// Sits down to prevent the ball
    state(goDown){
        transition{
            if(action_done && state_time > 15000)
                goto lookBall;
        }
        action{
            SpecialAction(SpecialActionRequest::preventBall);
        }
    }

		// It aligns to the enemy goal    
    state(alignToGoal){
        transition{
            if(action_done && state_time > 1000)
                goto lookBall;
        }
        action{
            WalkToTarget(Pose2f(100.f, 100.f, 100.f), Pose2f(theLibCodeRelease.angleToGoal, 0.f , 0.f));
            theHeadControlMode = HeadControl::lookForward;
        }
    }

    // walks left behind the ball 
    state(walkLeft){
        transition{
            if(action_done && state_time > 500)
                goto lookBall;
        }
        action{
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theLibCodeRelease.angleToGoal, 0.f, theBallModel.estimate.position.y()));
        }
    }
    
		//walks right behind the ball 
    state(walkRight){
        transition{
            if(action_done && state_time > 500)
                goto lookBall;
						//if(GoalFeature.);
							//goto stayInPlace;
        }
        action{
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(theLibCodeRelease.angleToGoal, 0.f, theBallModel.estimate.position.y()));
        }
    }
		
		//it keeps in place
    state(stayInPlace){
        transition{
            if(action_done && state_time > 500)
                goto lookBall;
        }
        action{
            WalkToTarget(Pose2f(50.f, 50.f, 50.f), Pose2f(0.f,0.f, 0.f));
        }
		}
		
    // state where it kicks the ball, takes time and action done
		state(kickNear){

		transition{
				if(state_time > 2000 || (state_time > 1000 && action_done)){
				  goto lookBall;		
				}
			}
		action{
							// patada de paladines
		}
	}
}
