option(ReadyState, (bool)(false) first)
{
  float i = 0.f;

  initial_state(stand)
  {
    transition
    {
      if(state_time > 10000)
        goto ubicate;
    }
    action
    {
      LookAround();
    }
  }

  state(ubicate)
  {
    transition
    {
      if(state_time>5000 && theGameInfo.kickingTeam == theOwnTeamInfo.teamNumber)
        goto rotate2Offensivepoint;

      i = 0 + rand() % (2 - 0);
      if(state_time>5000 && theGameInfo.kickingTeam != theOwnTeamInfo.teamNumber)
        goto rotate2Defensivepoint;
    }
    action
    {
      LookAround();
      if(first)
        WalkAtRelativeSpeed(Pose2f(0.f, 1.f, 0.f));
    }
  }

  state(rotate2Offensivepoint)
  {
    Vector2f objective;

    switch(theBehaviorStatus.role)
    {
      case Role::keeper:
        objective = {theFieldDimensions.xPosOwnGroundline,0};
        break;

      case Role::defender:
        if(theLibCodeRelease.numberOfDefenders == 1)
          objective = (Vector2f){-3050,0};
        else if(theLibCodeRelease.numberOfDefenders == 2)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,600} : (Vector2f){-3000,-600};
        else if(theLibCodeRelease.numberOfDefenders == 3)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,800} : (theLibCodeRelease.getNumberWithinRole() == 3) ? (Vector2f){-3000,-800} : (Vector2f){-3000,0};
      break;

      case Role::striker:
        if(theLibCodeRelease.numberOfStrikers == 1)
          objective = (Vector2f){-1000,0};

        else if(theLibCodeRelease.numberOfStrikers == 2){
          if(i>0.5)
            objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-750,0} : (Vector2f){-100,1700};
          else
            objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-750,0} : (Vector2f){-100,-1700};
        }
        break;

      default:
        objective = theRobotPose.translation;
        break;
    }

    transition
    {
      if(std::abs(((Vector2f){objective[0]-theRobotPose.translation.x(),objective[1]-theRobotPose.translation.y()}).angle() - theRobotPose.rotation) < 20_deg)
        goto initialOffensivePosition;
    }
    action
    {
      WalkToTarget(Pose2f(1.f, 1.f, 1.f),Pose2f((theRobotPose.inversePose * objective).angle(), 0.f, 0.f));
    }
  }

  state(initialOffensivePosition)
  {
    Vector2f objective;

    switch(theBehaviorStatus.role)
    {
      case Role::keeper:
        objective = {theFieldDimensions.xPosOwnGroundline,0};
        break;

      case Role::defender:
        if(theLibCodeRelease.numberOfDefenders == 1)
          objective = (Vector2f){-3050,0};
        else if(theLibCodeRelease.numberOfDefenders == 2)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,600} : (Vector2f){-3000,-600};
        else if(theLibCodeRelease.numberOfDefenders == 3)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,800} : (theLibCodeRelease.getNumberWithinRole() == 3) ? (Vector2f){-3000,-800} : (Vector2f){-3000,0};
      break;

      case Role::striker:
        if(theLibCodeRelease.numberOfStrikers == 1)
          objective = (Vector2f){-1000,0};

        else if(theLibCodeRelease.numberOfStrikers == 2){
          if(i>0.5)
            objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-750,0} : (Vector2f){-100,1700};
          else
            objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-750,0} : (Vector2f){-100,-1700};
        }
      break;

      default:
        objective = theRobotPose.translation;
        break;
    }

    transition
    {
      if(std::abs(((Vector2f){objective[0]-theRobotPose.translation.x(),objective[1]-theRobotPose.translation.y()}).angle() - theRobotPose.rotation) > 25_deg)
        goto rotate2Offensivepoint;
      if(std::abs((theRobotPose.translation.x() - objective[0])) + std::abs((theRobotPose.translation.y() - objective[1])) < 300)
        goto rotate2Zero;
    }

    action
    {
      if(std::abs(theRobotPose.translation.x() - (objective[0]))  + std::abs(theRobotPose.translation.y() - (objective[1])) < 200){
        LookAround();
        Stand();
      }else{
        theMotionRequest = thePathPlanner.plan(Pose2f(0.f,objective),Pose2f(0.9f,0.9f,0.9f),false,true);
        HeadControlMode(HeadControl::lookForward);
      }

    }
  }

  state(rotate2Defensivepoint)
  {
    Vector2f objective;
    switch(theBehaviorStatus.role)
    {
      case Role::keeper:
        objective = {theFieldDimensions.xPosOwnGroundline,0};
        break;

      case Role::defender:
        if(theLibCodeRelease.numberOfDefenders == 1)
          objective = (Vector2f){-3050,0};
        else if(theLibCodeRelease.numberOfDefenders == 2)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,500} : (Vector2f){-3000,-500};
        else if(theLibCodeRelease.numberOfDefenders == 3)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,700} : (theLibCodeRelease.getNumberWithinRole() == 3) ? (Vector2f){-3000,-700} : (Vector2f){-3000,0};
        break;

      case Role::striker:
        if(theLibCodeRelease.numberOfStrikers == 1)
          objective = (Vector2f){-1000,0};
        if(theLibCodeRelease.numberOfStrikers == 2)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-1000,0} : (Vector2f){-1100,500};
      break;
      default:
        objective = theRobotPose.translation;
        break;
    }

    transition
    {
      if(std::abs(((Vector2f){objective[0]-theRobotPose.translation.x(),objective[1]-theRobotPose.translation.y()}).angle() - theRobotPose.rotation) < 20_deg)
        goto initialDeffensivePosition;
    }
    action
    {
      WalkToTarget(Pose2f(1.f, 1.f, 1.f),Pose2f((theRobotPose.inversePose * objective).angle(), 0.f, 0.f));
    }
  }

  state(initialDeffensivePosition)
  {
    Vector2f objective;
    switch(theBehaviorStatus.role)
    {
      case Role::keeper:
        objective = {theFieldDimensions.xPosOwnGroundline,0};
        break;

      case Role::defender:
        if(theLibCodeRelease.numberOfDefenders == 1)
          objective = (Vector2f){-3050,0};
        else if(theLibCodeRelease.numberOfDefenders == 2)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,500} : (Vector2f){-3000,-500};
        else if(theLibCodeRelease.numberOfDefenders == 3)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-3000,700} : (theLibCodeRelease.getNumberWithinRole() == 3) ? (Vector2f){-3000,-700} : (Vector2f){-3000,0};
        break;

      case Role::striker:
        if(theLibCodeRelease.numberOfStrikers == 1)
          objective = (Vector2f){-1000,0};
        else if(theLibCodeRelease.numberOfStrikers == 2)
          objective = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-1000,0} : (Vector2f){-1100,500};
      break;
      default:
        objective = theRobotPose.translation;
        break;
    }

    transition
    {
      if(std::abs(((Vector2f){objective[0]-theRobotPose.translation.x(),objective[1]-theRobotPose.translation.y()}).angle() - theRobotPose.rotation) > 25_deg)
        goto rotate2Defensivepoint;
      if(std::abs((theRobotPose.translation.x() - objective[0])) + std::abs((theRobotPose.translation.y() - objective[1])) < 300)
        goto rotate2Zero;
    }

    action
    {
      if(std::abs(theRobotPose.translation.x() - (objective[0]))  + std::abs(theRobotPose.translation.y() - (objective[1])) < 200){
        LookAround();
        Stand();
      }else{
        theMotionRequest = thePathPlanner.plan(Pose2f(0.f,objective),Pose2f(0.9f,0.9f,0.9f),false,true);
        HeadControlMode(HeadControl::lookForward);
      }
    }
  }

  state(rotate2Zero)
  {
    action
    {
      WalkToTarget(Pose2f(0.5f, 0.5f, 0.5f), Pose2f((theRobotPose.inversePose * (Vector2f){4500,0}).angle(), 0.f, 0.f));
    }
  }
}
