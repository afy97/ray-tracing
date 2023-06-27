#pragma once

#include "pch.hpp"

class App {
private:
    GLFWwindow* main_window;
    std::future<void> event_loop;
    std::vector<std::function<void()>> command_list;

public:
    App(int width, int height);
    ~App();

    void run();
    void add_command(std::function<void()> command);
    void clear_commands();

    template <typename T>
    static void error_check_result(T returned, T expected)
    {
        if (returned != expected) {
            throw std::runtime_error("Expected value does not match");
        }
    }

    static void error_callback(int error_code, const char* description) {
        throw std::runtime_error(description);
    }
};
