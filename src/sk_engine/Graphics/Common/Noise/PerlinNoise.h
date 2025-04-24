#pragma once

#include <GLM/glm.hpp>

#include "Base.h"

namespace sk_graphic {
    class PerlinNoise : public Noise {
        public:
        PerlinNoise(int seed = 0, int samples = 16) : Noise(seed, samples) {
            initGradients();
        }
        PerlinNoise(const std::string& seedStr, int samples = 16) : Noise(seedStr, samples) {
            initGradients();
        }
        ~PerlinNoise() = default;

        uint32_t GetValue(int x, int y) override;
        float getValue01(int x, int y) override;
        float getValue255(int x, int y) override;
        public:
        std::vector<std::vector<glm::vec2>> gradients;

        std::vector<std::vector<float>> genNoiseMap255(const int width, const int height) override;
        private:
        void initGradients();

        // static float dotGridGradient(int ix, int iy, float x, float y, const std::vector<std::vector<glm::vec2>>& grads);

        float perlin(float x, float y);

    };
}