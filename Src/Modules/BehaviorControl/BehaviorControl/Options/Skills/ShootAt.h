/**
Habilidad para ponerse en la posicion adecuada para pegarle a balon:
Recibe:
	target: posicion global a la cual se desea disparar
	wait: si dispara en cuanto este listo o no
-------------------------!!!!!!El robot debe estar cerca a la pelota!!!!!-----------------------
*/
option(ShootAt, (const Vector2f&) target , (const bool&) wait)
{
  /*
	relativeTarget --> coordenadas del target cuasirelativas al jugador (no tienen la rotacion correcta)
	angleToTarget --> el angulo entre el eje X del robot y el target
  */
  Vector2f relativeTarget = target- theRobotPose.translation;
  Vector2f rotacionUnitariaRobot= Vector2f(cos(theRobotPose.rotation),sin(theRobotPose.rotation));
  float angleToTarget= (float) acos((relativeTarget.x()*rotacionUnitariaRobot.x()+relativeTarget.y()*rotacionUnitariaRobot.y())/(relativeTarget.norm()*rotacionUnitariaRobot.norm()));

  //rotar las coordenadas cuasirelativas, compara el y relativo del target para definir angulo
  if ((relativeTarget.x()*sin(theRobotPose.rotation)-relativeTarget.y()*cos(theRobotPose.rotation))>0){
  	angleToTarget*=-1;
  }

  bool izquierda= (theRobotPose.inversePose*theLibCodeRelease.ball).y()>=0;

   initial_state(setRequest)
  {

  	transition
  	{
  		if(theBallModel.estimate.position.norm() < (theBehaviorParameters.distanceToGoShoot+100.f)){

        float pelota= theLibCodeRelease.rotateCoordenates(-angleToTarget,theBallModel.estimate.position).x();
        float objetivo= theLibCodeRelease.rotateCoordenates(-angleToTarget, theRobotPose.inversePose * target).x();

        if(theLibCodeRelease.between(0.f, pelota, objetivo)){
          goto alignToTargetReverse;
        }else{
          goto alignToTarget;
        }

      }else{
        goto abortedState;
      }
  	}
    action
    {

    }
  }
  aborted_state(abortedState){
    transition{
      goto setRequest;
    }
  }

  state(alignToTargetReverse)
  {
  	Vector2f placeBehindBall= theLibCodeRelease.rotateCoordenates(angleToTarget*0.6f,Vector2f(- 250.f,0.f))+theBallModel.estimate.position;
  	transition
  	{

  		if(placeBehindBall.norm() < 30.f)
			 goto alignToTarget;

  	}
    action
    {

      WalkToTarget(Pose2f(0.7f,0.7f, 0.7f), Pose2f(angleToTarget/2,placeBehindBall));
    }
  }

  state(alignToTarget)
  {

    transition
    {
      if(std::abs(angleToTarget/2) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 100.f)
       goto alignBehindBall;

    }
    action
    {
      WalkToTarget(Pose2f(0.7f,0.7f, 0.7f), Pose2f(angleToTarget/2, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y()));
    }
  }


 state(alignBehindBall)
	{
		transition
		{
			if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -50.f + izquierda*70, -20.f+izquierda*70)
				&& theLibCodeRelease.between(theBallModel.estimate.position.x(), 170.f, 180.f)
				&& std::abs(angleToTarget) < 2_deg)
				goto kick;
		}
		action
		{
			WalkToTarget(Pose2f(0.5f,0.5f, 0.5f), Pose2f(angleToTarget, theBallModel.estimate.position.x() - 175.f, theBallModel.estimate.position.y() + 36.f-72*izquierda));
		}
	}


 target_state(kick)
	{
		transition
		{
      if(state_time>5000.f){
        if(theBallModel.estimate.position.norm() >(theBehaviorParameters.distanceToGoShoot))
			   goto done;
       else
        goto setRequest;
      }
		}
		action
		{
			//InWalkKick(WalkKickVariant(WalkKicks::turnOut, Legs::left), Pose2f(angleToTarget, theBallModel.estimate.position.x() , theBallModel.estimate.position.y() - 30.f));
      theMotionRequest.motion = MotionRequest::kick;
      theMotionRequest.kickRequest.kickMotionType = KickRequest::kickForward;
      theMotionRequest.kickRequest.mirror = izquierda;

    /*
      InWalkKick(WalkKickVariant(WalkKicks::forwardShoot, Legs::left), Pose2f(angleToTarget, theBallModel.estimate.position.x() - 160.f, theBallModel.estimate.position.y() - 55.f));
    */
    }
	}

  state(done){
     transition{
      goto setRequest;
    }
  }

}
