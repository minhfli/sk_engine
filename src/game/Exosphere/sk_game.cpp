#include <game/sk_game.h>
#include <sk_engine/Graphics/Graphics.h>
#include <sk_engine/Window/Input.h>
#include <sk_engine/Core/sk_time.h>
#include <sk_engine/Audio/skAudio.h>

#include <sk_engine/Graphics/Common/Tilemap/Tilemap2D.h>

#include <ImGUI/imgui.h>

#include <sk_engine/Graphics/Common/Noise/Noise.h>


#include <iostream>

/*
    WRITE ALL GAME LOGIC HERE
    all update and draw funtion is called in gameloop function in sk_engine/sk_main.cpp

    update funtion has delta_tick and delta_time(seconds)
    1000 tick = 1 second
    use delta_tick when you need true time(animation and stuff)


*/
namespace sk_game {
    namespace { //? local variables, only visible in this file
        sk_graphic::Camera* cam;

        float camsize = 11.25f;

        bool draw = true;
        sk_graphic::Sprite2D sprite;
        sk_graphic::Tilemap2D tilemap;

        sk_graphic::Sprite2D noise_sprite[3];
    }

    void Init() {
        cam = sk_graphic::Renderer2D_GetCam();
        cam->ProjectionO(camsize, 1280, 720);
        cam->position = glm::vec3(0.0f, 0.0f, 0.0f);

    }

    void Start() {
        sk_graphic::Texture2D tex;
        tex.Load("Assets/error.png");
        sprite.LoadTexture(tex, glm::vec2(1));

        tilemap.Init(100, 100, 0, glm::vec2(1), glm::vec2(1), glm::vec2(2), glm::vec2(0));
        for (int i = 0; i < 100; i++)
            for (int j = 0; j < 100; j++)
                tilemap.SetTile(i, j, sprite);


        sk_graphic::RandomNoise R_noise(0, 8);
        sk_graphic::ValueNoise V_noise(0, 8);
        sk_graphic::PerlinNoise P_noise(0, 8);
        tex = R_noise.genTexture(160, 160);
        noise_sprite[0].LoadTexture(tex, glm::vec2(10));
        tex = V_noise.genTexture(160, 160);
        noise_sprite[1].LoadTexture(tex, glm::vec2(10));
        tex = P_noise.genTexture(160, 160);
        noise_sprite[2].LoadTexture(tex, glm::vec2(10));

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
        cam->focus.pos = cam->position;
    }
    //? normal update, call before draw
    void Update() {
        //! update cam size and positon, temporary
        UpdateCam();
        cam->Update();

    }
    //? fixed update, call before draw
    //? use for physics and stuff
    //? delta_tick is 1000 tick = 1 second
    //? delta_time is in seconds
    //! for future implementation, currently not used
    void UpdateFixed() {

    }

    //? late update, call after draw
    void UpdateLate() {
    }

    void Draw() {

        glm::vec3 mouse_world_pos = cam->Screen_To_World(sk_input::MousePos(), glm::vec2(1280, 720));
        sk_graphic::Renderer2D_AddDotX(mouse_world_pos);

        cam->Draw();

        tilemap.Draw(glm::vec2(0), glm::vec4(1));

        noise_sprite[0].Draw(glm::vec2(-0.5, -0.5), 1, glm::vec2(0.5f));
        noise_sprite[1].Draw(glm::vec2(10.5, -0.5), 1, glm::vec2(0.5f));
        noise_sprite[2].Draw(glm::vec2(21.5, -0.5), 1, glm::vec2(0.5f));

    }

    //? call when game stop, use to free data, save, ....
    void Stop() {

    }

    //? GameLoop logic, please do not change if you don't know what you are doing
    void GameLoop() {
        Update();
        Draw();
        UpdateLate();
    }

}