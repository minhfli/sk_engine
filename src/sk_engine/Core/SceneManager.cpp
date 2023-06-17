#include "SceneManager.h"

#include <thread>

namespace sk_core {

    namespace SceneManager {
        namespace {
            void Update();
            void Draw();
            void LateUpdate();

            std::thread load_thread;
        }

        void LoadScene(BaseScene* scene) {

        }
    }

}