#include "imgui_context.h"

#include <imgui.h>

#ifdef IMGUI_PLATFORM_GLFW
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>
#ifdef IMGUI_BACKEND_OPENGL3
#include <imgui_impl_opengl3.h>
#elif defined(IMGUI_BACKEND_OPENGL2)
#include <imgui_impl_opengl2.h>
#elif defined(IMGUI_BACKEND_VULKAN)
#include <imgui_impl_vulkan.h>
#endif
#elif defined(IMGUI_PLATFORM_WIN32)
#include <imgui_impl_win32.h>
#ifdef IMGUI_BACKEND_DX11
#include <imgui_impl_dx11.h>
#elif defined(IMGUI_BACKEND_DX12)
#include <imgui_impl_dx12.h>
#elif defined(IMGUI_BACKEND_DX10)
#include <imgui_impl_dx10.h>
#elif defined(IMGUI_BACKEND_DX9)
#include <imgui_impl_dx9.h>
#endif
#endif

ScopedImGui::ScopedImGui(NativeWindow window, const char *ini_file) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    if (ini_file)
        io.IniFilename = ini_file;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Default style
    ImGui::StyleColorsDark();

#ifdef IMGUI_PLATFORM_GLFW
    ImGui_ImplGlfw_InitForOther(static_cast<GLFWwindow *>(window), true);

#ifdef IMGUI_BACKEND_OPENGL3
    ImGui_ImplOpenGL3_Init();
#elif defined(IMGUI_BACKEND_OPENGL2)
    ImGui_ImplOpenGL2_Init();
#elif defined(IMGUI_BACKEND_VULKAN)
    ImGui_ImplVulkan_Init(/* user must provide Vulkan init info */);
#endif

#elif defined(IMGUI_PLATFORM_WIN32)
    ImGui_ImplWin32_Init(window); // NativeWindow is HWND on Win32

#ifdef IMGUI_BACKEND_DX11
    ImGui_ImplDX11_Init(nullptr, nullptr); // TODO: pass ID3D11Device* and ID3D11DeviceContext*
#elif defined(IMGUI_BACKEND_DX12)
    {
        ImGui_ImplDX12_InitInfo init_info{};
        ImGui_ImplDX12_Init(&init_info);   // TODO: fill ImGui_ImplDX12_InitInfo
    }
#elif defined(IMGUI_BACKEND_DX10)
    ImGui_ImplDX10_Init(nullptr);          // TODO: pass ID3D10Device*
#elif defined(IMGUI_BACKEND_DX9)
    ImGui_ImplDX9_Init(nullptr);           // TODO: pass IDirect3DDevice9*
#endif
#endif
}

ScopedImGui::~ScopedImGui() {
#ifdef IMGUI_PLATFORM_GLFW
#ifdef IMGUI_BACKEND_OPENGL3
    ImGui_ImplOpenGL3_Shutdown();
#elif defined(IMGUI_BACKEND_OPENGL2)
    ImGui_ImplOpenGL2_Shutdown();
#elif defined(IMGUI_BACKEND_VULKAN)
    ImGui_ImplVulkan_Shutdown();
#endif
    ImGui_ImplGlfw_Shutdown();
#elif defined(IMGUI_PLATFORM_WIN32)
#ifdef IMGUI_BACKEND_DX11
    ImGui_ImplDX11_Shutdown();
#elif defined(IMGUI_BACKEND_DX12)
    ImGui_ImplDX12_Shutdown();
#elif defined(IMGUI_BACKEND_DX10)
    ImGui_ImplDX10_Shutdown();
#elif defined(IMGUI_BACKEND_DX9)
    ImGui_ImplDX9_Shutdown();
#endif
    ImGui_ImplWin32_Shutdown();
#endif

    ImGui::DestroyContext();
}

void ScopedImGui::beginFrame() {
#ifdef IMGUI_PLATFORM_GLFW
#ifdef IMGUI_BACKEND_OPENGL3
    ImGui_ImplOpenGL3_NewFrame();
#elif defined(IMGUI_BACKEND_OPENGL2)
    ImGui_ImplOpenGL2_NewFrame();
#elif defined(IMGUI_BACKEND_VULKAN)
    ImGui_ImplVulkan_NewFrame();
#endif
    ImGui_ImplGlfw_NewFrame();
#elif defined(IMGUI_PLATFORM_WIN32)
    ImGui_ImplWin32_NewFrame();
#ifdef IMGUI_BACKEND_DX11
    ImGui_ImplDX11_NewFrame();
#elif defined(IMGUI_BACKEND_DX12)
    ImGui_ImplDX12_NewFrame();
#elif defined(IMGUI_BACKEND_DX10)
    ImGui_ImplDX10_NewFrame();
#elif defined(IMGUI_BACKEND_DX9)
    ImGui_ImplDX9_NewFrame();
#endif
#endif

    ImGui::NewFrame();
}

void ScopedImGui::endFrame(NativeWindow window) {
    ImGui::Render();

#ifdef IMGUI_PLATFORM_GLFW
    auto *glfw_window = static_cast<GLFWwindow *>(window);

    int display_w = 0, display_h = 0;
    glfwGetFramebufferSize(glfw_window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.11f, 0.11f, 0.11f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

#ifdef IMGUI_BACKEND_OPENGL3
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#elif defined(IMGUI_BACKEND_OPENGL2)
    ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
#endif

    glfwSwapBuffers(glfw_window);

#elif defined(IMGUI_PLATFORM_WIN32)
#ifdef IMGUI_BACKEND_DX11
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#elif defined(IMGUI_BACKEND_DX12)
    ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData());
#elif defined(IMGUI_BACKEND_DX10)
    ImGui_ImplDX10_RenderDrawData(ImGui::GetDrawData());
#elif defined(IMGUI_BACKEND_DX9)
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
#endif
    // Swap chain present is the application's responsibility.
#endif
}
