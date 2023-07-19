#pragma once

#include <string>
#include <entt/entt.hpp>

#include <nlohmann/json.hpp>

namespace sk_engine {


    class Scene {
        public:
        nlohmann::json data;

        enum HINT {
            WORLD_INDEX_____,
            WORLD_NAME______,
            START_LEVEL_____,
            BACK_GROUND_____,

            HINT_NONE
        };
        Scene() {}
        virtual ~Scene() {}

        enum STATE {
            NONE,
            LOADING,
            TRANSITION_IN,
            ACTIVE,
            TRANSITION_OUT,
        };
        STATE SceneState = NONE;

        entt::registry EnTT_reg;

        // basic setup of new scene, call before scene create
        virtual void HintStr(HINT type, std::string hint) {}
        virtual void HintFlt(HINT type, float hint) {}
        virtual void HintInt(HINT type, int hint) {}

        // load and unload scene
        virtual void Load() {}
        virtual void UnLoad() {}

        virtual void Start() {}
        virtual void Stop() {}

        // game loop
        virtual void OnBeginLoop() {}
        virtual void OnEndLoop() {}
        virtual void Update() {}
        virtual void Draw() {}
        virtual void LateUpdate() {}
        virtual void FixedUpdate() {}
    };
}