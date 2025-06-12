#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <SDL.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <SDL_opengles2.h>
#else
#include <SDL_opengl.h>
#endif

#include "chip8.hh"
#include "file_browser.hh"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

template <typename T>
struct Result {
    T value;
    //Error err;
}; // simple and rust like...

void readRomFile(std::string const &filename, std::string const &mime_type, std::string_view buffer, void*);

class Application {
public:
    Application() {
        initSDL();
        createWindow();
        createGLContext();
        setupImGui();
    }

    virtual ~Application() {
    
    }

    friend void main_loop();
    friend void readRomFile(std::string const &filename, std::string const &mime_type, std::string_view buffer, void*);
private:
    Chip8 machine;
    SDL_Window* window;
    SDL_GLContext glContext;

    void initSDL() {
        auto result = SDL_Init(SDL_INIT_VIDEO);

        if(result != 0) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    }

    void createWindow() {
        window = SDL_CreateWindow(
            "Hachi", 
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            640, 480, 
            SDL_WINDOW_OPENGL
        );

        if(window == nullptr) {
            std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
            exit(1);
        }
    }

    void createGLContext() {
        glContext = SDL_GL_CreateContext(this->window);

        if(glContext == nullptr) {
            std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
            exit(1);
        }

        SDL_GL_MakeCurrent(this->window, this->glContext);
        SDL_GL_SetSwapInterval(1);
    }
    
    void setupImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void) io;
        ImGui::StyleColorsDark(); // Optional: Dark theme

        ImGui_ImplSDL2_InitForOpenGL(window, glContext);
        ImGui_ImplOpenGL3_Init("#version 300 es"); // WebGL2
    }

    void runFrame() {
        SDL_Event event;

        while(SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);

            if(event.type == SDL_QUIT) {
                exit(0);
            }
        }

        // ImGui frame logic
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if(ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Open ROM")) { 
                    emscripten_browser_file::upload(".ch8,.chip8,.rom,.pdf", readRomFile);
                    std::cout << "Open ROM\n";
                }
                if (ImGui::MenuItem("Exit")) std::cout << "Exit\n";
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Emulation")) {
                ImGui::MenuItem("Start");
                ImGui::MenuItem("Pause");
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Debug")) {
                ImGui::MenuItem("Step Instruction");
                ImGui::MenuItem("Reset");
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        // Render
        ImGui::Render();
        glViewport(0, 0, 640, 480);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

} app;

void readRomFile(std::string const &filename, std::string const &mime_type, std::string_view buffer, void*) {
    std::cout<<"Read ROM file: " << filename << std::endl;
}

void main_loop() {
    app.runFrame();
}


int main(int argc, char** argv) {
    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    main_loop();
    #endif

    return 0;
}
