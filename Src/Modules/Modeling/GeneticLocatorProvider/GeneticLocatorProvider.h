#include "Tools/Module/Module.h"
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Modeling/GeneticLocator.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Communication/TeamData.h"
#include "Representations/Infrastructure/RobotInfo.h"
#include "Representations/Modeling/BallModel.h"
#include "Representations/Infrastructure/FrameInfo.h"

MODULE(GeneticLocatorProvider,
{,
  REQUIRES(TeamData),
  REQUIRES(TeamBallModel),
  REQUIRES(RobotPose),
  REQUIRES(RobotInfo),
  REQUIRES(BallModel),
  REQUIRES(FrameInfo),
  REQUIRES(GeneticLocator),
  PROVIDES(GeneticLocator),
});

class GeneticLocatorProvider : public GeneticLocatorProviderBase
{
public:
  GeneticLocator* geneticLocatorr;
	GeneticLocatorProvider();
	float result(int a, int b);
  std::vector<std::vector<float>> ordenador(int a, int b);

private:
	void update(GeneticLocator& geneticLocator);
};
