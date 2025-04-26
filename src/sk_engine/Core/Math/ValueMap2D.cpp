#include "ValueMap2D.h"

namespace sk_math {
    sk_graphic::Texture2D ValueMap2D::toTexture() {
        unsigned char* data = new unsigned char[width * height * sizeof(unsigned char)];
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                float val = value[y][x] / scale * 256.0f;
                uint8_t valueC = (uint8_t)val;
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
                value[y][x] /= scale;
            }
        }
        scale = 1.0f;
    }
    ValueMap2D ValueMap2D::normalized() {
        ValueMap2D result(width, height);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                result.value[y][x] = value[y][x] / scale;
            }
        }
        result.scale = 1.0f;
        return result;
    }

    void ValueMap2D::scale_(float scale) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                value[y][x] *= scale;
            }
        }
        this->scale *= scale;
    }
    void ValueMap2D::setScale(float scale) {
        scale = scale / this->scale;
        scale_(scale);
    }

    Bitmask2D ValueMap2D::filter(float lower, float upper, bool normalize) {
        size_t height = value.size();
        size_t width = value[0].size();
        Bitmask2D result(width, height);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float val = value[i][j];
                    if (val >= lower && val <= upper) {
                        result.set(j, i);
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float val = value[i][j] / scale;
                    if (val >= lower && val <= upper) {
                        result.set(j, i);
                    }
                }
            }
        return result;
    }

    ValueMap2D ValueMap2D::filterValue(float lower, float upper, bool normalize) {
        size_t height = value.size();
        size_t width = value[0].size();
        ValueMap2D result(width, height);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float val = value[i][j];
                    if (val >= lower && val <= upper) {
                        result.value[i][j] = val;
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float val = value[i][j] / scale;
                    if (val >= lower && val <= upper) {
                        result.value[i][j] = val;
                    }
                }
            }
        return result;
    }

    ValueMap2D ValueMap2D::filterBlackWhite(float lower, float upper, bool normalize) {
        size_t height = value.size();
        size_t width = value[0].size();
        ValueMap2D result(width, height);

        if (!normalize)
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float val = value[i][j];
                    if (val >= lower && val <= upper) {
                        result.value[i][j] = 1;
                    }
                }
            }
        else
            for (size_t i = 0; i < height; ++i) {
                for (size_t j = 0; j < width; ++j) {
                    float val = value[i][j] / scale;
                    if (val >= lower && val <= upper) {
                        result.value[i][j] = 1;
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
                value[y][x] += other.value[y][x];
            }
        }
        scale += other.scale;
    }

    void ValueMap2D::flip() {
        for (int y = 0; y < height; y++)
            for (int x = 0; x < width; x++)
                value[y][x] += scale - value[y][x];
    }

    ValueMap2D addValueMap2D(ValueMap2D& a, const ValueMap2D& b) {
        if (a.height != b.height || a.width != b.width) {
            throw std::invalid_argument("Noise maps must have the same dimensions to be added");
        }
        ValueMap2D result(a.width, a.height);
        for (int y = 0; y < a.height; y++) {
            for (int x = 0; x < a.width; x++) {
                result.value[y][x] = a.value[y][x] + b.value[y][x];
            }
        }
        result.scale = a.scale + b.scale;
        return result;
    }

    void ValueMap2D::mul(const ValueMap2D& other) {
        if (height != other.height || width != other.width) {
            throw std::invalid_argument("Noise maps must have the same dimensions to be multiplied");
            return;
        }
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                value[y][x] *= other.value[y][x];
            }
        }
        scale *= other.scale;
    }

    ValueMap2D mulValueMap2D(ValueMap2D& a, const ValueMap2D& b) {
        if (a.height != b.height || a.width != b.width) {
            throw std::invalid_argument("Noise maps must have the same dimensions to be multiplied");
        }
        ValueMap2D result(a.width, a.height);
        for (int y = 0; y < a.height; y++) {
            for (int x = 0; x < a.width; x++) {
                result.value[y][x] = a.value[y][x] * b.value[y][x];
            }
        }
        result.scale = a.scale * b.scale;
        return result;
    }
}