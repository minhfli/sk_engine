#pragma once

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <random>
#include <vector>
#include <string>

#include <GLM/glm.hpp>

#include "Noise.h"

namespace sk_graphic {
    class PerlinNoise : public Noise {
        public:
        PerlinNoise(int seed = 0);
        PerlinNoise(const std::string& seed);
        ~PerlinNoise() = default;

        float getValueF(int x, int y) override;
        uint32_t GetValue(int x, int y) override;
    };
}