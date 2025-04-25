#pragma once
#include <vector>
#include <stdexcept>

#include <sk_engine/Graphics/Texture2D.h>
#include <sk_engine/Common/Bitset2D.h>

namespace sk_graphic {

    /// @brief A class to represent a 2D noise, default range is [0, 256] (float) or [0, 255] (casted to uint8_t)
    class NoiseMap {
        public:
        NoiseMap(const NoiseMap& other) = default;
        NoiseMap(const int height, const int width) : width(width), height(height) {
            noise_map = std::vector<std::vector<float>>(height, std::vector<float>(width, 0));
        }
        NoiseMap(const int height, const int width, const std::vector<std::vector<float>>& noise_map, const float scale = 1.0f)
            : width(width), height(height), noise_map(noise_map), scale(scale) {
            if (noise_map.size() != height || noise_map[0].size() != width) {
                throw std::invalid_argument("Noise map size does not match the specified width and height");
            }
        }
        ~NoiseMap() = default;

        const int height;
        const int width;
        std::vector<std::vector<float>> noise_map;

        /// @brief Scale factor, used when adding noise maps together, then is divided when generating the texture
        float scale = 1.0f;

        Texture2D toTexture();

        // aceess
        std::vector<float>& operator[](int i) {
            return noise_map[i % height];
        }

        float& operator()(int row, int col) {
            return noise_map[row % height][col % width];
        }

        /// @brief reset scale factor to 1.0f
        void normalize();
        /// @brief create a new noise map and normalize it
        NoiseMap normalized();

        /// @brief scale value of noise map
        void scale_(float scale);

        /// @brief  scale noisemap to this value
        void setScale(float scale);

        /// @brief filter the noise map to a bitset
        /// @param lower 
        /// @param upper 
        /// @param normalize normalize the value to [0, 255] before filtering, this will not change the original noise map
        /// @return 
        Bitset2D filter(float lower, float upper, bool normalize = false);

        /// @brief filter value of the noise map to a new noise map
        /// @param lower 
        /// @param upper 
        /// @param normalize normalize the value to [0, 255] before filtering, this will not change the original noise map
        /// @return 
        NoiseMap filterValue(float lower, float upper, bool normalize = false);

        /// @brief the value beetwen lower and upper will be set to 255, the rest will be set to 0
        /// @param lower 
        /// @param upper 
        /// @param normalize 
        /// @return 
        NoiseMap filterBlackWhite(float lower, float upper, bool normalize = false);

        void add(const NoiseMap& other);
    };

    NoiseMap addNoiseMap(NoiseMap& a, const NoiseMap& b);

}