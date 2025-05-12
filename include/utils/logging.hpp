#ifndef logging_H_
#define logging_H_
#include "utils/log_sink.hpp"
#include <chrono>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <ostream>
#include <print>
#include <colors.hpp>
#include <threadpool.hpp>

static inline std::string current_time()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&now_c));
    return buffer;
}

template <typename T>
concept Printable = requires(std::ostream& os, T val) {
    { os << val } -> std::convertible_to<std::ostream&>;
};

#include <string>
#include <format>

namespace logging
{

    class LambdaLogger : public ILogSink
    {
      public:
        LambdaLogger(std::string_view color, std::string label)
            : level_color(color),
              level_label(std::move(label))
        {
        }

      private:
        void log_impl(std::string_view fmt_str, std::format_args args) const override
        {
            std::string message = std::format("[{}{}{}{} {}] {}", level_color, text_formatting::bold, level_label,
                                              text_formatting::reset, current_time(), std::vformat(fmt_str, args));
            ThreadPool::global().enqueue_low_priority([message]() { std::cout << message << std::endl; });
        }

        std::string_view level_color;
        std::string level_label;

        static std::string current_time()
        {
            // Same as your earlier current_time implementation
            std::time_t now = std::time(nullptr);
            char buf[20];
            std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
            return buf;
        }
    };

    template <typename... Args>
    inline void __log(const std::string_view& level_color, const std::string& level_label,
                      std::format_string<Args...> fmt, Args&&... args)
    {
        std::string message =
            std::format("[{}{}{}{} {}] {}", level_color, text_formatting::bold, level_label, text_formatting::reset,
                        current_time(), std::vformat(fmt.get(), std::make_format_args(args...)));

        ThreadPool::global().enqueue_low_priority([msg = std::move(message)]() { std::println("{}", msg); });
    }

    inline std::shared_ptr<ILogSink> make_logger(std::string_view color, std::string label)
    {
        return std::make_shared<LambdaLogger>(color, std::move(label));
    }

    template <Printable... Args>
    inline void info(std::format_string<Args...> fmt, Args&&... args)
    {
        __log(text_formatting::green, "INFO", fmt, std::forward<Args>(args)...);
    }

    template <Printable... Args>
    inline void warn(std::format_string<Args...> fmt, Args&&... args)
    {
        __log(text_formatting::yellow, "WARN", fmt, std::forward<Args>(args)...);
    }

    template <Printable... Args>
    inline void error(std::format_string<Args...> fmt, Args&&... args)
    {
        __log(text_formatting::red, "ERROR", fmt, std::forward<Args>(args)...);
    }

    template <Printable... Args>
    inline void debug(std::format_string<Args...> fmt, Args&&... args)
    {
        __log(text_formatting::cyan, "DEBUG", fmt, std::forward<Args>(args)...);
    }

    template <Printable... Args>
    inline void trace(std::format_string<Args...> fmt, Args&&... args)
    {
        __log(text_formatting::magenta, "TRACE", fmt, std::forward<Args>(args)...);
    }

    namespace progress
    {
        // Helper function to log progress (overwriting the line)
        template <typename... Args>
        inline void logr(bool final, std::format_string<Args...> fmt, Args&&... args)
        {
            auto print = [fmt, &args...]() {
                std::print("\r[{}{}PROGRESS{} {}] {} ", text_formatting::blue, text_formatting::bold,
                           text_formatting::reset, current_time(),
                           std::vformat(fmt.get(), std::make_format_args(args...)));
            };
            if (!final)
            {
                print();
                std::cout.flush(); // Ensure immediate flushing for overwriting
            }
            else
            {
                print();
                std::cout.flush();
                std::print("\n... done");
                std::println();
            }
        }

        template <Printable... Args>
        inline void with_data(std::function<bool()> const& func, std::format_string<Args...> fmt, Args&&... args)
        {
            logr(func(), fmt, std::forward<Args>(args)...);
        }
    }
}

namespace files
{
    template <typename Stream, Printable... Args>
    void write_to_stream(Stream& stream, std::format_string<Args...> fmt, Args&&... args)
    {
        (stream << std::vformat(fmt.get(), std::make_format_args(args...)));
    }
}

#endif // logging_H_
