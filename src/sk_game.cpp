#include "sk_game.h"
#include <sk_engine/Graphics/Graphics.h>
#include <sk_engine/Window/Input.h>
#include <sk_engine/Core/sk_time.h>
#include <sk_engine/Audio/skAudio.h>

#include <ImGUI/imgui.h>


#include <entt/entt.hpp>

#include <iostream>

#include <game/Box2dTest.h>
/*
    WRITE ALL GAME LOGIC HERE
    all update and draw funtion is called in gameloop function in sk_engine/sk_main.cpp

    update funtion has delta_tick and delta_time(seconds)
    1000 tick = 1 second
    use delta_tick when you need true time(animation and stuff)


*/
namespace sk_game {
    namespace {
        sk_graphic::Camera* cam;

        float camsize = 11.25f;

        bool draw = true;
        sk_graphic::Sprite2D sprite;

        entt::registry ENTT_RES;

        entt::entity e1, e2, e3;

        struct c1 {
            int x, y;
        };
        struct c2 {
            int a, b;
        };
    }

    void Init() {
        cam = sk_graphic::Renderer2D_GetCam();
        cam->ProjectionO(camsize, 1280, 720);
        cam->position = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Start() {
        sk_graphic::Texture2D tex;
        tex.Load("Assets/error.png");
        sprite.LoadTexture(tex, glm::vec2(2));

        b2_test::Setup();
    }
    //! update cam size and positon, temporary
    void UpdateCam() {
        if (sk_input::Key(sk_key::KP_4)) {
            camsize -= 10 * sk_time::delta_time;
            cam->ProjectionO(camsize, 1280, 720);
        }
        if (sk_input::Key(sk_key::KP_6)) {
            camsize += 10 * sk_time::delta_time;
            cam->ProjectionO(camsize, 1280, 720);
        }
        if (sk_input::Key(sk_key::KP_1)) cam->position.x -= 1;
        if (sk_input::Key(sk_key::KP_3)) cam->position.x += 1;
        if (sk_input::Key(sk_key::KP_2)) cam->position.y -= 1;
        if (sk_input::Key(sk_key::KP_5)) cam->position.y += 1;
    }
    //? normal update, call before draw
    void UpdateN() {
        //! update cam size and positon, temporary
        UpdateCam();
        cam->Update();

        b2_test::Update();

    }
    //? late update, call after draw
    void UpdateL() {
    }

    void Draw() {

        //ImGUI draw
        ImGui::Begin("MyWindow");
        ImGui::Checkbox("Boolean property", &draw);
        ImGui::End();


        glm::vec3 mouse_world_pos = cam->Screen_To_World(sk_input::MousePos(), glm::vec2(1280, 720));
        sk_graphic::Renderer2D_AddDotX(mouse_world_pos);

        cam->Draw();

        if (draw)sprite.Draw(glm::vec2(0), 0, glm::vec2(0.5f));

        b2_test::Draw();
    }

    void GameLoop() {
        UpdateN();
        Draw();
        UpdateL();
    }

    //? call when game stop, use to free data, save, ....
    void Stop() {

    }
}