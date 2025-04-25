#include "Player.h"
#include <sk_engine/Physics/PixelPerfect/AABB.h>
#include <sk_engine/Window/Input.h>

void Player::Init(sk_physic2d::pixel_perfect::AABB_World* physic_world, sk_graphic::Camera* cam) {
    this->physic_world = physic_world;
    this->cam = cam;
}

void Player::Start() {
    sk_physic2d::pixel_perfect::Body_Def def(sk_physic2d::pixel_perfect::irect::irect_fbound(glm::vec4(-0.5, -0.5, 0.5, 0.5)));
    def.entity = this;
    def.layer = sk_physic2d::pixel_perfect::layer1;
    def.type = sk_physic2d::pixel_perfect::Body_Type::ACTOR;
    def.weight = 1;
    def.direction = sk_physic2d::pixel_perfect::Direction_tag::NONE;

    int id = physic_world->Create_Body(def);
    body = physic_world->Get_Body(id);
}

void Player::Update() {
    if (sk_input::Key(sk_key::W)) body->velocity.y = 1;
    else if (sk_input::Key(sk_key::S)) body->velocity.y = -1;
    else body->velocity.y = 0;

    if (sk_input::Key(sk_key::A)) body->velocity.x = -1;
    else if (sk_input::Key(sk_key::D)) body->velocity.x = 1;
    else body->velocity.x = 0;

    cam->focus.pos = body->RECT.true_center();
}
void Player::UpdateFixed() {
}
void Player::UpdateLate() {
}
void Player::Draw() {
    sk_graphic::Renderer2D_AddBBox(body->RECT.true_bound(), 2, { 1,0,0,1 });
}
void Player::Quit() {
}