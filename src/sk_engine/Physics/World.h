#pragma once

#include <box2d/box2d.h>

namespace sk_physic2d {
    class World {
        private:
        b2World b2_world;


        public:
        void Init();


    };
}