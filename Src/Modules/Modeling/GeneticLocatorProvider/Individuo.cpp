#include <stdlib.h>
#include "Individuo.h"
#include <math.h>
using namespace std;


// Function to generate random numbers in given range 
int random_num(int start, int end)
{
	int range = (end - start) + 1;
	int random_int = start + (rand() % range);
	return random_int;
}

Individuo::Individuo(string valor_)
{
	this->valor = valor_;

	int balon_x = 0;
	int balon_y = 0;
	int comp2_x = 0;
	int comp2_y = 0;
	int comp3_x = 0;
	int comp3_y = 0;
	int comp4_x = 0;
	int comp4_y = 0;

	int a = stoi(this->valor.substr(0, 4));
	int b = stoi(this->valor.substr(4, 8));

	//this->fitness = pow((this->valor - 423.9), 2);
	int ball = 0.3*(pow((a - balon_x), 2) + pow((b - balon_y), 2));
	int com2 = (pow((comp2_x - balon_x), 2) + pow((comp2_y - balon_x), 2));
	int g = 0.2*(pow((a - 3000), 2) + pow((b - 4500), 2));

	int shoe = (a * comp2_y) - (b * comp2_x) + (comp2_x * comp3_y) - (comp2_y * comp3_x) + (comp3_x * comp4_y) - (comp3_y * comp4_x) + (comp4_x * b) - (comp4_y * a);

	this->fitness = -shoe+ball+g;
}

/*// Overloading < operator 
bool operator<(const Individuo &ind1, const Individuo &ind2)
{
	return ind1.fitness < ind2.fitness;
}*/

//Metodo Nestor y Pala

string cross(string a, string b) {
	string genom_a = a;
	string genom_b = b;
	string child = "";

	for (int i = 0; i < 8; i++) {
		int prob = random_num(0, 100);

		if (prob < 30) {
			child += genom_a[i];
			//std::cout << "-30 ";
		}
		else if (prob < 60 && prob >= 30) {
			child += genom_b[i];
			//std::cout << "-60 ";
		}
		else if (prob < 90 and prob >= 60) {
			int prom = ((genom_a[i] - 48) + (genom_b[i] - 48))/2;
			child += std::to_string(prom);
			//std::cout << "promedio = " + std::to_string(prom) + "  ";
		}
		else {
			int rand = random_num(0, 9);
			child += std::to_string(rand);
			//std::cout << "ran = " + std::to_string(rand) + "  ";
		}
	}
	return child;
}