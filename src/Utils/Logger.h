#pragma once

class Logger
{
public:
    enum class Severity
    {
        Info,
        Warning,
        Error
    };

    static void Log(const std::string &origin, const std::string &message, Severity severity);
    static void Log(const std::string &str, Severity severity);

    /// Without arguments the message is taken verbatim, so it may be any runtime string
    static std::string Format(std::string_view message) { return std::string(message); }

    template <typename... Args>
        requires(sizeof...(Args) > 0)
    static std::string Format(std::format_string<Args...> format, Args &&...args)
    {
        return std::format(format, std::forward<Args>(args)...);
    }

    /// Log any remaining throttled messages
    static void FlushMessages();

private:
    static constexpr std::chrono::milliseconds THROTTLE_INTERVAL{2000};
    static constexpr unsigned int MAX_TRACKED_MESSAGES{4};
    static constexpr unsigned int GROUP_AFTER = 3;

    struct LogEntry
    {
        std::string message;
        Severity severity;
        unsigned int count = 0;

        std::chrono::steady_clock::time_point lastOccurred;
        std::chrono::steady_clock::time_point lastPrinted;
    };
    static std::array<LogEntry, MAX_TRACKED_MESSAGES> _recentMessages;
    static std::mutex _mutex;
};
