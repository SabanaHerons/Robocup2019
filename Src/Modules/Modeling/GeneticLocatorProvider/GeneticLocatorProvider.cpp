#include "GeneticLocatorProvider.h"
#include <string>
#include <iostream>
#include <math.h>

using namespace std;

MAKE_MODULE(GeneticLocatorProvider, modeling);

GeneticLocatorProvider::GeneticLocatorProvider()
{
}

void GeneticLocatorProvider::update(GeneticLocator& geneticLocator)
{
	this -> geneticLocatorr = &geneticLocator;

	geneticLocator.activation = [this](bool activates, int ctime) -> Pose2f
	{
		if(activates && ctime - theGeneticLocator.lastime > 3000){
			vector<vector<float>> poblacion;
			vector<vector<int>> posbls;

			for (int i = -50; i <= 50; i++){
				for (int j = -50; j <= 50; j++){
					posbls.push_back({i*90,j*60});
				}
			}

			for (int w = 0; w < 10000; w++)
			{
				poblacion.push_back({static_cast<float>(posbls[w][0]), static_cast<float>(posbls[w][1]), this -> result(posbls[w][0],posbls[w][1])});
			}

			for(int ym = 0; ym < 10000; ym++){
				if(poblacion[ym][2] > poblacion[0][2])
				{
					vector<float> aux = poblacion[0];
					poblacion[0] = poblacion[ym];
					poblacion[ym] = aux;
				}
			}
			this -> geneticLocatorr -> optimalX = poblacion[0][0];
			this -> geneticLocatorr -> optimalY = poblacion[0][1];

			this -> geneticLocatorr -> Hmtimes = theGeneticLocator.Hmtimes + 1;
			this -> geneticLocatorr -> lastime = ctime;
		}

		return Pose2f(theBallModel.estimate.rotation,this -> geneticLocatorr -> optimalX,this -> geneticLocatorr -> optimalY);
	};

	geneticLocator.optimalX = theGeneticLocator.optimalX;
	geneticLocator.optimalY = theGeneticLocator.optimalY;
	geneticLocator.lastime = theGeneticLocator.lastime;
	geneticLocator.Hmtimes = theGeneticLocator.Hmtimes;
}

float GeneticLocatorProvider::result(int a, int b) {
	float ballWeight = float(0);
	float areaWeight = float(1);
	float cornerWeight = float(0);
	float arcoWeight = float(0);


	float area = 2;
	float ball = (float)(pow((a - theBallModel.estimate.position.x()), 2) + pow((b -  theBallModel.estimate.position.y()), 2));
	float corner = (float)(pow((a - 3000), 2) + pow((b - 4500), 2));
	float arco = (float)(pow((a - (-4500)), 2) + pow((b - 0), 2)); //Problema por posicion relativa

	if(theTeamData.teammates.size()>1){
		std::vector<std::vector<float>> vov = ordenador(a,b);
		int tam = (int)vov.size()-1;
		area = vov[1][0];
		area = 0;

		for (int i = 0; i < tam; i++)
			area = area + ((vov[i][0] * vov[i+1][1]) - (vov[i][1] * vov[i+1][0]));
		area *= -0.5;
	}
	else if(theTeamData.teammates.size() == 1)
			area = (float)(sqrt(pow(abs(theTeamData.teammates[0].theRobotPose.translation.y()-b),2) + pow(abs(theTeamData.teammates[0].theRobotPose.translation.x()-a),2)));

	return ball * ballWeight + area * areaWeight + corner * cornerWeight + arco * arcoWeight;
}

vector<vector<float>> GeneticLocatorProvider::ordenador(int a, int b){
	vector<vector<float>> vov = {{static_cast<float>(b),static_cast<float>(a)}};//{{30,30},{30,0},{0,30},{0,0}};
	int num_compas = (int)vov.size();

	for (int i = 0; i < num_compas; i++){
		vov.push_back({theTeamData.teammates[i].theRobotPose.translation.y(),theTeamData.teammates[i].theRobotPose.translation.x()});
	}

	for(int ym = 1; ym < num_compas; ym++){
    if(vov[ym][1] < vov[0][1])
    {
      std::vector<float> aux = vov[0];
      vov[0] = vov[ym];
      vov[ym] = aux;
    }
    else if(vov[ym][1] == vov[0][1])
      if(vov[ym][0] < vov[0][0])
      {
        std::vector<float> aux = vov[0];
        vov[0] = vov[ym];
        vov[ym] = aux;
      }
  }

	vector<float> point = vov[0];

	for(int ct = 1; ct < num_compas; ct++){
    float cost = 0;
    float y = vov[ct][1] - point[1];
    float x = vov[ct][0] - point[0];

    if (x == 0)
      cost = y + 150;
    else
      cost = y / x;

    if (cost < 0)
      cost = 22500 / abs(cost) + 9001;

    vov[ct].push_back(cost);
  }

	for (int i = 1; i < int(vov.size()); i++){
    for (int j = 1; j < int(vov.size()); j++){
      if (vov[i][2]>vov[j][2]){
        vector<float> aux = vov[i];
        vov[i] = vov[j];
        vov[j] = aux;
      }
    }
  }

	vov.push_back(point);

	return vov;
}
