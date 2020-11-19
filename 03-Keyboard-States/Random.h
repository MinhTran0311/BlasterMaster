
#include <cstdlib> // for rand() and srand()
#include <ctime> // for time()
using namespace std;

class Random
{
public: 
	int getRandomInt(int min = 0, int max = 32767)
	{
		srand(static_cast <unsigned> (time(0)));
		int random = rand() % max + min;
		return random;
	}

	float getRandomFloat(float min = 0, float max = RAND_MAX)
	{
		srand(static_cast <unsigned> (time(0)));
		float r3 = min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
		return r3;
	}
};