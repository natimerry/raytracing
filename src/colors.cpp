#include "utils/logging.hpp"
#include <colors.hpp>
#include <ostream>
#include <tuple>

auto get_color_bytes(const color::Color3& pixel_color) -> std::tuple<int, int, int>
{
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    return {static_cast<int>(255.999 * r), static_cast<int>(255.999 * g), static_cast<int>(255.999 * b)};
}

void color::write_color(std::ostream& out, const Color3& pixel_color)
{
    auto [rbyte, gbyte, bbyte] = get_color_bytes(pixel_color);
    files::write_to_stream(out, "{} {} {}", rbyte, gbyte, bbyte);
}

void color::write_color(std::ofstream& out, const Color3& pixel_color)
{
    auto [rbyte, gbyte, bbyte] = get_color_bytes(pixel_color);
    files::write_to_stream(out, "{} {} {}\n", rbyte, gbyte, bbyte);
}