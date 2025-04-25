#pragma once

#include <GLM/glm.hpp>

namespace sk_physic2d {
    namespace pixel_perfect {
        class Body;
    }
}

namespace sk_engine {

    // this class should be inherited   
    class Entity {
        public:
        Entity() = default;
        ~Entity() = default;

        uint32_t tag = 0;
        virtual void onCollision(const sk_physic2d::pixel_perfect::Body& other) {};
        virtual void onTrigger(const sk_physic2d::pixel_perfect::Body& other) {};
        virtual void onSquish() {};
        virtual void onRiding(const sk_physic2d::pixel_perfect::Body& other) {};

    };

}