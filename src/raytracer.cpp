#include "colors.hpp"
#include "renderer.hpp"
#include "utils/logging.hpp"
#include "vector.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <format>
#include <iostream>
#include <print>
#include <fstream>
#include <rays.hpp>
#include <threadpool.hpp>

#define PROJECT_NAME "raytracer"

std::ofstream get_output_file_stream(std::filesystem::path file_name)
{
    logging::info("Set output to: {}", file_name.c_str());
    std::ofstream out_stream(file_name);
    if (!out_stream)
    {
        logging::error("Opening {} failed.", file_name.c_str());
    }

    return out_stream;
}

float radians(float degrees)
{
    return degrees * (M_PI / 180.0f);
}

void handle_input(Renderer& renderer, Point3& camera_center)
{
    auto move_speed = 0.1f;             // Speed of camera movement
    Vec3 camera_front = Vec3(0, 0, -1); // Camera looks down the negative Z axis initially
    Vec3 camera_up = Vec3(0, 1, 0);     // World up vector

    Vec3 camera_right = unit_vec(cross(camera_front, camera_up)); // Right vector

    // Handle keyboard movement (WASD)
    if (renderer.is_key_pressed(GLFW_KEY_W))
        camera_center += camera_front * move_speed; // Move forward (camera front direction)
    if (renderer.is_key_pressed(GLFW_KEY_S))
        camera_center += camera_front * -move_speed; // Move backward
    if (renderer.is_key_pressed(GLFW_KEY_A))
        camera_center += camera_right * move_speed; // Move left (camera right direction)
    if (renderer.is_key_pressed(GLFW_KEY_D))
        camera_center += camera_right * -move_speed; // Move right
    if (renderer.is_key_pressed(GLFW_KEY_Q))         // Move up
        camera_center += camera_up * move_speed;
    if (renderer.is_key_pressed(GLFW_KEY_E)) // Move down
        camera_center += camera_up * -move_speed;
}

int main()
{
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 2160;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    Point3 camera_center = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vec3(viewport_width, 0, 0);
    auto viewport_v = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    auto render_logger = logging::make_logger(text_formatting::bg_magenta, "RENDERER");
    Renderer renderer(image_width, image_height, "Raytracer", render_logger);

    while (!renderer.should_close())
    {
        handle_input(renderer, camera_center);
        const int tile_size = 256;
        int num_tiles_x = (image_width + tile_size - 1) / tile_size;
        int num_tiles_y = (image_height + tile_size - 1) / tile_size;

        auto& pool = ThreadPool::global();
        std::atomic<int> tiles_completed(0);

        for (int ty = 0; ty < num_tiles_y; ty++)
        {
            for (int tx = 0; tx < num_tiles_x; tx++)
            {
                pool.enqueue([&, tx, ty]() {
                    int start_x = tx * tile_size;
                    int end_x = std::min(start_x + tile_size, image_width);
                    int start_y = ty * tile_size;
                    int end_y = std::min(start_y + tile_size, image_height);

                    for (int j = start_y; j < end_y; j++)
                    {
                        for (int i = start_x; i < end_x; i++)
                        {
                            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                            auto ray_direction = pixel_center - camera_center;
                            Ray r(camera_center, ray_direction);

                            color::Color3 pixel_color = r.ray_color();
                            renderer.update_pixel_color(i, j, pixel_color);
                        }
                    }

                    tiles_completed++;
                });
            }
        }

        while (tiles_completed < num_tiles_x * num_tiles_y)
        {
            renderer.clear();
            renderer.render();
            renderer.swap_buffers();
            renderer.poll_events();
        }
    }

    return 0;
}