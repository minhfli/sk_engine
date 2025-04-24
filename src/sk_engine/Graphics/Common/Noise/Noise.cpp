#include "Noise.h"
#include "PerlinNoise.h"

#include <functional>

namespace sk_graphic {
    namespace {
        std::hash<int> int_hasher;
        uint32_t hash(int x, int y) {
            uint32_t h = static_cast<uint32_t>(x * 374761393 + y * 668265263); // large primes
            h = (h ^ (h >> 13)) * 1274126177;
            return h;
        }
    }

    bool Noise::filter(int x, int y, uint32_t lower, uint32_t upper) {


        return (GetValue(x, y) >= lower && GetValue(x, y) <= upper);
    }

    Texture2D Noise::genTexture(const int width, const int height) {
        unsigned char* data = new unsigned char[width * height * sizeof(unsigned char)];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                uint32_t value = GetValue(x, y);
                // Normalize the value to the range [0, 255]
                float normalized = static_cast<float>(value) / static_cast<float>(UINT32_MAX);
                uint8_t valueC = static_cast<uint8_t>(normalized * 255.0f);
                data[(y * width + x) + 0] = valueC;

                // data[3 * (y * width + x) + 0] = (value & 0xFF0000) >> 16;
                // data[3 * (y * width + x) + 1] = (value & 0x00FF00) >> 8;
                // data[3 * (y * width + x) + 2] = (value & 0x0000FF);
            }
        }
        Texture2D texture;
        texture.Load(width, height, 1, data);
        return texture;
    }

    PerlinNoise::PerlinNoise(int seed) {
        this->seed = seed;
        this->scale = 1.0f;
    }

    PerlinNoise::PerlinNoise(const std::string& seed) {
        this->seed = std::hash<std::string>{}(seed);
        this->scale = 1.0f;
    }

    uint32_t PerlinNoise::GetValue(int x, int y) {
        // Implement Perlin noise generation here
        // This is a placeholder implementation
        return hash(x, y) ^ seed;
    }

    float PerlinNoise::getValueF(int x, int y) {
        return (double)GetValue(x, y) / (double)std::numeric_limits<uint32_t>::max();
    }


}