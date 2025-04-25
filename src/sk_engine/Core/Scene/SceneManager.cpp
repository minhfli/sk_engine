#include <iostream>
#include <thread>

#include "SceneManager.h"
#include "Scene.h"

#include <sk_engine/Window/Input.h>
#include <sk_engine/Common/Error.h>

#include <nlohmann/json.hpp>

namespace sk_engine {
    namespace SceneManager {

        Scene* ActiveScene = nullptr;
        Scene* NewScene = nullptr;

        struct SCENE_LOADER {
            bool able_to_load = true;

            std::thread thread;
            bool thread_done = false;

            //if active scene should be active
            bool okay = false;
        } Scene_loader;

        Scene* GetActiveScene() { return ActiveScene; }

        // scene wont be loaded till the end of game loop only be loaded, undefine behavior  
        void LoadScene(nlohmann::json data) {
            if (NewScene) {
                Error("A scene is loading");
                return;
            }
            NewScene = new Scene();
            NewScene->data = data;
        }
        // actually load new scene here
        void SetLoadScene() {
            if (ActiveScene) {
                ActiveScene->Stop();
                ActiveScene->UnLoad();
            }
            ActiveScene = NewScene;
            if (ActiveScene) {
                ActiveScene->Load();
                ActiveScene->Start();
            }

        }
        void OnSignal(SCENE_STATE_SIGNAL signal) {

        }

        void OnBeginLoop() {
            if (ActiveScene) ActiveScene->OnBeginLoop();
        }
        void OnEndLoop() {
            if (ActiveScene) ActiveScene->OnEndLoop();
            if (NewScene) SetLoadScene();
        }

        void Update() {
            if (ActiveScene) ActiveScene->Update();
        }
        void Draw() {
            if (ActiveScene) ActiveScene->Draw();
        }
        void LateUpdate() {
            if (ActiveScene) ActiveScene->LateUpdate();
        }

    }
}