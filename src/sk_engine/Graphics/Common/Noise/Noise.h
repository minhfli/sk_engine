#pragma once


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
        int seed = 0;
        float scale = 1.0f;

        /**
         * @brief return value in range [0, 1]
         */
        virtual float getValueF(int x, int y) = 0;
        /**
         * @brief return value in range [0, 2^32 - 1]
          */
        virtual uint32_t GetValue(int x, int y) = 0;

        /**
         * @brief Return true if the noise value is in the range [lower, upper].
         */
        bool filter(int x, int y, uint32_t lower, uint32_t upper);

        Texture2D genTexture(const int width, const int height);

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