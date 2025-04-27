
#ifndef colors_H_
#define colors_H_

#include "vector.hpp"
#include <fstream>
#include <ostream>
#include <string_view>

namespace text_formatting
{
    constexpr std::string_view reset = "\033[0m";
    constexpr std::string_view red = "\033[31m";
    constexpr std::string_view yellow = "\033[33m";
    constexpr std::string_view green = "\033[32m";
    constexpr std::string_view blue = "\033[34m";
    constexpr std::string_view magenta = "\033[35m";
    constexpr std::string_view cyan = "\033[36m";
    constexpr std::string_view gray = "\033[90m";
    constexpr std::string_view bold = "\033[1m";
    constexpr std::string_view underline = "\033[4m";
    constexpr std::string_view inverted = "\033[7m";

    constexpr std::string_view bg_red = "\033[41m";
    constexpr std::string_view bg_green = "\033[42m";
    constexpr std::string_view bg_yellow = "\033[43m";
    constexpr std::string_view bg_blue = "\033[44m";
    constexpr std::string_view bg_magenta = "\033[45m";
    constexpr std::string_view bg_cyan = "\033[46m";
    constexpr std::string_view bg_gray = "\033[48m";
}

namespace color
{
    using Color3 = Vec3;
    void write_color(std::ostream& out, const Color3& pixel_color);
    void write_color(std::ofstream& out, const Color3& pixel_color);
}

#endif // colors_H_
