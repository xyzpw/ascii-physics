#include <random>
#include <limits>
#include "utils/random_utils.h"

const int MAX_SIZE_INT = std::numeric_limits<int>::max();

std::random_device randomDevice;
std::mt19937 mtGenerator(randomDevice());

std::uniform_int_distribution<int> intDistribution(0, MAX_SIZE_INT);

int randInt()
{
    return intDistribution(mtGenerator);
}
