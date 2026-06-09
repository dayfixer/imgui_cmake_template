# ── ImGui library ────────────────────────────────────────────────────────────
include(../cmake/ImGui.cmake)

# ── Demo executable ──────────────────────────────────────────────────────────
add_executable(Demo WIN32
        backend/window_position.h
        backend/window_position.cpp
        context/imgui_context.h
        context/imgui_context.cpp
        main.cpp
)

target_link_libraries(Demo PRIVATE ImGui::ImGui)
target_include_directories(Demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# ── MSVC: UTF-8 encoding + suppress CRT deprecation warnings ──────────────────
if (MSVC)
    target_compile_options(Demo PRIVATE /utf-8)
    target_compile_definitions(Demo PRIVATE _CRT_SECURE_NO_WARNINGS)
endif ()

# ── Preprocessor defines for the active backend ──────────────────────────────
if (IMGUI_PLATFORM_GLFW)
    target_compile_definitions(Demo PRIVATE IMGUI_PLATFORM_GLFW)
    if (IMGUI_BACKEND_OPENGL3)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_OPENGL3)
    elseif (IMGUI_BACKEND_OPENGL2)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_OPENGL2)
    elseif (IMGUI_BACKEND_VULKAN)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_VULKAN)
    endif ()
elseif (IMGUI_PLATFORM_WIN32)
    target_compile_definitions(Demo PRIVATE IMGUI_PLATFORM_WIN32)
    if (IMGUI_BACKEND_DX11)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_DX11)
    elseif (IMGUI_BACKEND_DX12)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_DX12)
    elseif (IMGUI_BACKEND_DX10)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_DX10)
    elseif (IMGUI_BACKEND_DX9)
        target_compile_definitions(Demo PRIVATE IMGUI_BACKEND_DX9)
    endif ()
endif ()
