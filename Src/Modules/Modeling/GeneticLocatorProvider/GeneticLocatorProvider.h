/**
 * @file LibCodeReleaseProvider.h
 */

#include "Tools/Module/Module.h"	
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Modeling/GeneticLocator.h"
#include "Representations/Modeling/RobotPose.h"
#include "Representations/Communication/TeamData.h"
#include "Individuo.h"

MODULE(GeneticLocatorProvider,
{,
  REQUIRES(TeamData),
  REQUIRES(TeamBallModel),
  REQUIRES(RobotPose),
  PROVIDES(GeneticLocator),
});

class GeneticLocatorProvider : public GeneticLocatorProviderBase
{
public:
	GeneticLocatorProvider();
	int random_num(int start, int finish);
	string cross(string a, string b);
	
private:	
	void update(GeneticLocator& geneticLocator);
};
