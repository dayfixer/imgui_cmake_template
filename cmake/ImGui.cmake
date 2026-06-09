include_guard(GLOBAL)

# ── Validate that imgui_SOURCE_DIR is set ────────────────────────────────────
if (NOT DEFINED imgui_SOURCE_DIR)
    message(FATAL_ERROR "imgui_SOURCE_DIR is not defined. "
                        "CMakeLists.txt must resolve ImGui sources before including ImGui.cmake")
endif ()

# ── Handle vendored GLFW ─────────────────────────────────────────────────────
if (IMGUI_PLATFORM_GLFW AND NOT TARGET glfw)
    if (EXISTS ${PROJECT_SOURCE_DIR}/vendor/glfw)
        add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glfw ${CMAKE_BINARY_DIR}/glfw)
    else ()
        message(FATAL_ERROR "GLFW backend selected but glfw target not found. "
                            "Place GLFW in vendor/glfw or ensure CMakeLists.txt fetches it.")
    endif ()
endif ()

# ── ImGui core sources ───────────────────────────────────────────────────────
set(IMGUI_CORE_SOURCES
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
)

# ── Backend sources by configuration ─────────────────────────────────────────
set(IMGUI_BACKEND_SOURCES "")

if (IMGUI_PLATFORM_GLFW)
    list(APPEND IMGUI_BACKEND_SOURCES
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    )
    if (IMGUI_BACKEND_OPENGL3)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
        )
    elseif (IMGUI_BACKEND_OPENGL2)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl2.cpp
        )
    elseif (IMGUI_BACKEND_VULKAN)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_vulkan.cpp
        )
    endif ()
elseif (IMGUI_PLATFORM_WIN32)
    list(APPEND IMGUI_BACKEND_SOURCES
        ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    )
    if (IMGUI_BACKEND_DX11)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
        )
    elseif (IMGUI_BACKEND_DX12)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp
        )
    elseif (IMGUI_BACKEND_DX10)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_dx10.cpp
        )
    elseif (IMGUI_BACKEND_DX9)
        list(APPEND IMGUI_BACKEND_SOURCES
            ${imgui_SOURCE_DIR}/backends/imgui_impl_dx9.cpp
        )
    endif ()
endif ()

# ── Build ImGui library ──────────────────────────────────────────────────────
add_library(ImGui STATIC
    ${IMGUI_CORE_SOURCES}
    ${IMGUI_BACKEND_SOURCES}
)

target_include_directories(ImGui
    PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# Platform-specific link dependencies
if (IMGUI_PLATFORM_GLFW)
    target_link_libraries(ImGui PUBLIC glfw)
    find_package(OpenGL REQUIRED)
    target_link_libraries(ImGui PUBLIC OpenGL::GL)
elseif (IMGUI_PLATFORM_WIN32)
    if (IMGUI_BACKEND_DX9)
        target_link_libraries(ImGui PUBLIC d3d9)
    elseif (IMGUI_BACKEND_DX10)
        target_link_libraries(ImGui PUBLIC d3d10 d3dcompiler)
    elseif (IMGUI_BACKEND_DX11)
        target_link_libraries(ImGui PUBLIC d3d11 d3dcompiler)
    elseif (IMGUI_BACKEND_DX12)
        target_link_libraries(ImGui PUBLIC d3d12 d3dcompiler dxgi)
    endif ()
endif ()

# ── Alias target for clean consumption ───────────────────────────────────────
add_library(ImGui::ImGui ALIAS ImGui)
