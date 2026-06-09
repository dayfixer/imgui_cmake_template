#include "window_position.h"

#ifdef IMGUI_PLATFORM_GLFW

#include <GLFW/glfw3.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>

void load_window_position(GLFWwindow *window, const char *filename) {
    if (!window || !filename)
        return;

    FILE *file = fopen(filename, "r");
    if (!file)
        return; // first run -- no saved position yet

    int x = 0, y = 0, w = 0, h = 0;
    char line[128];
    while (fgets(line, sizeof(line), file)) {
        // strip trailing newline
        line[strcspn(line, "\r\n")] = '\0';

        if (strncmp(line, "x=", 2) == 0)
            x = atoi(line + 2);
        else if (strncmp(line, "y=", 2) == 0)
            y = atoi(line + 2);
        else if (strncmp(line, "width=", 6) == 0)
            w = atoi(line + 6);
        else if (strncmp(line, "height=", 7) == 0)
            h = atoi(line + 7);
    }
    fclose(file);

    if (w > 0 && h > 0) {
        glfwSetWindowPos(window, x, y);
        glfwSetWindowSize(window, w, h);
    }
}

void save_window_position(GLFWwindow *window, const char *filename) {
    if (!window || !filename)
        return;

    int x = 0, y = 0, w = 0, h = 0;
    glfwGetWindowPos(window, &x, &y);
    glfwGetWindowSize(window, &w, &h);

    // Only save if window is not minimized (negative or zero dimensions)
    if (w <= 0 || h <= 0)
        return;

    FILE *file = fopen(filename, "w");
    if (!file)
        return;

    fprintf(file, "x=%d\n", x);
    fprintf(file, "y=%d\n", y);
    fprintf(file, "width=%d\n", w);
    fprintf(file, "height=%d\n", h);
    fclose(file);
}

#endif // IMGUI_PLATFORM_GLFW
