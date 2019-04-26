/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Math/Pose2f.h"
#include "Tools/Function.h"

STREAMABLE(GeneticLocator,
{
  //FUNCTION(bool(float value, float min, float max)) activation,
  FUNCTION(Pose2f(const bool activates, const int ctime)) activation,

  (float) optimalX,
  (float) optimalY,
  (float) lastime,
  (int) Hmtimes,
});
