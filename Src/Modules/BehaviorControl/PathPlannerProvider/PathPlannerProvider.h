/**
 * @file Modules/BehaviorControl/PathPlannerProvider/PathPlannerProvider.h
 *
 * This file declares a module that provides a representation that allows to
 * determine a motion request that brings the robot closer to a given target
 * based on path planning. The planing problem is modeled using a visibility
 * graph.
 *
 * @author Thomas Röfer
 */

#include "Representations/BehaviorControl/PathPlanner.h"
#include "Representations/Configuration/BehaviorParameters.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Representations/Infrastructure/FrameInfo.h"
#include "Representations/Infrastructure/GameInfo.h"
#include "Representations/Infrastructure/TeamInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Modeling/ObstacleModel.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Modeling/TeamPlayersModel.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Tools/Module/Module.h"
#include <limits>

MODULE(PathPlannerProvider,
{,
  REQUIRES(BallModel),
  REQUIRES(TeamBallModel),
  REQUIRES(BehaviorParameters),
  REQUIRES(FieldDimensions),
  REQUIRES(FrameInfo),
  REQUIRES(GameInfo),
  REQUIRES(ObstacleModel),
  REQUIRES(RobotPose),
  REQUIRES(OwnTeamInfo),
  REQUIRES(TeamPlayersModel),
  PROVIDES(PathPlanner),
});

class PathPlannerProvider : public PathPlannerProviderBase
{
  /**
   * Provide a representation that is able to plan a path using this module.
   * @param pathPlanner The representation that is provided.
   */
  void update(PathPlanner& pathPlanner) override;

public:
  /** The default constructor constructs the borders from the field dimensions. */
  PathPlannerProvider();
};
