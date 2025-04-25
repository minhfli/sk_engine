#pragma once

#include <GLM/glm.hpp>


namespace  sk_physic2d {
    namespace pixel_perfect {
        class Entity;

        // physic layer of physic body, a body can exist in multiple layers
        // a body can only interact with other body if there is a layer match
        constexpr uint8_t layer1 = 1;
        constexpr uint8_t layer2 = 1 << 1;
        constexpr uint8_t layer3 = 1 << 2;
        constexpr uint8_t layer4 = 1 << 3;
        constexpr uint8_t layer5 = 1 << 4;
        constexpr uint8_t layer6 = 1 << 5;
        constexpr uint8_t layer7 = 1 << 6;
        constexpr uint8_t layer8 = 1 << 7;

        enum class Body_Type :uint8_t {
            /// @brief static solid, not moveable, not triggerable
            STATIC_SOLID = 0,
            /// @brief dynamic solid, moveable, move actors that collide with it
            MOVING_SOLID = 1,
            /// @brief dynamic actor, moveable, push actors that overlap with it
            ACTOR = 2,
            /// @brief trigger, not moveable, trigger other actors or triggers that overlap with it
            TRIGGER = 3,
        };
        enum class Direction_tag :uint8_t {
            NONE = 0,
            // direction tag
            // solid will only push actor if actor move in the opposite direction
            // trigger will only trigger if actor move in the opposite direction
            // for example, if actor move up, and solid has up tag, it will not collide with actor, 
            // but if actor move down, it will collide with actor
            U = 1, // up
            D = 2, // down
            L = 3, // left
            R = 4, // right
        };

        // some function for checking   
        bool overlap(const glm::ivec4 a, const glm::ivec4 b);
        bool contain(const glm::ivec4 a, const glm::ivec4 b);

        struct rect {

            /// @brief center pos
            glm::vec2 pos = glm::vec2(0);
            /// @brief half size
            glm::vec2 hsize = glm::vec2(1);

            rect(glm::vec2 p = glm::vec2(0), glm::vec2 hs = glm::vec2(1)) : pos(p), hsize(hs) {}
            rect(glm::vec4 bound) {
                pos = (glm::vec2(bound.z, bound.w) + glm::vec2(bound.x, bound.y)) / 2.0f;
                hsize = (glm::vec2(bound.z, bound.w) - glm::vec2(bound.x, bound.y)) / 2.0f;
            }

            inline rect expand(float x, float y) {
                return rect(pos, hsize + glm::vec2(x, y));
            }
            inline glm::vec4 bound(glm::vec2 expand = glm::vec2(0)) const {
                return glm::vec4(pos - hsize - expand, pos + hsize + expand);
            }
            inline bool contain(const rect& r2) const {
                return {
                    r2.pos.x - r2.hsize.x >= this->pos.x - this->hsize.x &&
                    r2.pos.y - r2.hsize.y >= this->pos.y - this->hsize.y &&
                    r2.pos.x + r2.hsize.x <= this->pos.x + this->hsize.x &&
                    r2.pos.y + r2.hsize.y <= this->pos.y + this->hsize.y
                };
            }
            inline bool overlap(const rect& r2) const {
                return{
                    this->pos.x > r2.pos.x - r2.hsize.x - this->hsize.x &&
                    this->pos.y > r2.pos.y - r2.hsize.y - this->hsize.y &&
                    this->pos.x < r2.pos.x + r2.hsize.x + this->hsize.x &&
                    this->pos.y < r2.pos.y + r2.hsize.y + this->hsize.y
                };
            }
        };

        const int INTCOORD_PRECISION = 16;
        const float INTCOORD_ONE_OVER_PRECISION = 1.0f / INTCOORD_PRECISION;
        /// @brief physic rectangle collider in integer coodinate
        struct irect {
            glm::ivec4 bound;
            /// @brief offet of the rect to its true positon in integer coodinate, always smaller than 1, should not be set outside class
            glm::vec2 offset = glm::vec2(0);

            // simple constuctors
            irect() :bound(0) {}
            irect(const int x, const int y, const int z, const int w) : bound(x, y, z, w) {}
            irect(const glm::ivec4 b) : bound(b) {}

            /// @brief advanced contructor, world size, world position   
            static irect irect_fsize_fpos(glm::vec2 hsize, glm::vec2 pos);
            /// @brief advanced contructor, world bound   
            static irect irect_fbound(glm::vec4 bound);
            /// @brief advanced contructor, world bound   
            static irect irect_fray(glm::vec2 pos, glm::vec2 size);

            /// @brief center of rect in integer coordinate, can return false result if rect size is not even
            glm::ivec2 centeri()const;
            /// @brief true center of rect in world coordinate
            glm::vec2 true_center()const;
            /// @brief true bound of rect in world coordinate
            glm::vec4 true_bound(bool add_offset = true) const;

            /// @brief move rect in integer coordinate
            /// @param offset_0 if true, set offset to 0
            void movei(const int x, const int y, const bool offset_0 = 0);
            /// @brief move rect in world(float) coordinate
            void move(const float x, const float y);
            /// @brief set center of the rect in world coordinate
            void set_center(float x, float y);

            void expand(int x, int y);
            void extend(int x, int y);
            /// @brief return new expanded rect 
            irect expand_(int x, int y);
            /// @brief return new extended rect 
            irect extend_(int x, int y);

            bool contain(const irect& r2) const;
            bool overlap(const irect& r2) const;
        };
        struct ray {
            glm::vec2 pos = glm::vec2(0);
            glm::vec2 dir = glm::vec2(0, 1); // default is up

            ray() {};
            ray(const glm::vec2 p = glm::vec2(0), const glm::vec2 d = glm::vec2(0, 1)) : pos(p), dir(d) {}

            inline rect bounding_box() const {
                glm::vec2 p = pos;
                glm::vec2 s = dir;
                if (s.x < 0) {
                    p.x += s.x;
                    s.x = -s.x;
                }
                if (s.y < 0) {
                    p.y += s.y;
                    s.y = -s.y;
                }
                return rect(p, s);
            }
            inline glm::vec2 begin() const { return pos; }
            inline glm::vec2 end() const { return pos + dir; }

            inline ray reverse() const {
                return ray(pos + dir, -dir);
            }
        };

        struct contact {
            bool hit = false;
            glm::vec2 pos;      //? contact point
            glm::vec2 normal;   //? contact normal
            float t_near = 0;
            float t_far = 0;
            //? time till collide (realtime)
        };

        //! may change in future implementation 

        struct Body_Def {
            irect RECT;
            int weight = 1;
            uint8_t layer;
            Body_Type type = Body_Type::STATIC_SOLID;
            Direction_tag direction = Direction_tag::NONE;
            Entity* entity;

            /// @param t collider type, 0:solid, 1:actor, 2:triggerer
            /// @param tg tag
            Body_Def(irect r, int weight = 1, Body_Type t = Body_Type::STATIC_SOLID, Direction_tag dir = Direction_tag::NONE, uint8_t layer = layer1, Entity* e = nullptr) :
                RECT(r),
                weight(weight),
                type(t),
                layer(layer),
                direction(dir),
                entity(e) {
            }
        };
        struct Body {
            public:
            bool is_active = false;
            irect RECT;
            int weight = 1;
            uint8_t layer;
            Body_Type type = Body_Type::STATIC_SOLID;
            Direction_tag direction = Direction_tag::NONE;
            Entity* entity;

            // use for dynamic movement of physic body, should be 0 if use moveamount to move body
            glm::vec2 velocity = glm::vec2(0);
            // use for precise movement of phyisc body, reset every frame, should be 0 if use velocity to move body
            glm::vec2 move_amount = glm::vec2(0);

            Body() {}
            Body(Body_Def def) :
                is_active(true),
                RECT(def.RECT),
                weight(def.weight),
                layer(def.layer),
                type(def.type),
                direction(def.direction),
                entity(def.entity) {
            }
            void set_moveto(glm::vec2 pivot, glm::vec2 target);
        };

    }
}