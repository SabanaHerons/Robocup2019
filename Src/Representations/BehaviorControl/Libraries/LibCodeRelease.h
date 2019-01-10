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
FUNCTION(void(bool isMissing)) setMissingTeammate,

(float)(0.f) angleToOppGoal,
(float)(0.f) angleToOwnGoal,
(int)(0) timeSinceBallWasSeen,
(bool)(false) closerToTheBall,
});
