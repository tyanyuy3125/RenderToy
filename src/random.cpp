#include "random.h"

#include <random>

static std::mt19937 random_device;

namespace RenderToy
{
    /// @brief Generates an int between min and max.
    /// @param min 
    /// @param max 
    /// @return 
    const int Random::Int(int min, int max)
    {
        std::uniform_int_distribution<> distrib(min, max);
        return distrib(random_device);
    }

    /// @brief Generates a float between 0.0f and 1.0f.
    /// @return 
    const float Random::Float()
    {
        std::uniform_real_distribution<float> distrib(0.0f, 1.0f);
        return distrib(random_device);
    }
}