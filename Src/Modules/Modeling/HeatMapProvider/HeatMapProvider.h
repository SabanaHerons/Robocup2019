/**
 * @file LibCodeReleaseProvider.h
 */

#include "Tools/Module/Module.h"	
#include "Representations/Modeling/TeamBallModel.h"
#include "Representations/Modeling/HeatMapRepresentation.h"
#include "Representations/Configuration/FieldDimensions.h"
#include "Tools/Modeling/Zone.h"

MODULE(HeatMapProvider,
{,
  REQUIRES(FieldDimensions),
  REQUIRES(TeamBallModel),
  PROVIDES(HeatMapRepresentation),
});

class HeatMapProvider : public HeatMapProviderBase
{
public:
	HeatMapProvider();
	
private:
  int pruebas;
  int prueba2;
  float sumation;
  std::vector<Zone> zonas;
  void update(HeatMapRepresentation& heatMapRepresentation);
};
