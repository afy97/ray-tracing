#include "App.hpp"

App::App(int width, int height)
{
    error_check_result(glfwInit(), GLFW_TRUE);
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    main_window = glfwCreateWindow(width, height, "App", nullptr, nullptr);
    error_check_result(!main_window, false);
    glfwMakeContextCurrent(main_window);

    int w, h;
    glfwGetFramebufferSize(main_window, &w, &h);
    glViewport(0, 0, w, h);
    error_check_result(glewInit(), static_cast<GLenum>(GLEW_OK));
}

App::~App()
{
    glfwDestroyWindow(main_window);
    glfwTerminate();
}

void App::run()
{
    while (!glfwWindowShouldClose(main_window)) {
        for (auto function : command_list) {
            function();
        }

        glfwSwapBuffers(main_window);
        glfwWaitEventsTimeout(0.2);
    }
}

void App::add_command(std::function<void()> command)
{
    command_list.push_back(command);
}

void App::clear_commands() {
    command_list.clear();
}
