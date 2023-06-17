#include <iostream>

#include <SDL2/SDL.h>
#include <GLAD/glad.h>

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_sdl2.h>
#include <ImGUI/imgui_impl_opengl3.h>

#include <Common/Error.h>

#include "skWindow.h"
#include "Input.h"

static SDL_Window* window = nullptr;
static bool window_should_close;

namespace sk_window {
    void Init(const std::string name, const int width, const int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL video system could not be initialized: " << SDL_GetError();
            FatalError("Stop");
        }
        else
            std::cout << "SDL video system is ready to go\n";

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

        window = SDL_CreateWindow("C++ SDL2 Window",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN |
            SDL_WINDOW_OPENGL
        );

        SDL_GLContext main_glcontext = SDL_GL_CreateContext(window);
        std::cout << "GL_context created\n";

        gladLoadGLLoader(SDL_GL_GetProcAddress);

        glViewport(0, 0, width, height);

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        // window is the SDL_Window*
        // context is the SDL_GLContext
        ImGui_ImplSDL2_InitForOpenGL(window, main_glcontext);
        ImGui_ImplOpenGL3_Init();

    }
    void ShutDown() {
        // ImGui Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_DestroyWindow(window);
    }

    void Process_event() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            ImGui_ImplSDL2_ProcessEvent(&event);
            switch (event.type) {
                case SDL_QUIT:
                    window_should_close = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        window_should_close = true;
                    break;

                default:
                    break;
            }
        }
        //process input 
        sk_input::Process();
    }
    bool Should_close() {
        return window_should_close;
    }

    void BeginFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void EndFrame() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }
    void Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    void Swapbuffer() {
        SDL_GL_SwapWindow(window);
    }

    void sk_keystate_get();
    void sk_keystate_process();
}