cmake_minimum_required(VERSION 3.10)

# ImGui is a source-only library without a buildable root CMakeLists.txt;
# FetchContent_Populate is the correct API for this pattern.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0")
    cmake_policy(SET CMP0169 OLD)
endif ()

project(Demo)

include(cmake/helper_functions.cmake)

# ── Platform options (mutually exclusive) ─────────────────────────────────────
# Declare both options; resolve_mutex_option persists the choice into the cache
# so that -DIMGUI_PLATFORM_GLFW=ON automatically forces Win32 OFF and vice versa.
option(IMGUI_PLATFORM_WIN32 "Use Win32 backend" OFF)
option(IMGUI_PLATFORM_GLFW  "Use GLFW backend"  ON)
resolve_mutex_option(CACHE IMGUI_PLATFORM_WIN32 IMGUI_PLATFORM_GLFW)

# ── Win32 renderer backends (mutually exclusive) ─────────────────────────────
if (IMGUI_PLATFORM_WIN32)
    option(IMGUI_BACKEND_DX9  "[ImGui] Backend by [Direct3D9]"  ON)
    option(IMGUI_BACKEND_DX10 "[ImGui] Backend by [Direct3D10]" OFF)
    option(IMGUI_BACKEND_DX11 "[ImGui] Backend by [Direct3D11]" OFF)
    option(IMGUI_BACKEND_DX12 "[ImGui] Backend by [Direct3D12]" OFF)
    resolve_mutex_option(CACHE IMGUI_BACKEND_DX11 IMGUI_BACKEND_DX12 IMGUI_BACKEND_DX10 IMGUI_BACKEND_DX9)

# ── GLFW renderer backends (mutually exclusive) ──────────────────────────────
elseif (IMGUI_PLATFORM_GLFW)
    option(IMGUI_BACKEND_OPENGL2 "[ImGui] Backend by [OpenGL2]" ON)
    option(IMGUI_BACKEND_OPENGL3 "[ImGui] Backend by [OpenGL3]" OFF)
    option(IMGUI_BACKEND_VULKAN  "[ImGui] Backend by [Vulkan]"  OFF)
    resolve_mutex_option(CACHE IMGUI_BACKEND_OPENGL3 IMGUI_BACKEND_VULKAN IMGUI_BACKEND_OPENGL2)
endif ()

# ── ImGui source resolution ──────────────────────────────────────────────────
# Prefer a vendored copy; otherwise fetch from GitHub via FetchContent.
if (NOT EXISTS ${PROJECT_SOURCE_DIR}/vendor/imgui)
    include(FetchContent)

    message(STATUS "Fetching imgui....")
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG origin/docking
            GIT_SHALLOW TRUE
            SYSTEM
    )
    FetchContent_GetProperties(imgui)
    if (NOT imgui_POPULATED)
        FetchContent_Populate(imgui)
    endif ()
    message(STATUS "Fetched imgui^^^")

    if (IMGUI_PLATFORM_GLFW AND NOT EXISTS ${PROJECT_SOURCE_DIR}/vendor/glfw)
        message(STATUS "Fetching glfw....")
        FetchContent_Declare(
                glfw
                GIT_REPOSITORY https://github.com/glfw/glfw.git
                GIT_TAG 3.4
                GIT_SHALLOW TRUE
                SYSTEM
        )
        set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
        set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(glfw)
        message(STATUS "Fetched glfw^^^")
    endif ()
else ()
    set(imgui_SOURCE_DIR ${PROJECT_SOURCE_DIR}/vendor/imgui)
endif ()

message(STATUS "[Platform]:")
message(STATUS "   Win32? ${IMGUI_PLATFORM_WIN32}")
if(${IMGUI_PLATFORM_WIN32})
    message(STATUS IMGUI_BACKEND_DX9: ${IMGUI_BACKEND_DX9})
    message(STATUS IMGUI_BACKEND_DX10: ${IMGUI_BACKEND_DX10})
    message(STATUS IMGUI_BACKEND_DX11: ${IMGUI_BACKEND_DX11})
    message(STATUS IMGUI_BACKEND_DX12: ${IMGUI_BACKEND_DX12})
endif ()
message(STATUS "   GLFW ? ${IMGUI_PLATFORM_GLFW}")

add_subdirectory(src)