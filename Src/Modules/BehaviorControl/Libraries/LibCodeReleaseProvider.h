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
#include "Representations/Configuration/BehaviorParameters.h"

MODULE(LibCodeReleaseProvider,
{,
  REQUIRES(BallModel),
  REQUIRES(TeamBallModel),
  REQUIRES(FieldDimensions),
  REQUIRES(FrameInfo),
  REQUIRES(RobotPose),
  REQUIRES(RobotInfo),
  REQUIRES(TeamData),
  REQUIRES(ObstacleModel),
  REQUIRES(BehaviorParameters),
  USES(BehaviorStatus),
  PROVIDES(LibCodeRelease),
});

class LibCodeReleaseProvider : public LibCodeReleaseProviderBase
{
  void update(LibCodeRelease& libCodeRelease);

  private:

    bool isCloserToTheBall();
    Teammate getCloserToTheBall();
    double distanceToBall;

    //Néstor
    float lineDistancer();
    bool whoDefenderAtLeft();
    bool whoDefenderAtRight();
    int howManyDefences();
    float whereItIntersects();
    float whereItIntersectsRelative();
    bool timeWithAStaticBall();

    //Omar
    Teammate closerToTheBall;
    Vector2f desiredPosL =Vector2f(0,0);

    Vector2f nearestEnemyL = Vector2f(1000.f,1000.f);
    Vector2f whereToShootL = Vector2f(0,0);
    float getDistanceToLine(Vector2f left,Vector2f right, float pendiente);
    bool isAboutToShoot();
    Vector2f getPlaceToShoot();

    //Coronate
    bool prevRivalHasTheBall;

    bool hasRivalTheBall();
    bool rivalNearestToBall();
    int howManyDefenders();
    int howManyStrikers();
    int howManyKeepers();


    //Valentina
    bool toRight();
    bool toLeft();
    bool toDown();

    //Defecto
    unsigned timeStampCloser=0;
    bool prevCloserToBall=false;

    bool ballInOppField();
    bool ballInsideOwnGoal();
    bool keeperInsidePenaltyArea();
    bool mustShootToGoal();
    Vector2f getDesiredPosition();
    Vector2f getBall();
    bool isThatClear(Vector2f coordenada,float distanceToBeClear);
    bool willBeClear(Vector2f origen, Vector2f destino);



};
