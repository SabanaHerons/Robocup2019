/** behavior for the ready state */
option(ReadyState)
{
  /* position has been reached -> stand and wait */
  initial_state(stand)
  {
   transition
    {
        if(state_time >= 40)
            goto girar;
    }
    action
    {
      if(theRobotInfo.number == 1){
	theBehaviorStatus.role = Role::keeper;
	}
      else if(theRobotInfo.number == 2){
	theBehaviorStatus.role = Role::striker;
	}

      else if(theRobotInfo.number == 3){
	theBehaviorStatus.role = Role::striker;
	}

      else if(theRobotInfo.number == 4){
	theBehaviorStatus.role = Role::defender;	
	}
  
      else if(theRobotInfo.number == 5){
	theBehaviorStatus.role = Role::defender;
	}
  
      HeadControlMode(HeadControl::lookForward);
      Stand();
    }
  }

  state(initialPosition)
  { 
  	Pose2f idealPosition= Pose2f(0.f, 0.f,0.f);


      transition
      {

      	

     if(theRobotInfo.number == 1){
     	idealPosition=Pose2f(0.f,-3900.f,0.f);
     	Vector2f proximidad1= idealPosition.translation- theRobotPose.translation;
     	if(std::abs(proximidad1.norm())<30.f){
     		goto rotar;
     	}
		
	}
	else if(theRobotInfo.number == 2){
		idealPosition=Pose2f(0.f,-3500.f,900.f);
		Vector2f proximidad2= idealPosition.translation- theRobotPose.translation;
     	if(std::abs(proximidad2.norm())<30.f){
     		goto rotar;
     	}
		
	}
	else if(theRobotInfo.number == 3){
		idealPosition=Pose2f(0.f,-3500.f,-900.f);
		Vector2f proximidad3= idealPosition.translation- theRobotPose.translation;
     	if(std::abs(proximidad3.norm())<30.f){
     		goto rotar;
     	}
		
	}
	else if(theRobotInfo.number == 4){
		idealPosition=Pose2f(0.f,-1850.f,1600.f);
		Vector2f proximidad4= idealPosition.translation- theRobotPose.translation;
     	if(std::abs(proximidad4.norm())<30.f){
     		goto rotar;
     	}
		
	}
	else if(theRobotInfo.number == 5){
		idealPosition=Pose2f(0.f,-1850.f,-1600.f);
		Vector2f proximidad5= idealPosition.translation- theRobotPose.translation;
     	if(std::abs(proximidad5.norm())<30.f){
     		goto rotar;
     	}
		
	}
      }
      action
      {
      		HeadControlMode(HeadControl::lookForward);
			theMotionRequest = thePathPlanner.plan(idealPosition,Pose2f(0.9f,0.9f,0.9f),true);
	
      }
  }
  state(rotar){
  	action{
  		 WalkToTarget(Pose2f(0.8f, 0.8f, 0.8f), Pose2f(theLibCodeRelease.angleToOppGoal, 0.f, 0.f));

  }
  }
  state(girar){
  	transition
    {
        if(state_time > 12000)
            goto initialPosition;
    }
  	action{
  		 LookAround();

  }
  }
}

