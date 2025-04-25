#include <game/sk_game.h>
#include <sk_engine/Graphics/Graphics.h>
#include <sk_engine/Window/Input.h>
#include <sk_engine/Core/sk_time.h>
#include <sk_engine/Audio/skAudio.h>

#include <sk_engine/Graphics/Common/Tilemap/Tilemap2D.h>

#include <ImGUI/imgui.h>

#include <sk_engine/Graphics/Common/Noise/Noise.h>

#include <sk_engine/Physics/PixelPerfect/AABB.h>

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
        float default_camsize = 11.25f;

        sk_physic2d::pixel_perfect::AABB_World physic_world;

        bool draw = true;
        sk_graphic::Sprite2D sprite;
        sk_graphic::Tilemap2D tilemap;
        sk_graphic::Tilemap2D bg_tilemap;

        sk_graphic::Sprite2D noise_sprite[3];
        sk_graphic::Sprite2D noise_sprite2[10];

        int noise_map_size = 256;

        sk_graphic::NoiseMap genPerlinSprite() {
            sk_graphic::ValueNoise noise1(0, 4);
            sk_graphic::ValueNoise noise2(0, 8);
            sk_graphic::ValueNoise noise3(0, 16);
            sk_graphic::ValueNoise noise4(0, 32);
            sk_graphic::NoiseMap noise_map1 = noise1.toNoiseMap(noise_map_size, noise_map_size);
            sk_graphic::NoiseMap noise_map2 = noise2.toNoiseMap(noise_map_size, noise_map_size);
            sk_graphic::NoiseMap noise_map3 = noise3.toNoiseMap(noise_map_size, noise_map_size);
            sk_graphic::NoiseMap noise_map4 = sk_graphic::addNoiseMap(noise_map1, noise_map2);
            sk_graphic::NoiseMap noise_map5 = sk_graphic::addNoiseMap(noise_map3, noise_map4);
            sk_graphic::Texture2D tex1 = noise_map1.toTexture();
            sk_graphic::Texture2D tex2 = noise_map2.toTexture();
            sk_graphic::Texture2D tex3 = noise_map3.toTexture();
            sk_graphic::Texture2D tex4 = noise_map4.toTexture();
            sk_graphic::Texture2D tex5 = noise_map5.toTexture();
            noise_sprite2[0].LoadTexture(tex1, glm::vec2(10));
            noise_sprite2[1].LoadTexture(tex2, glm::vec2(10));
            noise_sprite2[2].LoadTexture(tex3, glm::vec2(10));
            noise_sprite2[3].LoadTexture(tex4, glm::vec2(10));
            noise_sprite2[4].LoadTexture(tex5, glm::vec2(10));
            sk_graphic::NoiseMap noise_map6 = noise_map1.filterBlackWhite(128, 256);
            sk_graphic::NoiseMap noise_map7 = noise_map2.filterBlackWhite(128, 256);
            sk_graphic::NoiseMap noise_map8 = noise_map3.filterBlackWhite(128, 256);
            sk_graphic::NoiseMap noise_map9 = noise_map4.filterBlackWhite(128, 256, true);
            sk_graphic::NoiseMap noise_map10 = noise_map5.filterBlackWhite(128, 256, true);
            sk_graphic::Texture2D tex6 = noise_map6.toTexture();
            sk_graphic::Texture2D tex7 = noise_map7.toTexture();
            sk_graphic::Texture2D tex8 = noise_map8.toTexture();
            sk_graphic::Texture2D tex9 = noise_map9.toTexture();
            sk_graphic::Texture2D tex10 = noise_map10.toTexture();
            noise_sprite2[5].LoadTexture(tex6, glm::vec2(10));
            noise_sprite2[6].LoadTexture(tex7, glm::vec2(10));
            noise_sprite2[7].LoadTexture(tex8, glm::vec2(10));
            noise_sprite2[8].LoadTexture(tex9, glm::vec2(10));
            noise_sprite2[9].LoadTexture(tex10, glm::vec2(10));

            return noise_map1;
        }
    }

    void Init() {
        cam = sk_graphic::Renderer2D_GetCam();
        cam->ProjectionO(camsize, 1280, 720);
        cam->position = glm::vec3(0.0f, 0.0f, 0.0f);

        physic_world.Hint_WorldBound(glm::vec4(-256, -256, 256, 256));
        physic_world.Init();
        physic_world.enable_debug_draw = true;
    }

    void Start() {
        sk_graphic::Texture2D tex;

        sk_graphic::RandomNoise R_noise(0, 8);
        sk_graphic::ValueNoise V_noise(0, 8);
        sk_graphic::PerlinNoise P_noise(0, 8);
        tex = R_noise.genTexture(noise_map_size, noise_map_size);
        noise_sprite[0].LoadTexture(tex, glm::vec2(10));
        tex = V_noise.genTexture(noise_map_size, noise_map_size);
        noise_sprite[1].LoadTexture(tex, glm::vec2(10));
        tex = P_noise.genTexture(noise_map_size, noise_map_size);
        noise_sprite[2].LoadTexture(tex, glm::vec2(10));

        sk_graphic::NoiseMap selected_noise_map = genPerlinSprite();

        tex.Load("Assets/error.png");
        sprite.LoadTexture(tex, glm::vec2(1));


        Bitset2D bitset = selected_noise_map.filter(128, 256, true);
        Bitset2D bitset2 = selected_noise_map.filter(64, 256, true);

        tilemap.Init(noise_map_size, noise_map_size, 0, glm::vec2(1), glm::vec2(0), glm::vec2(0), glm::vec2(0.5f));
        for (int i = 0; i < noise_map_size; i++)
            for (int j = 0; j < noise_map_size; j++)
                if (bitset(i, j)) {
                    tilemap.SetTile(i, j, sprite);
                    sk_physic2d::pixel_perfect::irect rect =
                        sk_physic2d::pixel_perfect::irect::irect_fbound(
                            glm::vec4(i, j, i + 1, j + 1)
                            + glm::vec4(tilemap.GetPosition(glm::vec2(0)), tilemap.GetPosition(glm::vec2(0)))
                        );

                    sk_physic2d::pixel_perfect::Body_Def def(rect);
                    physic_world.Create_Body(def);
                }
        bg_tilemap.Init(noise_map_size, noise_map_size, -1, glm::vec2(1), glm::vec2(0), glm::vec2(0), glm::vec2(0.5f));
        for (int i = 0; i < noise_map_size; i++)
            for (int j = 0; j < noise_map_size; j++)
                if (bitset2(i, j))
                    bg_tilemap.SetTile(i, j, sprite);
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

        if (sk_input::Key(sk_key::KP_7)) {
            cam->ProjectionO(default_camsize, 1280, 720);
        }
    }

    //? normal update, call before draw
    void Update() {
        //! update cam size and positon, temporary
        UpdateCam();
        cam->Update();
        physic_world.Update();
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
        bg_tilemap.Draw(glm::vec2(0), glm::vec4(glm::vec3(0.5f), 1.0f));

        // for (int i = 0; i < 3; i++)
        //     noise_sprite[i].Draw(glm::vec2(-0.5 + i * 10, -0.5), 1, glm::vec2(0.5f));

        for (int i = 0; i < 10; i++)
            noise_sprite2[i].Draw(glm::vec2(-0.5 + i * 10, 10.5), 1, glm::vec2(0.5f));
        physic_world.Draw();

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