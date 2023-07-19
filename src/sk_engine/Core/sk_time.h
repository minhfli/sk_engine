#pragma once

struct sk_time {
    static unsigned int delta_tick;
    static unsigned int current_tick;
    static unsigned int current_real_tick;

    static float delta_time;
    static float current_time;
    static float current_real_time;

    static constexpr unsigned int fixed_delta_tick = 17;
    static constexpr float fixed_delta_time = (float)fixed_delta_tick / 1000;
};
