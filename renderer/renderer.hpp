#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "colors.hpp"

class Renderer
{
  public:
    Renderer(int width, int height, const char* title);
    ~Renderer();

    bool should_close() const;
    void poll_events() const;
    void swap_buffers() const;
    void clear() const;
    void render();
    void update_pixel_color(int x, int y, const color::Color3& color);

  private:
    void setup_shader();
    void setup_quad();
    void setup_texture();
    void update_texture();
    std::string load_shader_source(const std::string& filename);

    GLFWwindow* m_window;
    GLuint m_shader_program;
    GLuint m_vao, m_vbo;
    GLuint m_texture;

    std::vector<float> m_pixel_buffer;
    int m_width;
    int m_height;
};

#endif // RENDERER_H