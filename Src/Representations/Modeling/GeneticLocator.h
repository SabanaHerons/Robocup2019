/**
 * @file LibCodeRelease.h
 */

#pragma once
#include "Tools/Function.h"

STREAMABLE(GeneticLocator,
{
  FUNCTION(bool(float value, float min, float max)) hola,

  (int) optimalX,
  (int) optimalY,
});
