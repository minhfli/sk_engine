#pragma once


#include <GLM/glm.hpp>

#include "Base.h"

namespace sk_graphic {
    class ValueNoise : public Noise {
        public:
        ValueNoise(const int seed = 0, const int samples = 16) : Noise(seed, samples) {}
        ValueNoise(const std::string& seed, const int samples = 16) : Noise(seed, samples) {}
        ~ValueNoise() = default;

        uint32_t GetValue(int x, int y) override;
        float getValue01(int x, int y) override;
        float getValue255(int x, int y) override;
    };
}