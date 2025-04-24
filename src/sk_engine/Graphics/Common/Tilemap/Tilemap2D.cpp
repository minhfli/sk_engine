#include "Tilemap2D.h"

namespace sk_graphic {

    Tilemap2D::Tilemap2D() {}

    Tilemap2D::~Tilemap2D() {
        tilemap.clear();
    }

    void Tilemap2D::Init(
        const int width,
        const int height,
        const float depth,
        const glm::vec2 tile_size,
        const glm::vec2 tile_gap,
        const glm::vec2 position,
        const glm::vec2 pivot,
        const bool parallax,
        const float parallax_speed
    ) {
        this->width = width;
        this->height = height;
        this->depth = depth;
        this->tile_size = tile_size;
        this->position = position;
        this->pivot = pivot;
        this->parallax = parallax;
        this->parallax_speed = parallax_speed;

        map_size.x = width * tile_size.x;
        map_size.y = height * tile_size.y;

        tilemap.resize(height, std::vector<Sprite2D>(width));

        is_loaded = true;
    }


    void Tilemap2D::Draw(const glm::vec2 pos_offset, const glm::vec4 color) {
        if (!is_loaded) return;

        // TODO calculate parallax effect
        // if (parallax) {}

        glm::vec2 zero_pos = this->position + pos_offset;
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (tilemap[y][x].texture_id != 0) {
                    tilemap[y][x].Draw(
                        zero_pos + glm::vec2(x, y) * (tile_size + tile_gap),
                        this->depth,
                        glm::vec2(0.5f),
                        false, false, color);
                }
            }
        }
    }

    void Tilemap2D::SetTile(int x, int y, const Sprite2D& sprite) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        tilemap[y][x] = sprite;
    }

}