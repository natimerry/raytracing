#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <vector>
#include "colors.hpp"
#include "utils/log_sink.hpp"
#include "vector.hpp"

class Renderer
{
  public:
    Renderer(int width, int height, const char* title, std::shared_ptr<ILogSink> log_sink)
        : m_logger(log_sink), // Get ownership of the logger
          m_width(width),
          m_height(height)
    {
        log_sink->log("Initialising renderer with dimensions: {}x{}", width, height);
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW");
        }

        m_window = glfwCreateWindow(width, height, title, nullptr, nullptr);
        if (!m_window)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to create GLFW window");
        }

        glfwMakeContextCurrent(m_window);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        if (glewInit() != GLEW_OK)
        {
            throw std::runtime_error("Failed to initialize GLEW");
        }

        // Initialize pixel buffer
        m_pixel_buffer.resize(width * height * 3, 0.0f);

        setup_shader();
        setup_quad();
        setup_texture();
    }

    ~Renderer();

    bool should_close() const;
    void poll_events() const;
    void swap_buffers() const;
    void clear() const;
    void render();
    void update_pixel_color(int x, int y, const color::Color3& color);
    bool is_key_pressed(int key);

    Vec3 get_mouse_delta();

  private:
    std::shared_ptr<ILogSink> m_logger;
    void setup_shader();
    void setup_quad();
    void setup_texture();
    void update_texture();

    static std::string load_shader_source(const std::string& filename);

    GLFWwindow* m_window;
    GLuint m_shader_program;
    GLuint m_vao, m_vbo;
    GLuint m_texture;

    std::vector<float> m_pixel_buffer;
    int m_width;
    int m_height;

    double last_mouse_x = 0;
    double last_mouse_y = 0;
    bool first_mouse = true;
};

#endif // RENDERER_H