#pragma once

#include <GLM/glm.hpp>

#include "Base.h"

namespace sk_math {
    class PerlinNoise : public Noise {
        public:
        PerlinNoise(int seed = 0, int samples = 16) : Noise(seed, samples) {
            // initGradients();
        }
        PerlinNoise(const std::string& seedStr, int samples = 16) : Noise(seedStr, samples) {
            // initGradients();
        }
        ~PerlinNoise() = default;

        uint32_t GetValue(int y, int x) override;
        float getValue01(int y, int x) override;
        float getValue255(int y, int x) override;
        public:
        // std::vector<std::vector<glm::vec2>> gradients;

        private:
        // void initGradients();
        glm::vec2 getGradient(int ix, int iy) const;

        float dotGridGradient(int ix, int iy, float x, float y);

        float perlin(float x, float y);

    };
}