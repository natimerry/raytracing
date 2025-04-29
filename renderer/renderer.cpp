#include "renderer.hpp"
#include <GLFW/glfw3.h>
#include <cstdio>
#include <fstream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <iostream>

Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteTextures(1, &m_texture);
    glDeleteProgram(m_shader_program);
    glfwDestroyWindow(m_window);
    glfwSwapInterval(1);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwTerminate();
}

void Renderer::setup_texture()
{
    // Create one texture and initialise to m_texture
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    // Initialize texture with empty data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_FLOAT, nullptr);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Renderer::update_texture()
{
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_pixel_buffer.data());
}

void Renderer::update_pixel_color(int x, int y, const color::Color3& color)
{
    if (x >= 0 && x < m_width && y >= 0 && y < m_height)
    {
        // Flip Y coordinate (OpenGL textures are bottom-up)
        int flipped_y = m_height - 1 - y;

        // Calculate position in buffer
        int index = (flipped_y * m_width + x) * 3;

        // Store components using x()=R, y()=G, z()=B
        m_pixel_buffer[index] = color.x();     // R
        m_pixel_buffer[index + 1] = color.y(); // G
        m_pixel_buffer[index + 2] = color.z(); // B
    }
}

Vec3 Renderer::get_mouse_delta()
{
    double current_x, current_y;
    glfwGetCursorPos(m_window, &current_x, &current_y);

    if (first_mouse)
    {
        last_mouse_x = current_x;
        last_mouse_y = current_y;
        first_mouse = false;
        return Vec3(0, 0, 0);
    }

    Vec3 delta(current_x - last_mouse_x, last_mouse_y - current_y, 0); // Inverted Y
    last_mouse_x = current_x;
    last_mouse_y = current_y;

    return delta;
}

std::string Renderer::load_shader_source(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + filename);
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void Renderer::setup_shader()
{
    // Vertex shader
    std::string vertex_shader_code = load_shader_source("renderer/shaders/vertex_shader.glsl");

    // Fragment shader
    std::string fragment_shader_code = load_shader_source("renderer/shaders/fragment_shader.glsl");

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertex_shader_cstr = vertex_shader_code.c_str();
    glShaderSource(vertex_shader, 1, &vertex_shader_cstr, nullptr);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragment_shader_cstr = fragment_shader_code.c_str();
    glShaderSource(fragment_shader, 1, &fragment_shader_cstr, nullptr);
    glCompileShader(fragment_shader);

    m_shader_program = glCreateProgram();
    glAttachShader(m_shader_program, vertex_shader);
    glAttachShader(m_shader_program, fragment_shader);
    glLinkProgram(m_shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

void Renderer::setup_quad()
{
    float vertices[] = {-1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f};

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

bool Renderer::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

void Renderer::poll_events() const
{
    glfwPollEvents();
}

bool Renderer::is_key_pressed(int key)
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

void Renderer::swap_buffers() const
{
    glfwSwapBuffers(m_window);
}

void Renderer::clear() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::render()
{
    update_texture();

    glUseProgram(m_shader_program);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glUniform1i(glGetUniformLocation(m_shader_program, "u_texture"), 0);

    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}