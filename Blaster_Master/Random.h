//#include <cstdlib> // for rand() and srand()
#pragma once
#include <random>
#include <ctime>
using namespace std;

class Random
{
public: 
	int getRandomInt(int min = 0, int max = 32767)
	{
		//srand((time(NULL)));
		int random = rand() % max + min;
		return random;
	}

	float getRandomFloat(float min = 0, float max = RAND_MAX)
	{
		//srand(time(NULL));
		float r3 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		return r3;

	}
};