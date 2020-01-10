/**
Habilidad para caminar con el balon como HTWK Bp
Recibe:
	finalTarget: posicion global a la cual se desea llevar el balon caminando
-------------------------!!!!!!El robot debe estar cerca a la pelota!!!!!-----------------------
*/
option(DribbleBall, (const Vector2f&) (Vector2f(0,0))finalTarget)
{



  /*
  target--> paso intermedio entre para llegar al finalTarget (se calcula con el pathPlanner
	relativeTarget --> coordenadas del finalTarget cuasirelativas al jugador (no tienen la rotacion correcta)
	angleToTarget --> el angulo entre el eje X del robot y el finalTarget
  */

  MotionRequest  plan = thePathPlanner.plan(Pose2f(0.f,finalTarget),Pose2f(0.9f,0.9f,0.9f),false,false);
  Vector2f relativeTarget =plan.walkRequest.target.translation;

  float angleToTarget= plan.walkRequest.target.rotation;
  //Evitar oscilaciones del angulo debido a las oscilaciones del pathPlanner
  static float angleAlign =0;
  static int count=0;


  initial_state(setRequest)
  {

  	transition
  	{
      float pelota= theLibCodeRelease.rotateCoordenates(-angleToTarget,theBallModel.estimate.position).x();
      float objetivo= theLibCodeRelease.rotateCoordenates(-angleToTarget, relativeTarget).x();

      if(theLibCodeRelease.between(0.f, pelota, objetivo)){
        goto alignToTargetReverse;
      }else{
        goto alignToTarget;
      }

      if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut)
        goto abortedState;

  	}
    action
    {
     	TrackBall();
    }
  }
  aborted_state(abortedState){
    transition{
      if(theLibCodeRelease.timeSinceBallWasSeen <300)
          goto setRequest;
    }action{
      TrackBall();
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
      TrackBall();
      WalkToTarget(Pose2f(0.7f,0.7f, 0.7f), Pose2f(0.f,placeBehindBall));
    }
  }

  state(alignToTarget)
  {

    transition
    {
      angleAlign =0;
      count=0;
      if(std::abs(angleToTarget/2) < 10_deg && std::abs(theBallModel.estimate.position.y()) < 60.f)
       goto alignBehindBall;

    }
    action
    {
      TrackBall();
      WalkToTarget(Pose2f(0.7f,0.7f, 0.7f), Pose2f(angleToTarget/2, theBallModel.estimate.position.x() - 200.f, theBallModel.estimate.position.y()));
    }
  }


 state(alignBehindBall)
	{
    if(angleAlign==0){
      angleAlign= angleToTarget+theRobotPose.rotation;
      count=0;
    }
    //Hay gran cambio!
    if (std::abs(angleAlign-(angleToTarget+theRobotPose.rotation))>30_deg){
      //si es la primera vez se actualiza el global
      if(count==0){
        count++;
        angleAlign= angleToTarget+theRobotPose.rotation;
      }else{
        // si es alguna otra vez se cambia el relativo
        angleToTarget= angleAlign-theRobotPose.rotation;
      }
    }else{
      //si no hay gran cambio se actualiza el global
      angleAlign= angleToTarget+theRobotPose.rotation;
    }
    //se resetea el contador cada 2 segundos que haya estado fijo
    if((state_time/1000)%10==0)
          count=0;

		transition
		{
			if(theLibCodeRelease.between(theBallModel.estimate.position.y(), -30, 30) && std::abs(angleToTarget) < 4_deg)
				goto walk;
		}
		action
		{
      TrackBall();
			WalkToTarget(Pose2f(0.7f,0.7f, 0.7f), Pose2f(angleToTarget, theBallModel.estimate.position.x()-170.f , theBallModel.estimate.position.y()));
		}
	}


 state(walk)
	{
		transition
		{
      angleAlign =0;
      count=0;

      if(theLibCodeRelease.between(theLibCodeRelease.ball.y(), finalTarget.y()-70.f, finalTarget.y()+70.f)
        && theLibCodeRelease.between(theLibCodeRelease.ball.x(), finalTarget.x()-70.f, finalTarget.x()+70.f))
        goto done;
      if(theBallModel.estimate.position.norm()>200.f)
        goto setRequest;
      if(std::abs(angleToTarget) > 5_deg || (theRobotPose.inversePose * theLibCodeRelease.ball).x()<140.f)
        goto setRequest;
		}
		action
		{
      TrackBall();
			WalkToTarget(Pose2f(0.9f,0.9f, 0.9f), Pose2f(angleToTarget, theBallModel.estimate.position.x(), theBallModel.estimate.position.y()));
    }
	}

  target_state(done)  {
    transition{
       if(!theLibCodeRelease.between(theLibCodeRelease.ball.y(), finalTarget.y()-70.f, finalTarget.y()+70.f)
        || !theLibCodeRelease.between(theLibCodeRelease.ball.x(), finalTarget.x()-70.f, finalTarget.x()+70.f))
           goto setRequest;
    }
    action{
      TrackBall();
      Stand();
    }
  }
}
