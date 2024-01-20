#ifndef LOGGINGMANAGER_HPP
#define LOGGINGMANAGER_HPP

#include <filesystem>
#include <fstream>
#include <string>

class LoggingManager {
public:
    enum class LoggingMethod { none, console, file };
    enum class LogLevel {
        none = 0,
        error = 1,
        warning = 2,
        info = 3,
        debug = 4
    };

    LoggingManager(LogLevel log_level);

    auto to_console() -> bool;
    auto to_file(const std::filesystem::path& filepath) -> bool;

    auto write(const std::string& string) const -> void;
    auto debug(const std::string& string) const -> void;
    auto info(const std::string& string) const -> void;
    auto warning(const std::string& string) const -> void;
    auto error(const std::string& string) const -> void;

private:
    LoggingMethod logging_method_;
    LogLevel log_level_;
    std::ostream* output_stream_;
    std::ofstream file_stream_;
};

#endif // LOGGINGMANAGER_HPP
