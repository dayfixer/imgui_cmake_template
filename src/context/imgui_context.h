#pragma once

// Platform-neutral window handle — GLFWwindow* or HWND, both cast to/from void*
using NativeWindow = void *;

/// RAII wrapper that owns an ImGui context and its backend bindings.
/// Create one instance after your window is ready; destroy it before the window.
///
/// Named ScopedImGui to avoid collision with ImGui's internal ImGuiContext struct.
class ScopedImGui {
public:
    /// Initialize ImGui with the currently-selected backend.
    /// @param window    Platform window handle (must outlive this context).
    /// @param ini_file  Optional ImGui .ini filename (nullptr = default).
    explicit ScopedImGui(NativeWindow window = nullptr,
                         const char *ini_file = nullptr);

    /// Shut down backends and destroy the ImGui context.
    ~ScopedImGui();

    // Non-copyable, non-movable
    ScopedImGui(const ScopedImGui &) = delete;

    ScopedImGui &operator=(const ScopedImGui &) = delete;

    /// Call at the start of each frame (after processing events).
    void beginFrame();

    /// Call at the end of each frame (renders and swaps buffers).
    void endFrame(NativeWindow window = nullptr);
};
