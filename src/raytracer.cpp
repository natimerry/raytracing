#include "colors.hpp"
#include "utils/logging.hpp"
#include "vector.hpp"
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <print>
#include <ranges>
#include <fstream>
#include <rays.hpp>
#include <thread>

#define PROJECT_NAME "raytracer"

color::Color3 ray_color(const Ray& r)
{
    return color::Color3(0, 0, 0);
}

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

int main(int argc, char** argv)
{
    std::ofstream out_stream;
    if (argc >= 2)
    {
        out_stream = get_output_file_stream(argv[1]);
    }
    else
    {
        logging::error("Invalid arguments passed, requires an output filename.");
        exit(1);
    }

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera

    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = Point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = Vec3(viewport_width, 0, 0);
    auto viewport_v = Vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - Vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    files::write_to_stream(out_stream, "P3\n{} {}\n255\n", image_width, image_height);

    for (auto j : std::ranges::iota_view(0, image_height))
    {
        logging::progress::with_data("Remaining scanlines: {}", image_height - j);
        for (auto i : std::ranges::iota_view(0, image_width))
        {
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            Ray r(camera_center, ray_direction);

            color::Color3 pixel_color = ray_color(r);
            color::write_color(out_stream, pixel_color);
        }
    }
    out_stream.close();
}
