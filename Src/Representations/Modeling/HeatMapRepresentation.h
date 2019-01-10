/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Function.h"
#include "Tools/Math/Eigen.h"
#include "Tools/Streams/AutoStreamable.h"
#include "Tools/Streams/Enum.h"

STREAMABLE(HeatMapRepresentation,
{
  void verify() const;
  void draw() const,
  (int) numberOfZones,
  (int) stepSizeX,
  (int) stepSizeY,
  (int[100]) initialXs,
  (int[100]) finalXs,
  (int[100]) initialYs,
  (int[100]) finalYs,
  //(int[72]) times,
  (float[100]) percentage,
});
