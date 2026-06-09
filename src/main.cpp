#include "context/imgui_context.h"

#ifdef IMGUI_PLATFORM_GLFW
#include "backend/window_position.h"
#include <GLFW/glfw3.h>
#endif

#include <imgui.h>

#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

// -- Constants ---------------------------------------------------------------
static constexpr int DEFAULT_WIDTH = 1280;
static constexpr int DEFAULT_HEIGHT = 720;
static constexpr char WINDOW_TITLE[] = "ImGui CMake Template";

// -- Demo UI state -----------------------------------------------------------
struct AppState {
    bool show_demo_window = true;
    bool show_another_window = false;
    float clear_color[4] = {0.11f, 0.11f, 0.11f, 1.0f};
    int counter = 0;
};

// -- Platform-agnostic UI rendering ------------------------------------------
static void render_ui(AppState &state) {
    // -- Main menu bar ---------------------------------------------------
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Exit")) {
#ifdef IMGUI_PLATFORM_GLFW
                glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
#elif defined(IMGUI_PLATFORM_WIN32)
                ::PostQuitMessage(0);
#endif
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            ImGui::MenuItem("ImGui Demo Window", nullptr, &state.show_demo_window);
            ImGui::MenuItem("Another Window", nullptr, &state.show_another_window);
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    // -- Built-in demo window --------------------------------------------
    if (state.show_demo_window)
        ImGui::ShowDemoWindow(&state.show_demo_window);

    // -- Custom application window ---------------------------------------
    if (state.show_another_window) {
        ImGui::Begin("Another Window", &state.show_another_window);

        ImGui::Text("This is a custom ImGui window.");
        ImGui::Separator();

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);

        if (ImGui::Button("Click me"))
            state.counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", state.counter);

        ImGui::ColorEdit3("clear color", state.clear_color);

        ImGui::End();
    }

    // -- Status overlay --------------------------------------------------
    {
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoDecoration
                                 | ImGuiWindowFlags_AlwaysAutoResize
                                 | ImGuiWindowFlags_NoSavedSettings
                                 | ImGuiWindowFlags_NoFocusOnAppearing
                                 | ImGuiWindowFlags_NoNav
                                 | ImGuiWindowFlags_NoMove;

        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos({
                                    viewport->WorkPos.x + 10,
                                    viewport->WorkPos.y + viewport->WorkSize.y - 10
                                }, ImGuiCond_Always, {0.0f, 1.0f});
        ImGui::SetNextWindowBgAlpha(0.35f);

        if (ImGui::Begin("StatusOverlay", nullptr, flags)) {
            ImGui::Text("FPS: %.0f  |  Right-click menu bar to toggle windows",
                        ImGui::GetIO().Framerate);
        }
        ImGui::End();
    }
}

// -- Application entry point -------------------------------------------------
static int app_main() {
    // ======================================================================
    //  GLFW path
    // ======================================================================
#ifdef IMGUI_PLATFORM_GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(
        DEFAULT_WIDTH, DEFAULT_HEIGHT,
        WINDOW_TITLE,
        nullptr, nullptr
    );
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    load_window_position(window);

    // -- ImGui scope -----------------------------------------------------
    {
        ScopedImGui ctx(window);
        AppState state;

        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            ctx.beginFrame();
            render_ui(state);
            ctx.endFrame(window);
        }
    }

    save_window_position(window);
    glfwDestroyWindow(window);
    glfwTerminate();

#endif // IMGUI_PLATFORM_GLFW

    // ======================================================================
    //  Win32 native path (skeleton)
    // ======================================================================
#ifdef IMGUI_PLATFORM_WIN32
    // TODO: Create a native Win32 window, initialize the selected DX backend,
    //       then run the ImGui event loop.
    //       See imgui_impl_win32.cpp for reference.
#endif

    return EXIT_SUCCESS;
}

// -- OS entry points ---------------------------------------------------------
#ifdef _WIN32
int WINAPI WinMain(_In_ HINSTANCE /*hInst*/,
                   _In_ HINSTANCE /*hPrev*/,
                   _In_ LPSTR /*lpCmdLine*/,
                   _In_ int /*nShowCmd*/) {
    return app_main();
}
#else
int main() {
    return app_main();
}
#endif
