#include "ValueMap2D.h"

namespace sk_math {
    sk_graphic::Texture2D ValueMap2D::toTexture() {
        unsigned char* data = new unsigned char[width * height * sizeof(unsigned char)];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float value = noise_map[y][x] / scale * 255.0f;
                uint8_t valueC = (uint8_t)value;
                data[(y * width + x) + 0] = valueC;
            }
        }
        sk_graphic::Texture2D texture;
        texture.Load(width, height, 1, data);
        return texture;
    }

    void ValueMap2D::normalize() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] /= scale;
            }
        }
        scale = 1.0f;
    }
    ValueMap2D ValueMap2D::normalized() {
        ValueMap2D result(width, height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result.noise_map[y][x] = noise_map[y][x] / scale;
            }
        }
        result.scale = 1.0f;
        return result;
    }

    void ValueMap2D::scale_(float scale) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                noise_map[y][x] *= scale;
            }
        }
        this->scale *= scale;
    }
    void ValueMap2D::setScale(float scale) {
        scale = scale / this->scale;
        scale_(scale);
    }

    Bitmask2D ValueMap2D::filter(float lower, float upper, bool normalize) {
        size_t height = noise_map.size();
        size_t width = noise_map[0].size();
        Bitmask2D result(width, height);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j];
                    if (value >= lower && value <= upper) {
                        result.set(j, i);
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float value = noise_map[i][j] / scale;
                    if (value >= lower && value <= upper) {
                        result.set(j, i);
                    }
                }
            }
        return result;
    }

    ValueMap2D ValueMap2D::filterValue(float lower, float upper, bool normalize) {
        size_t height = noise_map.size();
        size_t width = noise_map[0].size();
        ValueMap2D result(width, height);

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

    ValueMap2D ValueMap2D::filterBlackWhite(float lower, float upper, bool normalize) {
        size_t height = noise_map.size();
        size_t width = noise_map[0].size();
        ValueMap2D result(width, height);

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
    void ValueMap2D::add(const ValueMap2D& other) {
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

    ValueMap2D addValueMap2D(ValueMap2D& a, const ValueMap2D& b) {
        if (a.height != b.height || a.width != b.width) {
            throw std::invalid_argument("Noise maps must have the same dimensions to be added");
        }
        ValueMap2D result(a.width, a.height);
        for (int y = 0; y < a.height; y++) {
            for (int x = 0; x < a.width; x++) {
                result.noise_map[y][x] = a.noise_map[y][x] + b.noise_map[y][x];
            }
        }
        result.scale = a.scale + b.scale;
        return result;
    }
}