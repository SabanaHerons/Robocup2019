/**
 * @file Representations/Configuration/BehaviorParameters/BehaviorParameters.h
 *
 * The file declares a struct that contains frequently used parameters of the behavior which can be modified.
 *
 * @author Andreas Stolpmann
 */

#pragma once

#include "Representations/BehaviorControl/Role.h"
#include "Tools/Math/Angle.h"

STREAMABLE(BehaviorParameters,
{,
  (bool)(false) enableWalkStraight,
  (int)(7000) ballNotSeenTimeOut,
  (int)(100) timeToStart,
  (float)(600) distanceToBeClear,
  (float)(3000) maxShootDistance,
  (float)(1000) minShootDistance,
  (float)(3500) farPosStriker,
  (float)(2500) middlePosStriker,
  (float)(1500) nearestPosStriker,
  (float)(20)thresholdOwnBall,
  (int)(1000)wideHole,

  //Omar
  (float)(550) distanceToGoShoot,
  (float)(250) distanceToBeClearForShoot,

  (float)(550) ballEnemy,
  (bool)(false) offensive,
});
