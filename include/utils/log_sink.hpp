#ifndef log_sink_H_
#define log_sink_H_

#include <format>
struct ILogSink
{
    virtual ~ILogSink() = default;

    template <typename... Args>
    void log(std::format_string<Args...> fmt, Args&&... args) const
    {
        this->log_impl(fmt.get(), std::make_format_args(args...));
    }

  private:
    virtual void log_impl(std::string_view fmt_str, std::format_args args) const = 0;
};

#endif // log_sink_H_