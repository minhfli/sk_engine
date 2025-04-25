#include "NoiseMap.h"

namespace sk_math {
    sk_graphic::Texture2D NoiseMap::toTexture() {
        unsigned char* data = new unsigned char[width * height * sizeof(unsigned char)];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float value = noise_map[y][x] / scale;
                uint8_t valueC = (uint8_t)value;
                data[(y * width + x) + 0] = valueC;
            }
        }
        sk_graphic::Texture2D texture;
        texture.Load(width, height, 1, data);
        return texture;
    }

    void NoiseMap::normalize() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] /= scale;
            }
        }
        scale = 1.0f;
    }
    NoiseMap NoiseMap::normalized() {
        NoiseMap result(height, width);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result.noise_map[y][x] = noise_map[y][x] / scale;
            }
        }
        result.scale = 1.0f;
        return result;
    }

    void NoiseMap::scale_(float scale) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] *= scale;
            }
        }
        this->scale *= scale;
    }
    void NoiseMap::setScale(float scale) {
        scale = scale / this->scale;
        scale_(scale);
    }

    Bitset2D NoiseMap::filter(float lower, float upper, bool normalize) {
        size_t height = noise_map.size();
        size_t width = noise_map[0].size();
        DynamicBitset2D result(height, width);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j];
                    if (value >= lower && value <= upper) {
                        result.set(i, j);
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j] / scale;
                    if (value >= lower && value <= upper) {
                        result.set(i, j);
                    }
                }
            }
        return result;
    }

    NoiseMap NoiseMap::filterValue(float lower, float upper, bool normalize) {
        size_t height = noise_map.size();
        size_t width = noise_map[0].size();
        NoiseMap result(height, width);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j];
                    if (value >= lower && value <= upper) {
                        result.noise_map[i][j] = value;
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j] / scale;
                    if (value >= lower && value <= upper) {
                        result.noise_map[i][j] = value;
                    }
                }
            }
        return result;
    }

    NoiseMap NoiseMap::filterBlackWhite(float lower, float upper, bool normalize) {
        size_t height = noise_map.size();
        size_t width = noise_map[0].size();
        NoiseMap result(height, width);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j];
                    if (value >= lower && value <= upper) {
                        result.noise_map[i][j] = 255;
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j] / scale;
                    if (value >= lower && value <= upper) {
                        result.noise_map[i][j] = 255;
                    }
                }
            }
        return result;
    }
    void NoiseMap::add(const NoiseMap& other) {
        if (height != other.height || width != other.width) {
            throw std::invalid_argument("Noise maps must have the same dimensions to be added");
            return;
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] += other.noise_map[y][x];
            }
        }
        scale += other.scale;
    }

    NoiseMap addNoiseMap(NoiseMap& a, const NoiseMap& b) {
        if (a.height != b.height || a.width != b.width) {
            throw std::invalid_argument("Noise maps must have the same dimensions to be added");
        }
        NoiseMap result(a.height, a.width);
        for (int y = 0; y < a.height; y++) {
            for (int x = 0; x < a.width; x++) {
                result.noise_map[y][x] = a.noise_map[y][x] + b.noise_map[y][x];
            }
        }
        result.scale = a.scale + b.scale;
        return result;
    }
}