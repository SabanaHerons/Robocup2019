/**
 * @file Modules/BehaviorControl/PathPlannerProvider/PathPlannerProvider.cpp
 *
 * This file implements a module that provides a representation that allows to
 * determine a motion request that brings the robot closer to a given target
 * based on reactive movements.
 *
 * @author Omar Sanchez
 */

#include "PathPlannerProvider.h"
#include "Platform/SystemCall.h"
#include "Tools/Debugging/DebugDrawings.h"
#include "Tools/Debugging/DebugDrawings3D.h"
#include "Tools/Debugging/Annotation.h"
#include <algorithm>
#include <iostream>
#include <cmath>

MAKE_MODULE(PathPlannerProvider, behaviorControl);

//static const float epsilon = 0.1f; /**< Small offset in mm. */

PathPlannerProvider::PathPlannerProvider()
{

}

int direccionElegida =-1; //0 es izquierda || 1 es derecha
bool rotando=false;
Vector2f lastPositionToGo =Vector2f(1000000.f,500000.f);

void PathPlannerProvider::update(PathPlanner& pathPlanner)
{

  lastPositionToGo =Vector2f(1000000.f,500000.f); //Creo que LASTPOSITIONTOGO no esta haciendo nada! ...
  pathPlanner.plan = [this](const Pose2f& target, const Pose2f& speed, bool excludePenaltyArea, bool avoidBall) -> MotionRequest
  {

    /*
    Notas para el BOLUDO que programa:
      -ObstacleModel Provee obstaculos con posicion RELATIVA al robot
      -ObstacleModel Provee los obstaculos organizados por cercania??
      -Los obstaculos se eliminan despues de cierto periodo de no haberlos visto (No inmediatamente)
      -MotionRequest en modo target requiere una posicion RELATIVA al robot
      -VecTargetRobot es un vector RELATIVO del objetivo
      -En resumen: Todo es relativo, solo el TARGET es GLOBAL
    */
    MotionRequest motionRequest;
    //vector relativo del target
    Pose2f vecTargetRobot= target- theRobotPose;


    float angleRelativo=target.rotation;

    //std::cout << "LastPos: " << lastPositionToGo << "\n";
     //verifica si se fijo un objetivo previamente
    if(lastPositionToGo.x()<9000){
      vecTargetRobot = Pose2f(0.f,lastPositionToGo.x(),lastPositionToGo.y());
    }

    Obstacle nearestObstacle= Obstacle();
    nearestObstacle.center=vecTargetRobot.translation;
    bool obstacleFound= false;

  //se calcula la pendiente de vecTargetRobot (se toma como una recta)
    float pendiente= vecTargetRobot.translation.y()/vecTargetRobot.translation.x();

  //Fase filtrado de obstaculos!
    if(!theObstacleModel.obstacles.empty()){

      for(const auto& obstacle : theObstacleModel.obstacles){
      //See if the obstacle is first than the target
        if (vecTargetRobot.translation.norm()>obstacle.center.norm()){
        //See if the obstacle is in the way to the target
          float distanciaACamino = (float) (std::abs(pendiente*obstacle.center.x()-obstacle.center.y())/sqrt(pendiente*pendiente+1));
          if(distanciaACamino<400.f){
          //See if the "new obstacle" has already been dodged
          //verifica que aun le falte mas del 6% del vector targetRobot
            float porcentajeProyeccion = (vecTargetRobot.translation.x()*obstacle.center.x()+vecTargetRobot.translation.y()*obstacle.center.y())/(vecTargetRobot.translation.norm()*vecTargetRobot.translation.norm());
            if(porcentajeProyeccion>0.06f){
            //See if the new obstacle is first than the other
              if(nearestObstacle.center.norm()>obstacle.center.norm()){
                nearestObstacle=obstacle;
                obstacleFound= true;
              }
            }
          }
        }
      }
    }


    Vector2f relativeBall = theRobotPose.inversePose * theTeamBallModel.position;
    //see if we are in a state of game which we can not get closer to the ball
   if (theGameInfo.setPlay != SET_PLAY_NONE && theGameInfo.kickingTeam != theOwnTeamInfo.teamNumber){
      //See if the ball is first than the target
      if (vecTargetRobot.translation.norm()>(relativeBall.norm()-800.f)){
        //See if the obstacle is in the way to the target
        float distanciaACamino = (float) (std::abs(pendiente*relativeBall.x()-relativeBall.y())/sqrt(pendiente*pendiente+1));
        if(distanciaACamino<1000.f){
          //See if the "new obstacle" has already been dodged
          //verifica que aun le falte mas del 6% del vector targetRobot
          float porcentajeProyeccion = (vecTargetRobot.translation.x()*relativeBall.x()+vecTargetRobot.translation.y()*relativeBall.y())/(vecTargetRobot.translation.norm()*vecTargetRobot.translation.norm());
          if(porcentajeProyeccion>0.06f){
          //See if the new obstacle is first than the other
            if(nearestObstacle.center.norm()>(relativeBall.norm()-800.f)){
              nearestObstacle=Obstacle(Matrix2f(),relativeBall,Vector2f(relativeBall.x(),relativeBall.y()+800.f) ,Vector2f(relativeBall.x(),relativeBall.y()-800.f), Vector2f::Zero(),0, Obstacle::Type::unknown);
              obstacleFound= true;
            }
          }
        }
      }
    }else{
      //Verificar si la pelota no es el destino final y queremos esquivar la pelota!
      if((target.translation-theTeamBallModel.position).norm()>10.f && avoidBall){
        //See if the ball is first than the target
        if (vecTargetRobot.translation.norm()>relativeBall.norm()){
          //See if the obstacle is in the way to the target
          float distanciaACamino = (float) (std::abs(pendiente*relativeBall.x()-relativeBall.y())/sqrt(pendiente*pendiente+1));
          if(distanciaACamino<400.f){
          //See if the "new obstacle" has already been dodged
          //verifica que aun le falte mas del 6% del vector targetRobot
            float porcentajeProyeccion = (vecTargetRobot.translation.x()*relativeBall.x()+vecTargetRobot.translation.y()*relativeBall.y())/(vecTargetRobot.translation.norm()*vecTargetRobot.translation.norm());
            if(porcentajeProyeccion>0.06f){
            //See if the new obstacle is first than the other
              if(nearestObstacle.center.norm()>relativeBall.norm()){

                nearestObstacle=Obstacle(Matrix2f(),relativeBall,Vector2f(relativeBall.x(),relativeBall.y()+50.f) ,Vector2f(relativeBall.x(),relativeBall.y()-50.f), Vector2f::Zero(),0, Obstacle::Type::unknown);
                obstacleFound= true;
              }
            }
          }
        }
      }
    }

      if (obstacleFound){ //si hay obstaculo
        float distancia=300.f;
        float cercania= 650.f;

        //DETERMINA LAS ESQUINAS DE LA "BARRERA"
        Vector2f farthestTargets[2]; // 0 es izquierda || 1 es derecha
        farthestTargets[0]= nearestObstacle.left;
        farthestTargets[1]= nearestObstacle.right;

        //el angulo entre el eje y del robot y el vector del balon
        float radEje= (float) acos((vecTargetRobot.translation.y())/vecTargetRobot.translation.norm());

        for(const auto& obstacle : theObstacleModel.obstacles){
          //No comparar con el obstaculo actual
          if (obstacle.center!=nearestObstacle.center){
            //verificar si el obstaculo esta aun mas a la izquierda que el actual obstaculo mas a la izquierda
            //Si no se verifica todo lo contrario (si el obstaculo esta mas a la derecha que el mas a la derecha actual)
            float left = (float)(obstacle.left.x()*cos(radEje)-obstacle.left.y()*sin(radEje));
            float right=(float)(obstacle.right.x()*cos(radEje)-obstacle.right.y()*sin(radEje));
            float farthestLeft=(float)(farthestTargets[0].x()*cos(radEje)-farthestTargets[0].y()*sin(radEje));
            float farthestRight=(float)(farthestTargets[1].x()*cos(radEje)-farthestTargets[1].y()*sin(radEje));

            if (left<farthestLeft){
              //el obstaculo actual estata mas a la izquierda que el mas a la izquierda anterior
              //Verificar si el obstaculo esta lo suficientemente cerca del anterior obstaculo para ser tenido en cuenta
              Vector2f diffObstacles = obstacle.right-farthestTargets[0];
              if (diffObstacles.norm()<cercania){
                farthestTargets[0]=obstacle.left;
              }
            }else if(right>farthestRight){
              //el obstaculo actual estata mas a la derecha que el mas a la derecha anterior
              //Verificar si el obstaculo esta lo suficientemente cerca del anterior obstaculo para ser tenido en cuenta
              Vector2f diffObstacles = obstacle.left-farthestTargets[1];
              if (diffObstacles.norm()<cercania){
                farthestTargets[1]=obstacle.right;
              }
            }
          }
        }

        //DETERMINA LA DIRECCION A ELEGIR
        //se verifica la distancia de tomar cada camino y se elije uno de los dos!
        //si se esta muy cerca del menor target se fija ese destino

        bool hallarNuevaDir= true;
        if (direccionElegida!=-1){
          if (farthestTargets[direccionElegida].norm()<500.f){
            hallarNuevaDir=false;
          }
        }

        if (hallarNuevaDir){
          float distanciaMenor= 10000000.f;
          int count =0;
          for (Vector2f newTar:farthestTargets){
            Vector2f diffNewTar_Tar = vecTargetRobot.translation - newTar;
            float distanciaTotal =diffNewTar_Tar.norm()+newTar.norm();
            if (distanciaTotal<distanciaMenor){
              distanciaMenor = distanciaTotal;
              direccionElegida = count;
            }
            count++;
          }
        }

         //si el robot esta cerca al obstaculo fijarlo! (por el hecho de que deja de ver el obstaculo)
       // if (nearestObstacle.center.norm()<450.f){ //PEEEEEEEEEEEEEELLLLLIIIIGGGGRRRRROOOOOOOOOOOOO
        if(lastPositionToGo.x()>9000)
          lastPositionToGo=farthestTargets[direccionElegida];
          //}

        //DAR COORDENADA PARA ESQUIVAR EL OBSTACULO
        distancia*=(-1+2*direccionElegida);// se invierte la distancia en caso de que se elija la izquierda

        //se rota lo que se le sumara a la posicion del obstaculo que el robot debe seguir
        float distanciaX= (float)(distancia*cos(-radEje));
        float distanciaY=(float)(distancia*sin(-radEje));

        farthestTargets[direccionElegida] = Vector2f(farthestTargets[direccionElegida].x()+distanciaX, farthestTargets[direccionElegida].y()+distanciaY);

        //restriccion de movimiento en el eje hacia el target cuando hay un obstaculo esta muy cerca
        if(nearestObstacle.center.norm()<300.f){
          float constante = (vecTargetRobot.translation.x()*farthestTargets[direccionElegida].x()+vecTargetRobot.translation.y()*farthestTargets[direccionElegida].y())/(vecTargetRobot.translation.norm()*vecTargetRobot.translation.norm());
          Vector2f proyeccionNewTarget= constante*vecTargetRobot.translation;
          farthestTargets[direccionElegida] = farthestTargets[direccionElegida]-proyeccionNewTarget;
        }

        //std::cout << "Final X: " << farthestTargets[direccionElegida].x();
        //std::cout << " Final Y: "<< farthestTargets[direccionElegida].y() <<"\n";
        if(angleRelativo==0)
          angleRelativo=farthestTargets[direccionElegida].angle();
        //verificar si esta alineado con el objetivo final y lo alinea
        if((std::abs(angleRelativo)>60_deg || rotando) && target.rotation==0){
          rotando = true;
          motionRequest.walkRequest.target = Pose2f(angleRelativo, 0,0);
          if(std::abs(angleRelativo)<5_deg){
            rotando=false;
          }
        }else{
          motionRequest.walkRequest.target = Pose2f(angleRelativo, farthestTargets[direccionElegida].x(), farthestTargets[direccionElegida].y());
        }
      }else{
      //Si no hay obstaculos en el camino
        if (lastPositionToGo.norm()<200.f){//si ya se llego al objetivo previamente fijado
         lastPositionToGo=Vector2f(1000000.f,500000.f);
       }
       direccionElegida = -1;

       if(angleRelativo==0)
        angleRelativo=(theRobotPose.inversePose * target.translation).angle();
        //verificar si esta alineado con el objetivo final y lo alinea
        if((std::abs(angleRelativo)>60_deg || rotando) && target.rotation==0){
          rotando = true;
          motionRequest.walkRequest.target = Pose2f(angleRelativo, 0,0);
          if(std::abs(angleRelativo)<5_deg){
            rotando=false;
          }
        }else{
          motionRequest.walkRequest.target =  Pose2f(angleRelativo,vecTargetRobot.translation.x(),vecTargetRobot.translation.y());
        }
    }


  //std::cout <<motionRequest.walkRequest.target.rotation <<""<< motionRequest.walkRequest.target.translation.x() <<" "<<  motionRequest.walkRequest.target.translation.y()<<"\n";
  //std::cout << "X: " << motionRequest.walkRequest.target.translation.x();
  //std::cout << " Y: "<< motionRequest.walkRequest.target.translation.y();
  //std::cout << "\n";
    motionRequest.motion = MotionRequest::walk;
    motionRequest.walkRequest.mode = WalkRequest::targetMode;
    motionRequest.walkRequest.speed = speed;
    motionRequest.walkRequest.walkKickRequest = WalkRequest::WalkKickRequest();
    return motionRequest;

  };
}
