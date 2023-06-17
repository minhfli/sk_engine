#pragma once

#include <entt/entt.hpp>

namespace sk_core {

    enum class TRANSITION_MODE {
        NONE,

        FADE,
        SLIDE,
    };

    class BaseScene {

        private:
        bool loaded = false;
        entt::registry reg;

        public:
        struct BASE_DATA {
            TRANSITION_MODE OUT_MODE, IN_MODE;


        } base_data;

        virtual void Load() {}
        virtual void UnLoad() {}

        virtual void Start() {}
        virtual void Stop() {}

        virtual void Update() {}
        virtual void Draw() {}
        virtual void LateUpdate() {}
    };

    namespace SceneManager {
        void LoadScene(BaseScene* Scene);
    }
}