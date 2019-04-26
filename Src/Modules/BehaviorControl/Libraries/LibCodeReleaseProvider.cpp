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
	libCodeRelease.keeperInsideGoal = keeperInsidePenaltyArea();

	setPrimaryPosition(libCodeRelease);

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

	if (theRobotInfo.number == 1) {
		if (theRobotPose.inversePose.translation.x() > theFieldDimensions.xPosOwnPenaltyArea
			|| theRobotPose.inversePose.translation.y() > theFieldDimensions.yPosLeftPenaltyArea  // if its outside return false
			|| theRobotPose.inversePose.translation.y() < theFieldDimensions.yPosRightPenaltyArea)
		{
			*keeperInside = false;
		}
		else
		{
			*keeperInside = true;
		}
	}

	return *keeperInside;

	//return false;
}

void LibCodeReleaseProvider::setPrimaryPosition(LibCodeRelease& libCodeRelease)
{

	float lineMargin = theFieldDimensions.fieldLinesWidth * 3;

	Vector2f ownRhtPenalTopCorner(theFieldDimensions.xPosOwnPenaltyArea - lineMargin, theFieldDimensions.yPosRightPenaltyArea - lineMargin);
	Vector2f ownRhtPenalBottomCorner(theFieldDimensions.xPosOwnGroundline - lineMargin, theFieldDimensions.yPosRightPenaltyArea - lineMargin);
	Vector2f ownLftPenalTopCorner(theFieldDimensions.xPosOwnPenaltyArea - lineMargin, theFieldDimensions.yPosLeftPenaltyArea - lineMargin);
	Vector2f ownLftPenalBottomCorner(theFieldDimensions.xPosOwnGroundline - lineMargin, theFieldDimensions.yPosLeftPenaltyArea - lineMargin);

	Vector2f oppRhtPenalBottomCorner(theFieldDimensions.xPosOpponentGroundline - lineMargin, theFieldDimensions.yPosRightPenaltyArea - lineMargin);
	Vector2f oppLftPenalBottomCorner(theFieldDimensions.xPosOpponentGroundline - lineMargin, theFieldDimensions.yPosLeftPenaltyArea - lineMargin);

	Vector2f ownRhtMiddleFieldLinePnt(theFieldDimensions.xPosOwnFieldBorder - lineMargin / 2, theFieldDimensions.yPosRightFieldBorder - lineMargin);
	Vector2f ownLftMiddleFieldLinePnt(theFieldDimensions.xPosOwnFieldBorder - lineMargin / 2, theFieldDimensions.yPosLeftFieldBorder - lineMargin);

	Vector2f middleBottomPoint(theFieldDimensions.xPosOwnGroundline / 2, theFieldDimensions.yPosCenterGoal);
	Vector2f middleCenterPoint(theFieldDimensions.xPosHalfWayLine, theFieldDimensions.yPosCenterGoal);

	Vector2f intersectionPoint(0.f, 0.f);

	Geometry::Line penaltyYRightLine(ownRhtPenalBottomCorner, oppRhtPenalBottomCorner);
	Geometry::Line penaltyYLeftLine(ownLftPenalBottomCorner, oppLftPenalBottomCorner);
	Geometry::Line middleXFieldLine(middleBottomPoint, middleCenterPoint);
	Geometry::Line penaltyHipoLine(ownRhtPenalTopCorner, ownLftPenalBottomCorner);
	Geometry::Line middleYOwnFieldLine(ownRhtMiddleFieldLinePnt, ownLftMiddleFieldLinePnt);

	float xPos = 0.f;
	float yPos = 0.f;

	int defenderCount = 0;
	
	switch (theBehaviorStatus.role)
	{
		
	case Role::Defender:

		if (defenderCount == 0)
		{
			Geometry::getIntersectionOfLines(penaltyYLeftLine, middleYOwnFieldLine, intersectionPoint);
			xPos = intersectionPoint.x();
			yPos = intersectionPoint.y();
			defenderCount += 1;
		}
		else
		{
			Geometry::getIntersectionOfLines(penaltyYRightLine, middleYOwnFieldLine, intersectionPoint);
			xPos = intersectionPoint.x();
			yPos = intersectionPoint.y();
		}

		break;

	case Role::Keeper:

		Geometry::getIntersectionOfLines(penaltyHipoLine, middleXFieldLine, intersectionPoint);
		xPos = intersectionPoint.x();
		yPos = intersectionPoint.y();

		break;

	case Role::Striker:

		Geometry::getIntersectionOfLines(middleYOwnFieldLine, middleXFieldLine, intersectionPoint);
		xPos = intersectionPoint.x() + 500;
		yPos = intersectionPoint.y();

			break;

		case Role::Supporter:

			Geometry::getIntersectionOfLines(middleYOwnFieldLine, middleXFieldLine, intersectionPoint);
			xPos = intersectionPoint.x() - 500;
			yPos = intersectionPoint.y();

			break;
	}

	Vector2f startingPos(xPos, yPos);

	libCodeRelease.startingPos = startingPos;
	
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
