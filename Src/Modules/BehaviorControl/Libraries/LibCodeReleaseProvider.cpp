/**
 * @file LibCodeRelease.cpp
 */

#include "LibCodeReleaseProvider.h"
#include "Tools/Math/Geometry.h"
#include <iostream>

MAKE_MODULE(LibCodeReleaseProvider, behaviorControl);

void LibCodeReleaseProvider::update(LibCodeRelease& libCodeRelease)
{
  if(theBallModel.timeWhenLastSeen>theTeamBallModel.timeWhenLastSeen){
    libCodeRelease.timeSinceBallWasSeen = theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen);
  }else{
    libCodeRelease.timeSinceBallWasSeen = theFrameInfo.getTimeSince(theTeamBallModel.timeWhenLastSeen);
  }

  libCodeRelease.angleToOppGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  libCodeRelease.angleToOwnGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnGroundline, 0.f)).angle();

  //Néstor
  libCodeRelease.distanceFromLine = lineDistancer();
  libCodeRelease.defenderLefter = whoDefenderAtLeft();
  libCodeRelease.defenderRighter = whoDefenderAtRight();
  libCodeRelease.numberOfDefences = howManyDefences();
  libCodeRelease.intersectoEnYGlobal = whereItIntersects();
  libCodeRelease.intersectoEnYGlobalRelative = whereItIntersectsRelative();
  libCodeRelease.timeWithBallNotMoving = timeWithAStaticBall();

  //Omar
  whereToShootL = getPlaceToShoot();
  closerToTheBall = getCloserToTheBall();
  desiredPosL = libCodeRelease.desiredPos;

  libCodeRelease.whereToShoot=whereToShootL;
  libCodeRelease.nearestEnemy = nearestEnemyL;
  libCodeRelease.areShooting= isAboutToShoot();

  //Coronate
  prevRivalHasTheBall = libCodeRelease.theRivalHasTheBall;

  libCodeRelease.theRivalHasTheBall= hasRivalTheBall();
  libCodeRelease.theRivalIsCloserToTheBall= rivalNearestToBall();
  libCodeRelease.numberOfDefenders = howManyDefenders();
  libCodeRelease.numberOfStrikers = howManyStrikers();
  libCodeRelease.numberOfKeepers = howManyKeepers();


  //Valentina
  libCodeRelease.rightDive = toRight();
  libCodeRelease.leftDive = toLeft();
  libCodeRelease.needGoDown = toDown();

  //Defecto

  libCodeRelease.angleToGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  
  if(timeStampCloser==0){
    timeStampCloser=theFrameInfo.time;
    libCodeRelease.closerToTheBall = isCloserToTheBall();
    prevCloserToBall =  libCodeRelease.closerToTheBall;
  }else{
    if(theFrameInfo.getTimeSince(timeStampCloser)>5000){
      timeStampCloser=theFrameInfo.time;
      libCodeRelease.closerToTheBall = isCloserToTheBall();
      prevCloserToBall =  libCodeRelease.closerToTheBall;
    }else{
      libCodeRelease.closerToTheBall= prevCloserToBall;
    }
  }
 
  libCodeRelease.ballInOppField = ballInOppField();
  libCodeRelease.ballInsideOwnGoal = ballInsideOwnGoal();
  libCodeRelease.keeperInsideGoal = keeperInsidePenaltyArea();
  libCodeRelease.shootToGoal= mustShootToGoal();
  libCodeRelease.desiredPos= getDesiredPosition();
  libCodeRelease.ball=getBall();


  libCodeRelease.between = [&](float value, float min, float max) -> bool
  {
      return value >= min && value <= max;
  };
  libCodeRelease.clamp = [&](float value, float min, float max) -> float
  {
      if(min > max)
      {
        float tmp = max;
        max = min;
        min = tmp;
      }
      if(value <= min)
        return min;
      else if(value >= max)
        return max;
      else
        return value;
  };

  libCodeRelease.rotateCoordenates = [&](float angle,Vector2f coordenada)-> Vector2f
  {
    float newX = (float) (coordenada.x()*cos(angle)-coordenada.y()*sin(angle));
    float newY = (float) (coordenada.x()*sin(angle)+coordenada.y()*cos(angle));
    return Vector2f (newX,newY);
  };
  libCodeRelease.getNumberWithinRole = [&]()-> int
  {
    //Da numero a los miembros del rol  de menor numero a mayor
    //i.e. el de mas a la derecha es 1, el del medio es 2 y el de la izquierda es 3
    int number=1;
    for(auto const& teammate : theTeamData.teammates)
    {
      if(!teammate.isPenalized && teammate.theBallModel.tipo == theBehaviorStatus.role)
        if(theRobotInfo.number>teammate.number)
          number++;
    }
    return number;
  };



  libCodeRelease.isThatClear = [&](Vector2f coordenada, float distanceToBeClear)-> bool
  {
    return isThatClear(coordenada,distanceToBeClear);
  };

  /*
    BEST TEAMMATE FOR PASS
  */
  libCodeRelease.bestTeammateForPass = [&]()-> Vector2f
  {
    Vector2f coordenada= Vector2f(-100000,0);
      for(auto const& teammate : theTeamData.teammates)
      {
        if(!teammate.isPenalized){
          //si el compañero esta libre y en una posicion mas adelantada
          if(isThatClear(teammate.theRobotPose.translation,theBehaviorParameters.distanceToBeClear) && teammate.theRobotPose.translation.x()>theRobotPose.translation.x()){
            //y si el compañero es el que esta mas alejado
            if(teammate.theRobotPose.translation.x()>coordenada.x() && teammate.theRobotPose.translation.norm()<theBehaviorParameters.maxShootDistance && teammate.theRobotPose.translation.norm()>theBehaviorParameters.minShootDistance){
              coordenada=teammate.theRobotPose.translation;
            }
          }
        }
      }

      //si no hay espacio disparar hacia el arco y ya (siempre y cuando no sea stricker, ellos salen a correr con la pelota)!
      if(coordenada.x()==-100000 && theBehaviorStatus.role!=Role::striker)
        coordenada=whereToShootL;
    return coordenada;
  };

}

//===Néstor===

float LibCodeReleaseProvider::lineDistancer(){
  //Si estoy en la trayectoria
  //Ry = (Rx*(vector[1]/vector[0])) + (punto[1] + (-1 * punto[0] * (vector[1]/vector[0])));

  float distancia = 0;
  Vector2f punto = {theBallModel.estimate.position.y(),theBallModel.estimate.position.x()};
  Vector2f vector = {theBallModel.estimate.velocity.y(), theBallModel.estimate.velocity.x()};

  if(std::abs(vector[0]) > 0.5){
    float pendiente = vector[1]/vector[0];
    float intersecto = (punto[1] + (-1 * punto[0] * (vector[1]/vector[0])));

    distancia = (float)(std::abs(intersecto) / (sqrt(pow(pendiente,2) + 1)));
  }

  return distancia;
}
bool LibCodeReleaseProvider::whoDefenderAtLeft(){
  for (auto const& teammate : theTeamData.teammates) {
    if((teammate.status == Teammate::PLAYING &&
      teammate.theBallModel.tipo == Role::defender &&
      teammate.theRobotPose.translation.y() > theRobotPose.translation.y()) ||
      teammate.theBallModel.tipo == Role::supporter)
        return false;
  }
  return true;
}
bool LibCodeReleaseProvider::whoDefenderAtRight(){
  for (auto const& teammate : theTeamData.teammates) {
    if((teammate.status == Teammate::PLAYING &&
      teammate.theBallModel.tipo == Role::defender &&
      teammate.theRobotPose.translation.y() < theRobotPose.translation.y()) ||
      teammate.theBallModel.tipo == Role::supporter)
        return false;
  }
  return true;
}
int LibCodeReleaseProvider::howManyDefences(){
  int number = 0;

  for (int i = 0; i < int(theTeamData.teammates.size()); i++)
    if(theTeamData.teammates[i].status == Teammate::PLAYING && theTeamData.teammates[i].theBallModel.tipo == Role::defender)
      number++;

  if(theBehaviorStatus.role == Role::defender)
    number++;

  return number;
}
float LibCodeReleaseProvider::whereItIntersectsRelative(){
  float intersecto = 890000.f;

  if(theBallModel.estimate.velocity.x()<-50.f){
    float pendiente = theBallModel.estimate.velocity.y()/theBallModel.estimate.velocity.x();
    intersecto = (theBallModel.estimate.position.y() + (-1 * theBallModel.estimate.position.x() * pendiente));

    intersecto = (pendiente * (0)) + intersecto;
  }

  return intersecto;
}
float LibCodeReleaseProvider::whereItIntersects(){
  float intersecto = 50000.f;

  if(theTeamBallModel.velocity.x()<-550.f && theTeamBallModel.position.x() < -750){
    float pendiente = theTeamBallModel.velocity.y()/theTeamBallModel.velocity.x();
    intersecto = (theTeamBallModel.position.y() + (-1 * theTeamBallModel.position.x() * pendiente));

    intersecto = (pendiente * (-3750)) + intersecto;
  }

  return intersecto;
}
bool LibCodeReleaseProvider::timeWithAStaticBall(){
  return false;
}
//===Omar===
Vector2f LibCodeReleaseProvider::getPlaceToShoot(){
  /*
    Devuelve un lugar del arco enemigo que este libre para el tiro!
  */
  //inicializa whereToShoot
  if (whereToShootL.x()==0)
    return Vector2f(theFieldDimensions.xPosOpponentGroundline,0.f);

  //Verifica si ese lugar es aun es valido
  if(isThatClear(whereToShootL,theBehaviorParameters.distanceToBeClearForShoot)){
    return whereToShootL;
  }else{
    //Si ya no esta despejado busca un nuevo lugar
    Vector2f position= Vector2f(0.f,0.f);
    int count = 0;
    while(count<200){
      float y = (std::rand()%1000)-500.f;
      position= Vector2f(theFieldDimensions.xPosOpponentGroundline+200.f,y);
      if(isThatClear(position,theBehaviorParameters.distanceToBeClearForShoot)){
        return position;
      }
      count++;
    }
  }

  //si no encuentra nada deja el disparo a la mitad
  return Vector2f(theFieldDimensions.xPosOpponentGroundline+200.f,0.f);
}

float LibCodeReleaseProvider::getDistanceToLine(Vector2f left, Vector2f right, float pendiente){
  float distanciaLeft= (float) (std::abs(pendiente*left.x()-left.y())/sqrt(pendiente*pendiente+1));
  float distanciaRight= (float) (std::abs(pendiente*right.x()-right.y())/sqrt(pendiente*pendiente+1));
  if(distanciaLeft<distanciaRight){
    return distanciaLeft;
  }
  return distanciaRight;
}
bool LibCodeReleaseProvider::isAboutToShoot(){


  //verfica si el robot que tiene el balon esta casi en posicion para hacer el pase
  //tambien verifica que uno este libre para el pase
  //se utiliza para detener el robot para recibir el pase!
  Vector2f ball =  getBall();

  if((closerToTheBall.theRobotPose.translation -ball).norm()<200.f && std::abs(closerToTheBall.theRobotPose.rotation)<110_deg)
    return true;
  return false;
}


Vector2f LibCodeReleaseProvider::getBall(){

  Vector2f ball= theTeamBallModel.position;
  if(std::isnan(theTeamBallModel.position.x()) || (theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen)<=theBehaviorParameters.ballNotSeenTimeOut)){
    ball= theRobotPose * theBallModel.estimate.position;
  }
  return ball;
}
//===Coronate===

bool LibCodeReleaseProvider::hasRivalTheBall(){
  float threshold = theBehaviorParameters.ballEnemy;
  if(prevRivalHasTheBall){
    threshold*= 1.2f;
  }

  //Locally see if the rival has the ball
  if(!theObstacleModel.obstacles.empty()){
    for(const auto& obstacle : theObstacleModel.obstacles){
      if((theBallModel.estimate.position - obstacle.center).norm()<threshold){
        if(obstacle.type!= Obstacle::Type::teammate && obstacle.type!=Obstacle::Type::goalpost){
          nearestEnemyL=theRobotPose * obstacle.center;
          return true;
        }
      }
    }
  }

  //Ask the partners if the rival has the ball
  for(auto const& teammate : theTeamData.teammates){
    if(!teammate.isPenalized){
      if(!teammate.theObstacleModel.obstacles.empty()){
        for(const auto& obstacle : teammate.theObstacleModel.obstacles){
          if((teammate.theBallModel.estimate.position - obstacle.center).norm()<threshold){
            //POSIBLES PROBLEMAS!
            bool soyYoObstaculo= ((teammate.theRobotPose * obstacle.center)-theRobotPose.translation).norm()<150.f;
            if(obstacle.type!= Obstacle::Type::teammate && !soyYoObstaculo && obstacle.type!=Obstacle::Type::goalpost){
              nearestEnemyL=teammate.theRobotPose * obstacle.center;
              return true;
            }
          }
        }
      }
    }
  }

  //si no entonces el rival no la tiene
  nearestEnemyL=Vector2f(1000.f,1000.f);
  return false;
}

bool LibCodeReleaseProvider::rivalNearestToBall(){
  //posición relativa para que un robot sepa si debe bloquear o patear.
  if(!theObstacleModel.obstacles.empty()){
    for(const auto& obstacle : theObstacleModel.obstacles){
      if((theBallModel.estimate.position - obstacle.center).norm()<theBallModel.estimate.position.norm()){
        return true;
      }
    }
  }

  //si no, nuestro robot está más cerca y puede ir a patear AKA. cambio rol..???
  return false;
}


//===Valentina===
bool LibCodeReleaseProvider::toRight(){
  return ((-100 > theBallModel.estimate.position.y()) && (theBallModel.estimate.velocity.x() < -200)
        && (theBallModel.estimate.position.x() < 1200));
}
bool LibCodeReleaseProvider::toLeft(){
  return ((100 < theBallModel.estimate.position.y()) && (theBallModel.estimate.velocity.x() < -200)
        && (theBallModel.estimate.position.x() < 900));
}
bool LibCodeReleaseProvider::toDown(){
  return ((100 > theBallModel.estimate.position.y()) && (-100 < theBallModel.estimate.position.y())
    && (theBallModel.estimate.velocity.x() < -200) && (2500 > theBallModel.estimate.position.x()));
}

//===Defecto===

bool LibCodeReleaseProvider::ballInOppField(){

  bool *ballInOppField = new bool ;

  if(theTeamBallModel.position.x() > 0){
    *ballInOppField = true ;
  }else{
    *ballInOppField = false;
  }

  return *ballInOppField;
}

bool LibCodeReleaseProvider::ballInsideOwnGoal(){

  bool *ballInsideOwnGoal = new bool;


  if ( Geometry::isPointInsideRectangle2(Vector2f(theFieldDimensions.xPosOwnGroundline, theFieldDimensions.yPosLeftPenaltyArea),
    Vector2f(theFieldDimensions.xPosOwnPenaltyArea, theFieldDimensions.yPosRightPenaltyArea),
    theTeamBallModel.position))
  {
    *ballInsideOwnGoal = true;
  }else{
    *ballInsideOwnGoal = false;
  }


return *ballInsideOwnGoal;
}

bool LibCodeReleaseProvider::keeperInsidePenaltyArea()
{
	bool *keeperInside = new bool;

	if (theRobotInfo.number == 1 || theBehaviorStatus.role == Role::keeper) {
		if (theRobotPose.translation.x() > (theFieldDimensions.xPosOwnPenaltyArea - 50)
			|| theRobotPose.translation.y() > (theFieldDimensions.yPosLeftPenaltyArea - 250)
			|| theRobotPose.translation.y() < (theFieldDimensions.yPosRightPenaltyArea + 250))
		{
			*keeperInside = false;
		}
		else
		{
			*keeperInside = true;
		}
	}

	return *keeperInside;
	//return true;

	//return false;
}

Teammate LibCodeReleaseProvider::getCloserToTheBall(){
  double teammateDistanceToBall = 0.0;

  Teammate closerToTheBall;
  float minDistance= 100000.f;

  for(auto const& teammate : theTeamData.teammates)
  {
    if(!teammate.isPenalized){
      teammateDistanceToBall = (teammate.theRobotPose.inversePose*theTeamBallModel.position).norm();

      if(theFrameInfo.getTimeSince(teammate.theBallModel.timeWhenLastSeen)<=theBehaviorParameters.ballNotSeenTimeOut){
       teammateDistanceToBall = teammate.theBallModel.estimate.position.norm();
      }

      if(minDistance > teammateDistanceToBall)
      {
        closerToTheBall=teammate;
      }
    }
  }

  return closerToTheBall;
}

bool LibCodeReleaseProvider::isCloserToTheBall()
{

  double teammateDistanceToBall = 0.0;

  distanceToBall= (theRobotPose.inversePose*theTeamBallModel.position).norm();

  if(theFrameInfo.getTimeSince(theBallModel.timeWhenLastSeen)<=theBehaviorParameters.ballNotSeenTimeOut){
    distanceToBall= theBallModel.estimate.position.norm();
  }


  for(auto const& teammate : theTeamData.teammates)
  {
    if(!teammate.isPenalized){
      teammateDistanceToBall = (teammate.theRobotPose.inversePose*theTeamBallModel.position).norm();

      if(theFrameInfo.getTimeSince(teammate.theBallModel.timeWhenLastSeen)<=theBehaviorParameters.ballNotSeenTimeOut){
       teammateDistanceToBall = teammate.theBallModel.estimate.position.norm();
      }

      if(distanceToBall > teammateDistanceToBall)
      {
        return false;
      }
    }
  }
  return true;
}

bool LibCodeReleaseProvider::mustShootToGoal(){

  bool shootToGoal = true;
  bool skiped = false;//ignorar al arquero
  Vector2f goal=(theRobotPose.inversePose * whereToShootL);
  float angleToGoal =  (Vector2f(theFieldDimensions.xPosOpponentGroundline+200.f, 0.f)-getBall()).angle();

  //si no esta cerca evalue el tiro si esta cerca intente meter gol siempre
  if(goal.norm()>1000.f){
      //si el angulo hacia el arco enemigo es muy alto (un corner) no disparar
    if(std::abs(angleToGoal)<50_deg){

      if(goal.norm()<theBehaviorParameters.maxShootDistance){
        if(!theObstacleModel.obstacles.empty()){
          float pendiente= goal.y()/goal.x();
          for(const auto& obstacle : theObstacleModel.obstacles){
            //See if the obstacle is first than the target
            if (goal.norm()>obstacle.center.norm()){
              //See if the obstacle is in the way to the target
              float distanciaACamino = getDistanceToLine(obstacle.left, obstacle.right, pendiente);
              if(distanciaACamino<theBehaviorParameters.distanceToBeClearForShoot && obstacle.type!=Obstacle::Type::goalpost){
                //ignora al arquero!
                if ((goal-obstacle.center).norm()<700.f && !skiped){
                  skiped=true;
                }else {
                  shootToGoal=false;
                }
              }
            }
          }
        }
      }else{
        shootToGoal=false;
      }
    }else{
      shootToGoal=false;
    }
  }else{
    shootToGoal=true;
  }

  return shootToGoal;
}

Vector2f LibCodeReleaseProvider::getDesiredPosition(){
  float positionX = 0;
  float amplitud = 1.f;

  if(theTeamBallModel.position.x()<-2000.f){
    positionX= theBehaviorParameters.nearestPosStriker;
  }else if(theTeamBallModel.position.x()<1000.f){
    positionX= theBehaviorParameters.middlePosStriker;
    amplitud = 0.7f;
  }else{
    positionX= theBehaviorParameters.farPosStriker;
    amplitud=0.5f;
  }

  Vector2f origen = getBall();
  int fieldSide = (origen.y()>=0)*-2+1;
  float positionY = fieldSide*1000.f*amplitud;

    //Solo restar BALL a las otras coordenadas, no rotarlas!


  if(willBeClear(origen,desiredPosL) && std::abs(positionX-desiredPosL.x())<=300){
    return desiredPosL;
  }else{
    int count = 0;
    while(count<800){
      int rango = (int)(amplitud*theBehaviorParameters.wideHole);
      float y = ((std::rand()%rango)+100.f) * fieldSide;
      float x = ((std::rand()%400)-200.f);
      Vector2f destino_prueba = Vector2f(positionX+x,y);
      if(willBeClear(origen,destino_prueba)){
        return destino_prueba;
      }
      count++;
    }
  }
  Vector2f desiredPos= Vector2f(positionX, positionY);
  return desiredPos;
}

bool LibCodeReleaseProvider::isThatClear(Vector2f coordenada, float distanceToBeClear){
/*
  Verifica si hay un obstaculo entre la coordenada entrante GLOBAL y la posicion actual del robot
*/
  bool isClear = true;
  Vector2f coordenadaRel= (theRobotPose.inversePose * coordenada);

  if(!theObstacleModel.obstacles.empty()){
    float pendiente= coordenadaRel.y()/coordenadaRel.x();

    for(const auto& obstacle : theObstacleModel.obstacles){
      if(obstacle.type!= Obstacle::Type::teammate){
        //See if the obstacle is first than the target
        if ((coordenadaRel.norm()-200.f)>obstacle.center.norm() && std::abs(obstacle.center.angle()-coordenadaRel.angle())<90_deg ){
          //See if the obstacle is in the way to the target
          float distanciaACamino = getDistanceToLine(obstacle.left,obstacle.right,pendiente);
          if(distanciaACamino< distanceToBeClear){
              return isClear=false;
          }
        }
      }
    }
  }
  //std::cout << "Global  "<<coordenada <<"\n" << "Relativa  "<<coordenadaRel <<"\n"<< isClear<<"\n";

  return isClear;
}


bool LibCodeReleaseProvider::willBeClear(Vector2f origen, Vector2f destino){
  /*
  Metodo para evaluar si el lugar a donde se desea llegar estara vacio!
    origen: lugar de donde se dara el pase
    destino: lugar donde se pretende recibir el pase
  */
  bool isClear = true;
  Vector2f destinoRel= (theRobotPose.inversePose * destino);
  Vector2f origenRel = (theRobotPose.inversePose * origen);
  destinoRel = destinoRel-origenRel;

  // theObstacleModel mio
  if(!theObstacleModel.obstacles.empty()){

    float pendiente= (destinoRel.y())/(destinoRel.x());

    for(const auto& obstacle : theObstacleModel.obstacles){
      //See if the obstacle is first than the target
      Vector2f obstacleRel = obstacle.center-origenRel;
      Vector2f obstacleRelLeft = obstacle.left-origenRel;
      Vector2f obstacleRelRight = obstacle.right-origenRel;

      if ((destinoRel.norm()-200.f)>obstacleRel.norm() && std::abs(obstacleRel.angle()-destinoRel.angle())<90_deg ){
        //See if the obstacle is in the way to the target
        float distanciaACamino = getDistanceToLine(obstacleRelLeft,obstacleRelRight,pendiente);

        if(distanciaACamino< theBehaviorParameters.distanceToBeClear){
            return isClear=false;
        }
      }
    }
  }

  // theObstacleModel del que va a dar el pase
  destinoRel= (closerToTheBall.theRobotPose.inversePose * destino);
  origenRel = (closerToTheBall.theRobotPose.inversePose * origen);
  destinoRel = destinoRel-origenRel;

  if(!closerToTheBall.theObstacleModel.obstacles.empty()){
    float pendiente= (destinoRel.y())/(destinoRel.x());

    for(const auto& obstacle : closerToTheBall.theObstacleModel.obstacles){
      //See if the obstacle is first than the target
      Vector2f obstacleRel = obstacle.center-origenRel;
      Vector2f obstacleRelLeft = obstacle.left-origenRel;
      Vector2f obstacleRelRight = obstacle.right-origenRel;

      if ((destinoRel.norm()-200.f)>obstacleRel.norm() && std::abs(obstacleRel.angle()-destinoRel.angle())<90_deg){
        //See if the obstacle is in the way to the target
        float distanciaACamino = getDistanceToLine(obstacleRelLeft,obstacleRelRight,pendiente);
        if(distanciaACamino< theBehaviorParameters.distanceToBeClear){
            return isClear=false;
        }
      }
    }
  }

 // std::cout << isClear << " Destino: "<< destino.x() << " " << destino.y()<< "\n";
  return isClear;
}
int LibCodeReleaseProvider::howManyDefenders(){
    int number = 0;

    for (int i = 0; i < int(theTeamData.teammates.size()); i++)
      if(!theTeamData.teammates[i].isPenalized && (theTeamData.teammates[i].theBallModel.tipo == Role::defender || theTeamData.teammates[i].theBallModel.tipo == Role::supporter))
        number++;

    if(theBehaviorStatus.role == Role::defender)
      number++;

    return number;
}
int LibCodeReleaseProvider::howManyStrikers(){
    int number = 0;

    for (int i = 0; i < int(theTeamData.teammates.size()); i++)
      if(!theTeamData.teammates[i].isPenalized && theTeamData.teammates[i].theBallModel.tipo == Role::striker)
        number++;

    if(theBehaviorStatus.role == Role::striker)
      number++;

    return number;
}
int LibCodeReleaseProvider::howManyKeepers(){
    int number = 0;

    for (int i = 0; i < int(theTeamData.teammates.size()); i++)
      if(!theTeamData.teammates[i].isPenalized && theTeamData.teammates[i].theBallModel.tipo == Role::keeper)
        number++;

    if(theBehaviorStatus.role == Role::keeper)
      number++;

    return number;
}
