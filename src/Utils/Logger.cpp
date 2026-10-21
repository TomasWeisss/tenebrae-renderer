#include "Logger.h"

std::array<Logger::LogEntry, Logger::MAX_TRACKED_MESSAGES> Logger::_recentMessages{};
std::mutex Logger::_mutex;

const char* GetColor(Logger::Severity severity)
{
    switch (severity)
    {
        case Logger::Severity::Info:
            return COLOR_WHITE;

        case Logger::Severity::Warning:
            return COLOR_YELLOW;

        case Logger::Severity::Error:
            return COLOR_RED;

        default:
            return "";
    }
}

void Logger::Log(const std::string &origin, const std::string &message, Severity severity)
{
    Log(std::format("[{}] {}", origin, message), severity);
}

void Logger::Log(const std::string &str, Severity severity)
{
    std::lock_guard lock(_mutex);

    auto now = std::chrono::steady_clock::now();

    for (auto &entry : _recentMessages)
    {
        if (entry.count == 0 || entry.message != str)
            continue;

        entry.count++;
        entry.lastOccurred = now;

        if (entry.count <= GROUP_AFTER)
        {
            std::cout << GetColor(severity) << str << COLOR_WHITE << std::endl;
            entry.lastPrinted = now;
        }
        else if (now - entry.lastPrinted >= THROTTLE_INTERVAL)
        {
            std::cout << GetColor(severity) << str << " (" << entry.count << "x)" << COLOR_WHITE << std::endl;
            entry.lastPrinted = now;
        }

        return;
    }

    // Replace oldest
    auto oldestEntry = std::ranges::min_element(_recentMessages,
                                                [](const LogEntry &a, const LogEntry &b)
                                                {
                                                    if (a.count == 0)
                                                        return true;
                                                    if (b.count == 0)
                                                        return false;
                                                    return a.lastPrinted < b.lastPrinted;
                                                });
    oldestEntry->message = str;
    oldestEntry->count = 1;
    oldestEntry->severity = severity;
    oldestEntry->lastOccurred = now;
    oldestEntry->lastPrinted = now;

    // Print the message
    std::cout << GetColor(severity) << str << COLOR_WHITE << std::endl;
}

void Logger::FlushMessages()
{
    std::lock_guard lock(_mutex);

    auto now = std::chrono::steady_clock::now();

    for (auto &entry : _recentMessages)
    {
        if (entry.count == 0)
            continue;

        if (now - entry.lastPrinted >= THROTTLE_INTERVAL)
        {
            // Hasn't occurred since last print
            if (entry.lastPrinted >= entry.lastOccurred)
            {
                entry.message.clear();
                entry.count = 0;
                continue;
            }

            entry.lastPrinted = now;
            std::cout << GetColor(entry.severity) << entry.message << " (" << entry.count << "x)" << COLOR_WHITE << std::endl;
        }
    }
}
