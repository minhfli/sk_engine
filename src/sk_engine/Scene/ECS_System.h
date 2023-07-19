#pragma once

namespace sk_engine {
    class ECS_System {


        virtual void OnBeginFrame() {}
        virtual void OnEndFrame() {}

        virtual void Update() {}
        virtual void Draw() {}
        virtual void LateUpdate() {}
        virtual void FixedUpdate() {}

    };

}