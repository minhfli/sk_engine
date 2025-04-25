#pragma once

#include <GLM/glm.hpp>

#include "Collider.h"

namespace sk_physic2d {
    namespace pixel_perfect {

    // this class should be inherited   
        class Entity {
            public:
            Entity() = default;
            ~Entity() = default;

            virtual void onCollision(const Body& other) {}
            virtual void onTrigger(const Body& other) {}
            virtual void onSquish() {}
            virtual void onRiding(const Body& other) {}

        };
    }
}