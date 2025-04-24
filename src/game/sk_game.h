#pragma once
/*
    CHANGE GAME HARDCOED SETTINGS HERE

    ONLY ADD OR CHANGE FUNTION IF YOU KNOW WHAT YOU ARE DOING
*/

namespace sk_game {
    const int default_screen_width = 800;

    const int fixed_delta_tick = 50;

    void Init();

    void Start();

    void GameLoop();
    void OnBeginLoop();
    void OnEndLoop();

    //? Normal update, called before draw
    void Update();
    //? Fixed update, called every fixed time step, currently not implemented
    void UpdateFixed();
    //? Late update, called after draw
    void UpdateLate();

    //? Draw function, called every frame
    void Draw();



    void Stop();
}