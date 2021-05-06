#include <random>
#include "generator.h"

std::default_random_engine generator = std::default_random_engine(std::random_device()());
std::uniform_int_distribution<int> distribution(0, 405 / 15 - 1);

