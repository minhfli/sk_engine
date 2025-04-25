#pragma once

#include "Scene.h"

namespace sk_engine {
    enum class SCENE {
        NONE,

        SPASH_SCENE,

        MAIN_MENU,

        OPTION_MENU,

        GAMESCENE
    };
    namespace SceneManager {

        Scene* GetActiveScene();
        int GetCurrentSceneType();

        //scene wont be loaded till the end of game loop, undefine behavior if called multiple time, each scene is indepentdant, cant pass data between scene
        void LoadScene(SCENE toload);

        // call at the begining and ending of a gameloop
        void OnBeginLoop();
        void OnEndLoop();

        void Update();
        void Draw();
        void LateUpdate();

        // function for scene to call
        enum SCENE_STATE_SIGNAL {

            TRANSITION_IN_BEGIN,
            TRANSITION_IN_END,
            TRANSITION_OUT_BEGIN,
            TRANSITION_OUT_END,
            LOADING_BEGIN,
            LOADING_END,
        };
        //signal should only be send when endloop
        void OnSignal(SCENE_STATE_SIGNAL signal);

    }
}