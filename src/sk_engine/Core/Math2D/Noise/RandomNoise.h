#pragma once


#include <GLM/glm.hpp>

#include "Base.h"

namespace sk_math {
    class RandomNoise : public Noise {
        public:
        RandomNoise(const int seed = 0, const int samples = 16) : Noise(seed, samples) {}
        RandomNoise(const std::string& seed, const int samples = 16) : Noise(seed, samples) {}
        ~RandomNoise() = default;

        uint32_t GetValue(int y, int x) override;
        float getValue01(int y, int x) override;
        float getValue255(int y, int x) override;
    };
}