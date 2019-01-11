/**
 * @file LibCodeReleaseProvider.h
 */

#include "Tools/Module/Module.h"
#include "Representations/BehaviorControl/Libraries/LibCodeRelease.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/RobotInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Communication/TeamData.h"
#include "Representations/BehaviorControl/BehaviorStatus.h"

MODULE(LibCodeReleaseProvider,
{,
  REQUIRES(BallModel),
  REQUIRES(TeamBallModel),
  REQUIRES(FieldDimensions),
  REQUIRES(FrameInfo),
  REQUIRES(RobotPose),
  REQUIRES(RobotInfo),
  REQUIRES(TeamData),
  USES(BehaviorStatus),
  PROVIDES(LibCodeRelease),
});

class LibCodeReleaseProvider : public LibCodeReleaseProviderBase
{
  void update(LibCodeRelease& libCodeRelease);

  private:

    bool isCloserToTheBall();
    double distanceToBall;
    bool ballInOppField();
    bool ballInsideOwnGoal();
	bool keeperInsidePenaltyArea();


};
