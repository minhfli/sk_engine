#pragma once

#include <sk_engine/Core/Scene/Entity.h>
#include <sk_engine/Physics/PixelPerfect/AABB.h>
#include <sk_engine/Graphics/Graphics.h>
#include <GLM/glm.hpp>

class Player : public sk_engine::Entity {
    private:
        /* data */
    public:
    Player() {}
    ~Player() {}

    sk_physic2d::pixel_perfect::AABB_World* physic_world;
    sk_graphic::Camera* cam;

    sk_physic2d::pixel_perfect::Body* body;

    void Init(sk_physic2d::pixel_perfect::AABB_World* physic_world, sk_graphic::Camera* cam);

    void Start();

    void Update();
    void UpdateFixed();
    void UpdateLate();
    void Draw();

    void Quit();
};
