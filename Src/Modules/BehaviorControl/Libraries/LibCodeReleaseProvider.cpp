/**
 * @file LibCodeRelease.cpp
 */

#include "LibCodeReleaseProvider.h"
#include "Tools/Math/Geometry.h"
#include <iostream>

MAKE_MODULE(LibCodeReleaseProvider, behaviorControl);

void LibCodeReleaseProvider::update(LibCodeRelease& libCodeRelease)
{
  libCodeRelease.timeSinceBallWasSeen = theFrameInfo.getTimeSince(theTeamBallModel.timeWhenLastSeen);

  libCodeRelease.angleToOppGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOpponentGroundline, 0.f)).angle();
  libCodeRelease.angleToOwnGoal = (theRobotPose.inversePose * Vector2f(theFieldDimensions.xPosOwnGroundline, 0.f)).angle();


    libCodeRelease.closerToTheBall = isCloserToTheBall();
    libCodeRelease.ballInOppField = ballInOppField();
    libCodeRelease.ballInsideOwnGoal = ballInsideOwnGoal();

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

}

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

  if (theBallModel.estimate.position.x() > theFieldDimensions.xPosOwnPenaltyArea && theBallModel.estimate.position.y() > theFieldDimensions.yPosLeftPenaltyArea){ 
    *ballInsideOwnGoal = true;
  }else{
    *ballInsideOwnGoal = false;
  }


return *ballInsideOwnGoal;
}



bool LibCodeReleaseProvider::isCloserToTheBall()
{

        double teammateDistanceToBall = 0.0;

    distanceToBall = theBallModel.estimate.position.norm();

    for(auto const& teammate : theTeamData.teammates)
    {

                teammateDistanceToBall = teammate.theBallModel.estimate.position.norm();

                if(distanceToBall > teammateDistanceToBall)
			{
                    return false;
			}
        
    }
return true;
}
