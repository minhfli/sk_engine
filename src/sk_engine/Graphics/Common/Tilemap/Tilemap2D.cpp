#include "Tilemap2D.h"
#include <sk_engine/Graphics/2D_Renderer.h>

namespace sk_graphic {

    Tilemap2D::Tilemap2D() {}

    Tilemap2D::~Tilemap2D() {
        tilemap.clear();
    }

    void Tilemap2D::Init(
        const int height,
        const int width,
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
        this->tile_gap = tile_gap;
        this->map_size = (tile_size + tile_gap) * glm::vec2(height, width);
        this->position = position - map_size * pivot;
        this->pivot = pivot;
        this->parallax = parallax;
        this->parallax_speed = parallax_speed;



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
                        glm::vec2(0),
                        false, false, color);
                }
            }
        }
        Renderer2D_AddDotX(glm::vec3(this->position, this->depth));

    }

    void Tilemap2D::SetTile(int x, int y, const Sprite2D& sprite) {
        if (x < 0 || x >= width || y < 0 || y >= height) return;
        tilemap[y][x] = sprite;
    }

    glm::vec2 Tilemap2D::GetPosition(const glm::vec2 pivot) const {
        return position + map_size * pivot;
    }
}