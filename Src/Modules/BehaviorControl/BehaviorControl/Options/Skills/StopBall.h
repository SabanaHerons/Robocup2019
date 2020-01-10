/**
Habilidad para ponerse en la posicion adecuada para recibir balon, pararlo y pegarle al arco:

*/
option(StopBall)
{
  /*
	relativeOrigin
	angleToPerfil: angulo para perfilarse (90_deg si la pelota esta en la izquierda)
  */
  float anguloDeseado =90_deg*(2*(theLibCodeRelease.ball.y()>=theRobotPose.translation.y())-1);
  float angleToPerfil= anguloDeseado-theRobotPose.rotation;
  if((anguloDeseado<0 && theRobotPose.rotation>0) || (anguloDeseado>0 && theRobotPose.rotation<0)){
    angleToPerfil= theRobotPose.rotation+anguloDeseado;
  }

  initial_state(setRequest)
  {
  	transition
  	{
      if(std::abs(angleToPerfil)>7_deg)
			   goto perfilar;

      if(theBallModel.estimate.position.norm()<400.f){
            if(theBallModel.estimate.velocity.norm() >500.f && theBallModel.estimate.velocity.x() < 0){
              goto goDown;
            }
        }
  	}
    action
    {
    	Stand();
    }
  }

  state(perfilar)
  {

  	transition
    	{
    		if(std::abs(angleToPerfil)<3_deg)
          goto setRequest;

        if(theBallModel.estimate.position.norm()<400.f){
            if(theBallModel.estimate.velocity.norm() >500.f && theBallModel.estimate.velocity.x() < 0){
              goto goDown;
            }
        }


  	}
    action
    {
     	WalkToTarget(Pose2f(0.9f,0.9f, 0.9f), Pose2f(angleToPerfil,  0.f,  0.f));
    }
  }

  state(goDown){
    transition{
        if(action_done)
            goto done;
    }
    action{
        SpecialAction(SpecialActionRequest::sitDown);
    }
  }

  target_state(done){
     transition{
      goto setRequest;
    }
    action{
      Stand();
    }
  }
}
