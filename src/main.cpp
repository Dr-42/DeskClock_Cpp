#include <GL/glew.h>
#ifdef _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#ifdef __linux__
#define GLFW_EXPOSE_NATIVE_X11
#include <X11/Xlib.h>
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "game.h"
#include "utils/resource_manager.h"

#include <iostream>
#include <filesystem>
#include <fstream>

// GLFW function declarations
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void hide_taskbar_icon(GLFWwindow* window);

// The Width of the screen
unsigned int SCREEN_WIDTH = 450;
// The height of the screen
unsigned int SCREEN_HEIGHT = 450;

Game App(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char* argv[]) {
    #ifdef _WIN64
    std::string configPath = std::string(getenv("APPDATA")) + "\\DeskClock_Cpp\\" + "config.txt";
    #endif
    #ifdef __linux__
    std::string configPath = std::string(getenv("HOME")) + "/.config/DeskClock_Cpp/" + "config.txt";
    #endif

    std::string config_data;

    int width = -1;
    int height = -1;
    int x = -1;
    int y = -1;
    
    // Load config file data if it exists
    if (std::filesystem::exists(configPath)) {
        std::ifstream config_file(configPath);
        std::string line;
        while (std::getline(config_file, line)) {
            config_data += line;
        }
        config_file.close();

        // Parse config file data
        // Value are in the form of "key=value"
        // Values are separated by a \n or \r\n
        // Valid keys are:
        //     "width" - width of the window
        //     "height" - height of the window
        //     "x" - x position of the window
        //     "y" - y position of the window

        std::string width_str = config_data.substr(config_data.find("width=") + 6, config_data.find("\n") - (config_data.find("width=") + 6));
        std::string height_str = config_data.substr(config_data.find("height=") + 7, config_data.find("\n", config_data.find("height=")) - (config_data.find("height=") + 7));
        std::string x_str = config_data.substr(config_data.find("x=") + 2, config_data.find("\n", config_data.find("x=")) - (config_data.find("x=") + 2));
        std::string y_str = config_data.substr(config_data.find("y=") + 2, config_data.find("\n", config_data.find("y=")) - (config_data.find("y=") + 2));

        width = std::stoi(width_str);
        height = std::stoi(height_str);
        x = std::stoi(x_str);
        y = std::stoi(y_str);
    } else {
        // Create config file if it doesn't exist

        // Create directory if it doesn't exist
        std::string dirPath = "";
        #ifdef _WIN64
        dirPath = std::string(getenv("APPDATA")) + "\\DeskClock_Cpp\\";
        #endif
        #ifdef __linux__
        dirPath = std::string(getenv("HOME")) + "/.config/DeskClock_Cpp/";
        #endif
        if (!std::filesystem::exists(dirPath)) {
            std::filesystem::create_directory(dirPath);
        }
        std::ofstream config_file(configPath);
        config_file << "width=450\nheight=450\nx=735\ny=50\n";
        config_file.close();
    }

    if (width > 0 && height > 0) {
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
    } else {
        SCREEN_WIDTH = 450;
        SCREEN_HEIGHT = 450;
    }

    glm::vec2 pos;
    if (x >= 0 && y >= 0) {
        pos = glm::vec2(x, y);
    } else {
        pos = glm::vec2(735, 50);
    }

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_DECORATED, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_FOCUSED, false);
    glfwWindowHint(GLFW_SAMPLES, 16);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "App", nullptr, nullptr);
    glfwSetWindowPos(window, pos.x, pos.y);
    glfwMakeContextCurrent(window);
    // Get GLFW display
    hide_taskbar_icon(window);

    // glew: load all OpenGL function pointers
    // ---------------------------------------
    GLenum err = glewInit();
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW error" << err << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    App.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        App.ProcessInput(deltaTime);

        // update game state
        // -----------------
        App.Update(deltaTime);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render
        // ------
        App.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            App.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            App.Keys[key] = false;
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    App.MousePos->x = xpos;
    App.MousePos->y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        App.MouseLeft = true;
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        App.MouseLeft = false;

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        App.MouseRight = true;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        App.MouseRight = false;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// Hide taskbar icon in windows
#ifdef _WIN32
void hide_taskbar_icon(GLFWwindow* win) {
    FreeConsole();
    glfwHideWindow(win);
    SetWindowLong(glfwGetWin32Window(win), GWL_EXSTYLE, WS_EX_TOOLWINDOW);
    glfwShowWindow(win);
}
#endif

// Hide taskbar icon in linux
#ifdef __linux__
void hide_taskbar_icon(GLFWwindow* window) {
    Window win = glfwGetX11Window(window);
    Display* display = glfwGetX11Display();
    XEvent xev;
    Atom _NET_WM_STATE = XInternAtom(display, "_NET_WM_STATE", False);
    Atom _NET_WM_STATE_SKIP_TASKBAR = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);
    memset(&xev, 0, sizeof(xev));
    xev.type = ClientMessage;
    xev.xclient.window = win;
    xev.xclient.message_type = _NET_WM_STATE;
    xev.xclient.format = 32;
    xev.xclient.data.l[0] = 1;
    xev.xclient.data.l[1] = _NET_WM_STATE_SKIP_TASKBAR;
    xev.xclient.data.l[2] = 0;
    XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
}
#endif
