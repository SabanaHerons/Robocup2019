option(IdealPosition, (int)(0) selector){

	Vector2f target = Vector2f(0,0);
	float rotacion= (theRobotPose.inversePose*theLibCodeRelease.ball).angle();

	common_transition{
		if(theLibCodeRelease.timeSinceBallWasSeen > theBehaviorParameters.ballNotSeenTimeOut && selector!=1){
				goto searchForBall;
		}else{
			switch(selector){
				case 1: //Goal Free Kick
					//coloca un robot a estorbar el resto se va!
					if(theLibCodeRelease.closerToTheBall)
			            {
			              Vector2f offset= Vector2f(-900.f,0);
						  float angletoRotate= (theLibCodeRelease.ball-Vector2f(theFieldDimensions.xPosOwnGroundline,0.f)).angle();
						  target = theLibCodeRelease.rotateCoordenates(angletoRotate,offset)+theLibCodeRelease.ball;
			            }else{
			            	goto sameReady;
			            }
					break;
				case 2: //Foul para el enemigo
					//si hay espacio para colocar obstruccion
					if(theLibCodeRelease.ball.x()>(theFieldDimensions.xPosOwnGroundline+1000.f)){
						//el mas cercano que vaya a cubrir en direccion al arco
						//el resto se va a posicion de ready
						if(theLibCodeRelease.closerToTheBall)
			            {
			              Vector2f offset= Vector2f(-900.f,0);
						  float angletoRotate= (theLibCodeRelease.ball-Vector2f(theFieldDimensions.xPosOwnGroundline,0.f)).angle();
						  target = theLibCodeRelease.rotateCoordenates(angletoRotate,offset)+theLibCodeRelease.ball;
			            }else{
			            	goto sameReady;
			            }
					}else{ // si no se puedo todo van a ready
						goto sameReady;
					}
					break;
				case 3://Corner aliado

					goto asignar;
					break;
				case 4://Corner enemigo
					//Striker y arquero hacen lo mismo
					//Los defensas hacen bullying .-.
					if(theBehaviorStatus.role == Role::defender){

						int direccion = (2*(theLibCodeRelease.ball.y()>0)-1);//1 si corner izquierdo -1 si derecho
						//Vector2f ownCorner= Vector2f(theFieldDimensions.xPosOwnGroundline,theFieldDimensions.yPosLeftFieldBorder*direccion);
						Vector2f offset= Vector2f(0,-900.f * direccion);
						float angletoRotate= 15_deg+theLibCodeRelease.getNumberWithinRole()*15_deg;

						target = theLibCodeRelease.rotateCoordenates(angletoRotate*direccion,offset)+theLibCodeRelease.ball;
		   			}else{
						goto asignar;
					}
					break;
				case 5://Saque de banda enemigo
					//el mas cercano que vaya a cubrir en direccion al arco
					//el resto se va a posicion de ready
					if(theLibCodeRelease.closerToTheBall)
		            {
		              Vector2f offset= Vector2f(-900.f,0);
					  float angletoRotate= (theLibCodeRelease.ball-Vector2f(theFieldDimensions.xPosOwnGroundline,0.f)).angle();
					  target = theLibCodeRelease.rotateCoordenates(angletoRotate,offset)+theLibCodeRelease.ball;
		            }else{
		            	goto sameReady;
		            }
					break;
		        default:
		        	break;
			}
			if((target-theRobotPose.translation).norm()<150.f){
				if(std::abs(theRobotPose.rotation-rotacion)<7_deg){
					goto done;
				}else{
					goto rotate;
				}
			}else{
				goto goToPlace;
			}
		}
	}

	initial_state(goToPlace){
		transition{

		}
		action{
			DynamicLook();
			theMotionRequest = thePathPlanner.plan(Pose2f(0.f,target),Pose2f(0.9f,0.9f,0.9f),false,true);
		}
	}

	state(sameReady){
		transition{

		}
		action{ // Todos se van a ready excepto el arquero... por si algo!
			if(theBehaviorStatus.role == Role::keeper){
				Keeper();
			}else{
				ReadyState();
			}
		}
	}

	state(rotate){
		transition{

		}
		action{
			LookForward();
			WalkToTarget(Pose2f(0.6f,0.6f, 0.6f), Pose2f(rotacion,0.f,0.f));
		}
	}

	target_state(done){
		transition{

		}
		action{
			Stand();
		}
	}

	state(asignar)
  	{
      transition
      {

      }
      action
      {
          ChooseRole();
      }
  	}

  state(searchForBall)
  {
    transition
    {

    }
    action
    {
      TrackBall();
    }
  }
}
