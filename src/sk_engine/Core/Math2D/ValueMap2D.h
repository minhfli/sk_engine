#pragma once
#include <vector>
#include <stdexcept>

#include <sk_engine/Graphics/Texture2D.h>
#include <sk_engine/Core/Math2D/Bitset2D.h>

namespace sk_math {

    /// @brief A class to represent a 2D noise, default range is [0, 1]
    class ValueMap2D {
        public:
        ValueMap2D(const ValueMap2D& other) = default;
        ValueMap2D(const int width, const int height) : width(width), height(height) {
            value = std::vector<std::vector<float>>(height, std::vector<float>(width, 0));
        }
        ValueMap2D(const int width, const int height, const std::vector<std::vector<float>>& value, const float scale = 1.0f)
            : width(width), height(height), value(value), scale(scale) {
            if (value.size() != height || value[0].size() != width) {
                throw std::invalid_argument("Noise map size does not match the specified width and height");
            }
        }
        ~ValueMap2D() = default;

        const int height;
        const int width;
        std::vector<std::vector<float>> value;

        /// @brief Scale factor, used when adding noise maps together, then is divided when generating the texture
        float scale = 1.0f;

        sk_graphic::Texture2D toTexture();

        // aceess
        std::vector<float>& operator[](int i) {
            return value[i % height];
        }

        float& operator()(int x, int y) {
            return value[y % height][x % width];
        }

        /// @brief reset scale factor to 1.0f
        void normalize();
        /// @brief create a new noise map and normalize it
        ValueMap2D normalized();

        /// @brief scale value of noise map
        void scale_(float scale);

        /// @brief  scale ValueMap2D to this value
        void setScale(float scale);

        /// @brief filter the noise map to a bitset
        /// @param lower 
        /// @param upper 
        /// @param normalize normalize the value to [0, 255] before filtering, this will not change the original noise map
        /// @return 
        Bitmask2D filter(float lower, float upper, bool normalize = false);

        /// @brief filter value of the noise map to a new noise map
        /// @param lower 
        /// @param upper 
        /// @param normalize normalize the value to [0, 255] before filtering, this will not change the original noise map
        /// @return 
        ValueMap2D filterValue(float lower, float upper, bool normalize = false);

        /// @brief the value beetwen lower and upper will be set to 255, the rest will be set to 0
        /// @param lower 
        /// @param upper 
        /// @param normalize 
        /// @return 
        ValueMap2D filterBlackWhite(float lower, float upper, bool normalize = false);

        void add(const ValueMap2D& other);
        void mul(const ValueMap2D& other);

        /// @brief flip the value, return as (1-value/scale)*scale
        void flip();

    };

    ValueMap2D addValueMap2D(ValueMap2D& a, const ValueMap2D& b);
    ValueMap2D mulValueMap2D(ValueMap2D& a, const ValueMap2D& b);

}