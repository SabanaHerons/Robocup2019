/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Function.h"
#include "Tools/Math/Eigen.h"
#include "Tools/Streams/AutoStreamable.h"
#include "Tools/Streams/Enum.h"

STREAMABLE(LibCodeRelease,
{
  FUNCTION(bool(float value, float min, float max)) between;
  FUNCTION(float(float value, float min, float max)) clamp;
  FUNCTION(Vector2f(float angle,Vector2f coordenada)) rotateCoordenates;
  FUNCTION(bool(Vector2f coordenada,float distanceToBeClear)) isThatClear;
  FUNCTION(Vector2f()) bestTeammateForPass;
  FUNCTION(int()) getNumberWithinRole;
  FUNCTION(void(bool isMissing)) setMissingTeammate, // Coronate

  (float)(0.f) angleToOppGoal,
  (float)(0.f) angleToOwnGoal,

  //Néstor
  (float)(0.f) distanceFromLine,
  (bool)(false) defenderLefter,
  (bool)(false) defenderRighter,
  (int)(0) numberOfDefences,
  (float)(0.f) intersectoEnYGlobal,
  (float)(0.f) intersectoEnYGlobalRelative,
  (bool)(false) timeWithBallNotMoving,

  //Omar
  (Vector2f)(Vector2f(1000.f,1000.f)) nearestEnemy, //Global Coordenates of the neares Enemy to the ball
  (bool) (false) areShooting,
  (Vector2f)(Vector2f(4500,0.f)) whereToShoot,

  //Coronate
  (bool)(false) theRivalHasTheBall,
  (bool)(false) theRivalIsCloserToTheBall,

  //Valentina
  (bool)(false) rightDive,
  (bool)(false) leftDive,
  (bool)(false) needGoDown,

  (float) angleToGoal,
  (int)(0) timeSinceBallWasSeen,
  (bool)(false) closerToTheBall,
  (bool)(false) ballInOppField,
  (bool)(false) ballInsideOwnGoal,
  (bool)(false) keeperInsideGoal,
  (bool)(false) shootToGoal,
  (Vector2f)(Vector2f(0,0)) desiredPos,
  (Vector2f)(Vector2f(0,0)) ball,
    (int)(0) numberOfDefenders,
    (int)(0) numberOfStrikers,
    (int)(0) numberOfKeepers,

});
