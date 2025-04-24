#pragma once

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <random>
#include <vector>

#include <GLM/glm.hpp>

#include "Noise.h"
#include <sk_engine/Graphics/Texture2D.h>

namespace sk_graphic {
    class NoiseTexture {
        Noise* noise = nullptr;
        Texture2D texture;

        public:
        NoiseTexture() = default;



    };
}