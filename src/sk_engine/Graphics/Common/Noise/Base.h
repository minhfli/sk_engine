#pragma once

#include <vector>

#include <GLM/glm.hpp>
#include <sk_engine/Graphics/Texture2D.h>


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

        /// @brief return float value in range [0, 1]
        virtual float getValue01(int x, int y) = 0;

        /// @brief return float value in range [0, 255] 
        virtual float getValue255(int x, int y) = 0;

        /// @brief return uint32_t value in range [0, 2^32 - 1]
        virtual uint32_t GetValue(int x, int y) = 0;

        inline bool filter01(int x, int y, float lower, float upper) {
            return (getValue01(x, y) >= lower && getValue01(x, y) <= upper);
        }
        inline bool filter255(int x, int y, float lower, float upper) {
            return (getValue255(x, y) >= lower && getValue255(x, y) <= upper);
        }
        inline bool filter(int x, int y, uint32_t lower, uint32_t upper) {
            return (GetValue(x, y) >= lower && GetValue(x, y) <= upper);
        }

        /// @brief generate a texture from the noise, you can use this texture to display
        Texture2D genTexture(const int width, const int height);

        /// @brief generate a 2D array of noise value, in range [0, 256]
        virtual std::vector<std::vector<float>> genNoiseMap255(const int width, const int height);

        inline int setseed(const int s) {
            seed = s;
            return seed;
        }
        inline int setseed() {
            seed = std::rand();
            return seed;
        }
        inline int setseed(const std::string& s) {
            std::hash<std::string> hasher;
            seed = static_cast<int>(hasher(s));
            return seed;
        }


    };
}