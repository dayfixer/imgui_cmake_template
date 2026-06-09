#pragma once

#ifdef IMGUI_PLATFORM_GLFW

struct GLFWwindow;

/// Load the window position and size from an ini-style file.
/// If the file does not exist or values are invalid, the window keeps its defaults.
void load_window_position(GLFWwindow *window, const char *filename = "window_pos.ini");

/// Save the current window position and size to an ini-style file.
void save_window_position(GLFWwindow *window, const char *filename = "window_pos.ini");

#endif // IMGUI_PLATFORM_GLFW
