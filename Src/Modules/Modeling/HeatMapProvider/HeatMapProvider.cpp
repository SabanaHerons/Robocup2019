#include "HeatMapProvider.h"

MAKE_MODULE(HeatMapProvider, modeling);

HeatMapProvider::HeatMapProvider()
{
	int stepSizeX = (theFieldDimensions.xPosOpponentGroundline - theFieldDimensions.xPosOwnGroundline)/10;
	int stepSizeY = (theFieldDimensions.yPosLeftSideline - theFieldDimensions.yPosRightSideline)/10;
	for(int j = int(theFieldDimensions.yPosRightSideline); j < int(theFieldDimensions.yPosLeftSideline)-10; j += stepSizeY)
	{
		for(int i = int(theFieldDimensions.xPosOwnGroundline); i < int(theFieldDimensions.xPosOpponentGroundline); i += stepSizeX)
		{
			zonas.push_back(HeatMapZone(i,i+stepSizeX,j,j+stepSizeY));
		}
	}
	pruebas = stepSizeX;
	prueba2 = stepSizeY;
}
 
void HeatMapProvider::update(HeatMapRepresentation& heatMapRepresentation)
{
	heatMapRepresentation.numberOfZones = zonas.size();
	heatMapRepresentation.stepSizeX = pruebas;
	heatMapRepresentation.stepSizeY = prueba2;
	sumation = 0;
	for(int i = 0; i < int(zonas.size());i++)
	{
		if(theTeamBallModel.isValid)
		{
			zonas[i].updateTime(theTeamBallModel.position.x(),theTeamBallModel.position.y());
		}
		sumation += zonas[i].timeInside;
	}
	
	for(int i = 0; i < int(zonas.size());i++)
	{
		heatMapRepresentation.initialXs[i] = zonas[i].initialX;
		heatMapRepresentation.finalXs[i] = zonas[i].finalX;
		heatMapRepresentation.initialYs[i] = zonas[i].initialY;
		heatMapRepresentation.finalYs[i] = zonas[i].finalY;
		//heatMapRepresentation.times[i] = zonas[i].timeInside;
		if(sumation == 0)
		{
			heatMapRepresentation.percentage[i] = 1.f/zonas.size();
		}
		else
		{
			heatMapRepresentation.percentage[i] = zonas[i].timeInside/sumation;
		}
	}
}
