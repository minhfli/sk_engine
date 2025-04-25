#include "Base.h"
#include "RandomNoise.h"
#include "ValueNoise.h"
#include "PerlinNoise.h"
#include <iostream>
#include <functional>
#include <algorithm>
#include <vector>
#include <cmath>
#include <math.h>
#include <random>

constexpr double M_PI = 3.14159265358979323846;

namespace sk_graphic {
    namespace {
        std::hash<int> int_hasher;
        uint32_t hash(int y, int x, int seed = 0) {
            uint32_t h = static_cast<uint32_t>(x * 374761393 + y * 668265263 + seed); // large primes
            h = (h ^ (h >> 13)) * 1274126177;
            return h;
        }

        float lerp(float a, float b, float t) {
            return a + t * (b - a);
        }
        float smoothstep(float a, float b, float t) {
            return lerp(a, b, t * t * (3 - 2 * t));
        }
        static float fade(float t) {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }
    }
#pragma region BaseNoise
    Texture2D Noise::genTexture(const int height, const int width) {
        unsigned char* data = new unsigned char[width * height * sizeof(unsigned char)];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float value = 0.0f;
                try {
                    value = getValue255(x, y);
                }
                catch (...) {
                    std::cout << "GetValue255 is not implemented in this class" << std::endl;
                }

                uint8_t valueC = (uint8_t)value;
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

    std::vector<std::vector<float>> Noise::genNoiseMap255(const int height, const int width, const float scale) {
        std::vector<std::vector<float>> noise_map(height, std::vector<float>(width, 0));
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] = getValue255(x, y) * scale;
            }
        }
        return noise_map;
    }

    NoiseMap Noise::toNoiseMap(const int height, const int width, const glm::ivec2 offset) {
        std::vector<std::vector<float>> noise_map(height, std::vector<float>(width, 0));
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] = getValue255(y, x);
            }
        }
        return NoiseMap(height, width, noise_map);
    }

#pragma endregion BaseNoise

#pragma region RandomNoise

    uint32_t RandomNoise::GetValue(int y, int x) {
        return hash(x - x % samples, y - y % samples, seed);
    }

    float RandomNoise::getValue01(int y, int x) {
        return static_cast<double>(GetValue(x, y)) / static_cast<double>(UINT32_MAX);
    }

    float RandomNoise::getValue255(int y, int x) {
        return static_cast<double>(GetValue(x, y)) / static_cast<double>(UINT32_MAX >> 8);
    }
#pragma endregion RandomNoise

#pragma region ValueNoise
    uint32_t ValueNoise::GetValue(int y, int x) {
        return hash(x - x % samples, y - y % samples, seed);
    }

    float ValueNoise::getValue01(int y, int x) {
        int mod_x = x % samples;
        int lo_x = x - mod_x;
        int hi_x = lo_x + samples;
        int mod_y = y % samples;
        int lo_y = y - mod_y;
        int hi_y = lo_y + samples;

        float lolo = static_cast<double>(GetValue(lo_x, lo_y)) / static_cast<double>(UINT32_MAX);
        float lohi = static_cast<double>(GetValue(lo_x, hi_y)) / static_cast<double>(UINT32_MAX);
        float hilo = static_cast<double>(GetValue(hi_x, lo_y)) / static_cast<double>(UINT32_MAX);
        float hihi = static_cast<double>(GetValue(hi_x, hi_y)) / static_cast<double>(UINT32_MAX);

        // lerp x at low y
        float lerp_x_Lo_y = smoothstep(lolo, hilo, static_cast<double>(mod_x) / static_cast<double>(samples));
        // lerp x at high y
        float lerp_x_Hi_y = smoothstep(lohi, hihi, static_cast<double>(mod_x) / static_cast<double>(samples));
        // lerp y
        return smoothstep(lerp_x_Lo_y, lerp_x_Hi_y, static_cast<double>(mod_y) / static_cast<double>(samples));
    }

    float ValueNoise::getValue255(int y, int x) {
        int mod_x = x % samples;
        int lo_x = x - mod_x;
        int hi_x = lo_x + samples;
        int mod_y = y % samples;
        int lo_y = y - mod_y;
        int hi_y = lo_y + samples;

        float lolo = static_cast<double>(GetValue(lo_x, lo_y)) / static_cast<double>(UINT32_MAX >> 8);
        float lohi = static_cast<double>(GetValue(lo_x, hi_y)) / static_cast<double>(UINT32_MAX >> 8);
        float hilo = static_cast<double>(GetValue(hi_x, lo_y)) / static_cast<double>(UINT32_MAX >> 8);
        float hihi = static_cast<double>(GetValue(hi_x, hi_y)) / static_cast<double>(UINT32_MAX >> 8);

        // lerp x at low y
        float lerp_x_Lo_y = smoothstep(lolo, hilo, static_cast<double>(mod_x) / static_cast<double>(samples));
        // lerp x at high y
        float lerp_x_Hi_y = smoothstep(lohi, hihi, static_cast<double>(mod_x) / static_cast<double>(samples));
        // lerp y
        return smoothstep(lerp_x_Lo_y, lerp_x_Hi_y, static_cast<double>(mod_y) / static_cast<double>(samples));
    }
#pragma endregion ValueNoise

#pragma region PerlinNoise


    void PerlinNoise::initGradients() {
        int gridSize = 512 / samples + 2;
        gradients = std::vector<std::vector<glm::vec2>>(gridSize, std::vector<glm::vec2>(gridSize));
        std::mt19937 rng(seed);
        std::uniform_real_distribution<float> dist(0.0f, 2 * M_PI);

        for (int y = 0; y < gridSize; ++y) {
            for (int x = 0; x < gridSize; ++x) {
                float angle = dist(rng);
                gradients[y][x] = { std::cos(angle), std::sin(angle) };
            }
        }
    }

    static float dotGridGradient(int ix, int iy, float x, float y, const std::vector<std::vector<glm::vec2>>& grads) {
        float dx = x - ix;
        float dy = y - iy;
        glm::vec2 grad = grads[iy][ix];
        return dx * grad.x + dy * grad.y;
    }

    float PerlinNoise::perlin(float x, float y) {
        int x0 = (int)std::floor(x);
        int x1 = x0 + 1;
        int y0 = (int)std::floor(y);
        int y1 = y0 + 1;

        float sx = fade(x - x0);
        float sy = fade(y - y0);

        float n0 = dotGridGradient(x0, y0, x, y, gradients);
        float n1 = dotGridGradient(x1, y0, x, y, gradients);
        float ix0 = lerp(n0, n1, sx);

        n0 = dotGridGradient(x0, y1, x, y, gradients);
        n1 = dotGridGradient(x1, y1, x, y, gradients);
        float ix1 = lerp(n0, n1, sx);

        return lerp(ix0, ix1, sy);
    }

    float PerlinNoise::getValue01(int y, int x) {
        float nx = static_cast<float>(x) / samples;
        float ny = static_cast<float>(y) / samples;
        return (perlin(nx, ny) + 1.0f) * 0.5f;
    }

    float PerlinNoise::getValue255(int y, int x) {
        return getValue01(x, y) * 255.0f;
    }

    uint32_t PerlinNoise::GetValue(int y, int x) {
        return static_cast<uint32_t>(getValue01(x, y) * 4294967295.0f);
    }
#pragma endregion PerlinNoise
}