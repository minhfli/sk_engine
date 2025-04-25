#pragma once

#include <vector>

#include <GLM/glm.hpp>
#include <sk_engine/Graphics/Texture2D.h>
#include "NoiseMap.h"

namespace sk_graphic {
    enum class NoiseType {
        NONE,
        RANDOM,
        PERLIN,

    };

    class Noise {
        public:
        Noise(const int seed = 0, const int samples = 16) : seed(seed), samples(samples) {}
        Noise(const std::string& seed, const int samples = 16) {
            std::hash<std::string> hasher;
            this->seed = static_cast<int>(hasher(seed));
        }

        int seed = 0;
        /// @brief the noise will sample value every samples pixels, for true random noise, set samples to 1
        int samples = 16;

        virtual float getValue01(int y, int x) = 0;        /// @brief return float value in range [0, 1]
        virtual float getValue255(int y, int x) = 0;        /// @brief return float value in range [0, 255] 
        virtual uint32_t GetValue(int y, int x) = 0;        /// @brief return uint32_t value in range [0, 2^32 - 1]


        inline bool filter01(int y, int x, float lower, float upper) {
            return (getValue01(x, y) >= lower && getValue01(x, y) <= upper);
        }
        inline bool filter255(int y, int x, float lower, float upper) {
            return (getValue255(x, y) >= lower && getValue255(x, y) <= upper);
        }
        inline bool filter(int y, int x, uint32_t lower, uint32_t upper) {
            return (GetValue(x, y) >= lower && GetValue(x, y) <= upper);
        }

        /// @brief generate a texture from the noise, you can use this texture to display
        Texture2D genTexture(const int height, const int width);

        /// @brief generate a 2D array of noise value, in range [0, 256] * scale, only use at startup, because it might be slow
        virtual std::vector<std::vector<float>> genNoiseMap255(const int height, const int width, const float scale = 1.0f);

        virtual NoiseMap toNoiseMap(const int height, const int width, const glm::ivec2 offset = glm::ivec2(0, 0));

    };
}