option(Defender)
{
  Vector2f relativeBall =theRobotPose.inversePose*theLibCodeRelease.ball;

  initial_state(start)
  {
		transition
		{
			if(state_time > 100)
				goto whereIsIt;
		}
		action
    {
			LookAround();
			Stand();
		}
	}

  state(geneticDefense)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 1000)
        goto whereIsIt;

      if(!theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.closerToTheBall)
        goto despejando;
      if((theLibCodeRelease.theRivalIsCloserToTheBall && theBallModel.estimate.position.norm() < 400.f && theLibCodeRelease.closerToTheBall)
        || (theLibCodeRelease.numberOfDefences == 1 && state_time > 10000 && theLibCodeRelease.theRivalIsCloserToTheBall))
        goto attack4Defence;

      if(theTeamBallModel.position.x() > 500)
        goto followFromDistance;
    }
    action
    {
      float ballWeight = 1.2f;

      if(theLibCodeRelease.closerToTheBall && state_time > 800 && theLibCodeRelease.numberOfDefences > 1 && theLibCodeRelease.theRivalIsCloserToTheBall){
        if(theTeamBallModel.position.y() < -5 && theLibCodeRelease.defenderLefter)
          ballWeight = (float)(ballWeight + (state_time / 1000));
        if(theTeamBallModel.position.y() > 5 && theLibCodeRelease.defenderRighter)
          ballWeight = (float)(ballWeight + (state_time / 1000));
      }

      if(theLibCodeRelease.defenderLefter){
        theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
        theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
      }
      else if(theLibCodeRelease.defenderRighter){
        theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::keyFrame;
        theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = ArmKeyFrameRequest::back;
      }
      else{
        theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
        theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::keyFrame;//acosta o acevedo
        theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = ArmKeyFrameRequest::back;
        theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
      }

      // if(theBallModel.estimate.velocity.x() != 0 || theBallModel.estimate.velocity.y() != 0)
      //   timeAwait = state_time;

      // if(theLibCodeRelease.numberOfDefences == 1 && (state_time - timeAwait) > 1000000)
      //   ballWeight = 6.2f;

      SearchBall();
      theMotionRequest = thePathPlanner.plan(theGeneticLocator.activation(true, (int)theFrameInfo.time,ballWeight),Pose2f(0.85f, 0.85f, 0.85f),false,true);
    }
  }

  state(followFromDistance)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 1000)
        goto whereIsIt;

      if(theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.numberOfDefences > 1 && theBallModel.estimate.position.norm() < 400.f && theLibCodeRelease.closerToTheBall)
        goto attack4Defence;

      if(!theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.closerToTheBall)
        goto despejando;
      if(theTeamBallModel.position.x() < 450)
        goto geneticDefense;
    }
    action
    {
      HeadControlMode(HeadControl::lookForward);
      Vector2f going2 = {-2000.f,theTeamBallModel.position.y()};
      if(theLibCodeRelease.numberOfDefenders==3){
        if(theLibCodeRelease.defenderLefter && theRobotPose.translation.y() < 2700 && theRobotPose.translation.y() > -1700)
          going2 = {-3000.f,theTeamBallModel.position.y() + 700.f};
        else if(theLibCodeRelease.defenderRighter && theRobotPose.translation.y() > -2700 && theRobotPose.translation.y() < 1700)
          going2 = {-3000.f, theTeamBallModel.position.y() - 700.f};

       }else{
        if(theLibCodeRelease.defenderLefter && theRobotPose.translation.y() < 2700 && theRobotPose.translation.y() > -1700)
          going2 = {-2000.f,theTeamBallModel.position.y()};
        else if(theLibCodeRelease.defenderRighter && theRobotPose.translation.y() > -2700 && theRobotPose.translation.y() < 1700)
          going2 = {-3000.f, theTeamBallModel.position.y() - 700.f};
       }
      

      theMotionRequest = thePathPlanner.plan(Pose2f((theRobotPose.inversePose * ((Vector2f){theTeamBallModel.position.x(),theTeamBallModel.position.y()})).angle(), going2),Pose2f(0.6f, 0.6f, 0.6f),false,true);
    }
  }

  state(whereIsIt)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) < 500)
        goto geneticDefense;
      if(state_time > 12000)
        goto goBackHome;
    }
    action
    {
      TrackBall();
    }
  }

  state(goBackHome)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) < 500)
        goto geneticDefense;
    }
    action
    {
      Vector2f going2;

      if(theLibCodeRelease.numberOfDefenders == 1)
        going2 = (Vector2f){-2400,0};
      else if(theLibCodeRelease.numberOfDefenders == 2)
        going2 = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-2400,500} : (Vector2f){-2400,-500};
      else if(theLibCodeRelease.numberOfDefenders == 3)
        going2 = (theLibCodeRelease.getNumberWithinRole() == 2) ? (Vector2f){-2400,700} : (theLibCodeRelease.getNumberWithinRole() == 3) ? (Vector2f){-2400,-700} : (Vector2f){-2400,0};

      if(std::abs(theRobotPose.translation.x() - (going2[0]))  + std::abs(theRobotPose.translation.y() - (going2[1])) < 250){
        TrackBall();
      }
      else{
        WalkToTarget(Pose2f(0.8f, 0.6f, 0.6f),Pose2f((theRobotPose.inversePose * ((Vector2f){0,0})).angle(),theRobotPose.inversePose * going2));
      }
    }
  }

  state(attack4Defence)
  {
    transition
    {
      if(theBallModel.estimate.position.norm() >1000.f && !theLibCodeRelease.closerToTheBall){
        theBehaviorStatus.role= Role::defender;
        goto geneticDefense;
      }

      if(action_done || action_aborted)
        goto geneticDefense;
    }
    action
    {
      theBehaviorStatus.role = Role::supporter;
      Supporter();
    }
  }

  state(despejando)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 1000)
        goto whereIsIt;

      if(theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.numberOfDefences > 1 && theBallModel.estimate.position.norm() < 400.f && theLibCodeRelease.closerToTheBall)
        goto attack4Defence;

      if(relativeBall.norm() < 500.f)
        goto shoot;

      if(!theLibCodeRelease.closerToTheBall || theLibCodeRelease.theRivalIsCloserToTheBall)
        goto geneticDefense;
    }
    action
    {
      TrackBall();
      theMotionRequest = thePathPlanner.plan(Pose2f(0.f,theLibCodeRelease.ball),Pose2f(0.6f,0.6f,0.6f),false,true);

      theArmMotionRequest.armMotion[Arms::right] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armMotion[Arms::left] = ArmMotionRequest::keyFrame;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::left].motion = ArmKeyFrameRequest::back;
      theArmMotionRequest.armKeyFrameRequest.arms[Arms::right].motion = ArmKeyFrameRequest::back;
    }
  }

  state(shoot)
  {
    transition
    {
      if((theTeamBallModel.timeWhenLastValid - theFrameInfo.time) > 1000)
        goto whereIsIt;

      if(theLibCodeRelease.theRivalIsCloserToTheBall && theLibCodeRelease.numberOfDefences > 1 && theBallModel.estimate.position.norm() < 400.f && theLibCodeRelease.closerToTheBall)
        goto attack4Defence;


      if(theBallModel.estimate.position.norm()>600 || action_done || action_aborted )
        goto geneticDefense;
      if(!theLibCodeRelease.closerToTheBall)
        goto geneticDefense;
    }
    action
    {
      Vector2f target = Vector2f(4500.f, 0.f);

      if(!theLibCodeRelease.shootToGoal && false)
        target = theLibCodeRelease.bestTeammateForPass();


      LookBallTarget(theRobotPose.inversePose*target);
      Shoot();
    }
  }
}
