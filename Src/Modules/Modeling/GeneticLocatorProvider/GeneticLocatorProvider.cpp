#include "GeneticLocatorProvider.h"
//#include "stdafx.h"
#include <string>

using namespace std;

#define POPULATION_SIZE 1000

MAKE_MODULE(GeneticLocatorProvider, modeling);

GeneticLocatorProvider::GeneticLocatorProvider()
{
	
}
 
void GeneticLocatorProvider::update(GeneticLocator& geneticLocator)
{
	vector<Individuo> poblacion;
	string solution;

	for (int i = 0; i < POPULATION_SIZE; i++)
	{
		string num = std::to_string(random_num(0, 99999999));
		while (num.length() < 9){
			num = "0" + num;
		}

		poblacion.push_back(Individuo(num));
	}
	int iteracion = 0;
	while (iteracion < 1000)
	{
		//Organizamos la población de menor a mayor
		sort(poblacion.begin(), poblacion.end());

		//Creamos una nueva generación vacía
		vector<Individuo> nuevosIndividuos;

		//El 10% con mejor fitness pasa a la siguiente generación automáticamente
		int s = (10 * POPULATION_SIZE) / 100;
		for (int i = 0; i < s; i++)
		nuevosIndividuos.push_back(poblacion[i]);

		//Con el mejor 50% realizamos los crossovers y lo metemos en el otro 90% disponible
		s = (90 * POPULATION_SIZE) / 100;
		for (int i = 0; i < s; i++)
		{
		int len = poblacion.size();

		// Primer padre aleatorio
		int r = random_num(0, len / 2);
		Individuo padre1 = poblacion[r];

		// Segundo padre aleatorio
		r = random_num(0, len / 2);
		Individuo padre2 = poblacion[r];

		Individuo Hijo = Individuo(cross(padre1.valor, padre2.valor));
		nuevosIndividuos.push_back(Hijo);
		}

		// La población se vuelve los nuevos individuos
		poblacion = nuevosIndividuos;

		iteracion++;
		// Solucion al algoritmo genético poblacion[0].valor
	}
	solution = poblacion[0].valor;
	geneticLocator.optimalX = stoi(solution.substr(0,4));
	geneticLocator.optimalY = stoi(solution.substr(4,8));
}

// Function to generate random numbers in given range 
int GeneticLocatorProvider::random_num(int start, int end)
{
	int range = (end - start) + 1;
	int random_int = start + (rand() % range);
	//string random = "random = " + std::to_string(random_int) + "  " ;
	//std::cout << random;
	return random_int;
}

// Overloading < operator 
bool operator<(const Individuo &ind1, const Individuo &ind2)
{
	return ind1.fitness < ind2.fitness;
}

//Metodo Nestor y Pala

string GeneticLocatorProvider::cross(string a, string b) {
	string genom_a = a;
	string genom_b = b;
	string child = "";

	for (int i = 0; i < 8; i++) {
		int prob = random_num(0, 100);

		if (prob < 30) {
			child += genom_a[i];
		}
		else if (prob < 60 && prob >= 30) {
			child += genom_b[i];
		}
		else if (prob < 90 and prob >= 60) {
			int prom = ((genom_a[i] - 48) + (genom_b[i] - 48))/2;
			child += std::to_string(prom);
		}
		else {
			int rand = random_num(0, 9);
			child += std::to_string(rand);
		}
	}
	return child;
}