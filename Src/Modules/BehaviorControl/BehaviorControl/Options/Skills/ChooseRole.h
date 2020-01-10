option(ChooseRole)
{
  initial_state(elegir)
  {
    transition
    {
      if(theBehaviorStatus.role == Role::keeper){
          goto esPortero;
      }else if(theBehaviorStatus.role == Role::defender  || theBehaviorStatus.role == Role::supporter || theBehaviorStatus.role == Role::none){
          goto esDefensa;
      }else if(theBehaviorStatus.role == Role::striker){
          goto esDelantero;
      }
    }
    action
    {
      if(theBehaviorStatus.role == Role::undefined)
      {
        if(theRobotInfo.number == 1)
          theBehaviorStatus.role = Role::keeper;
        else if(theRobotInfo.number == 2)
          theBehaviorStatus.role = Role::defender;
        else if(theRobotInfo.number == 3)
          theBehaviorStatus.role = Role::defender;
        else if(theRobotInfo.number == 4)
          theBehaviorStatus.role = Role::defender;
        else if(theRobotInfo.number == 5)
          theBehaviorStatus.role = Role::striker;
        else if(theRobotInfo.number == 6)
          theBehaviorStatus.role = Role::defender;
      }
      LookForward();
    }
  }

  state(esPortero)
  {
    action
    {
      theBehaviorStatus.role = Role::keeper;

      aloneKeeper();
    }
  }

  state(esDefensa)
  {
    transition
    {
      if(theLibCodeRelease.closerToTheBall)
        goto esDelantero;
      
    }
    action
    {
      if(theBehaviorStatus.role != Role::supporter)
        theBehaviorStatus.role = Role::defender;
    
      Defender();
    }
  }
  state(esDelantero)
  {
    transition
    {
      
     if(!theLibCodeRelease.closerToTheBall)
        goto volvemosRufay;      
    }
    action
    {
      theBehaviorStatus.role = Role::striker;
      OffensiveSkills();
    }
  }

  state(volvemosRufay)
  {
    transition
    {
    	if (theLibCodeRelease.closerToTheBall)
        goto esDelantero;


      for(auto const& teammate : theTeamData.teammates){
        if(!teammate.isPenalized && teammate.theBallModel.tipo == Role::defender){
            if (std::abs(teammate.theRobotPose.translation.x()-theRobotPose.translation.x())<1000.f){
                goto esDefensa;
            }
        }
      }
      if (theLibCodeRelease.numberOfDefenders==0){
        goto esDefensa;
      }
    }
    action
    {
      theBehaviorStatus.role = Role::none;
      if((theLibCodeRelease.ball-theRobotPose.translation).norm()>theBehaviorParameters.minShootDistance && state_time<10000){
        StopBall();
        LookAround();
      }else{
        DynamicLook();
        theMotionRequest = thePathPlanner.plan(Pose2f(theLibCodeRelease.angleToOwnGoal,Vector2f(-3000,theRobotPose.translation.y())),Pose2f(1.f,1.f,1.f),false,true);  
      }
      
    }
  }
}
