#ifndef UTLRANDOM_HPP
#define UTLRANDOM_HPP

#include <chrono>
#include <random>

static std::mt19937 rndEngine(std::chrono::high_resolution_clock::now().time_since_epoch().count());

namespace utl
{
namespace random
{

int getUniformInt(const int min, const int max)
{
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rndEngine);
}

float getUniformFloat(const float min, const float max)
{
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rndEngine);
}

}
}

#endif // UTLRANDOM_HPP
