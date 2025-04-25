#pragma once

#include <vector>
#include <sk_engine/Graphics/Sprite2D.h>
#include <sk_engine/Graphics/Texture2D.h>
#include <GLM/glm.hpp>

namespace sk_graphic {

    /**
     * @brief 2D tilemap class
     * Tilemap is tiled from left to right, bottom to top
     */
    class Tilemap2D {
        public:
        Tilemap2D();

        ~Tilemap2D();

        /**
         * @brief Initialize the tilemap
         * @param width Width of the tilemap in tiles
         * @param height Height of the tilemap in tiles
         * @param depth Depth of the tilemap, default is 0
         * @param tile_size Size of each tile in world units
         * @param tile_gap Gap between tiles in world units
         * @param position Position of the tilemap in world units
         * @param pivot Pivot of the tilemap, default is center
         * @param parallax If true, the tilemap will move with the camera
         * @param parallax_speed Speed of the parallax effect
         */
        void Init(
            const int height,
            const int width,
            const float depth = 0.0f,
            const glm::vec2 tile_size = glm::vec2(1.0f),
            const glm::vec2 tile_gap = glm::vec2(0.0f),
            const glm::vec2 position = glm::vec2(0.0f),
            const glm::vec2 pivot = glm::vec2(0.5f),
            const bool parallax = false,
            const float parallax_speed = 0.5f
        );
        // void Load(const std::string& path, const int width, const int height, const glm::vec2 tile_size = glm::vec2(1.0f));

        void Draw(const glm::vec2 pos_offset, const glm::vec4 color = glm::vec4(1.0f));

        void SetTile(int x, int y, const Sprite2D& sprite);
        void DeleteTile(int x, int y);
        int GetTile(int x, int y) const;


        glm::vec2 GetPosition(const glm::vec2 pivot = glm::vec2(0.5f)) const;
        float depth; // change this to change the depth of the tilemap


        private:
        glm::vec2 pivot; // pivot of the tilemap, default is center, used to calculate parallax effect
        bool parallax = false; // if true, the tilemap will move with the camera
        float parallax_speed = 0.5f; // speed of the parallax effect
        int width;
        int height;
        glm::vec2 tile_size;
        glm::vec2 tile_gap;
        glm::vec2 map_size; // real size of the tilemap
        glm::vec2 position; // lower left corner 
        bool is_loaded = false;
        std::vector<std::vector<Sprite2D>> tilemap;

    };


}