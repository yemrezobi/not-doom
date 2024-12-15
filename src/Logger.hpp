#pragma once

#include <filesystem>
#include <fstream>
#include <string>

class Logger {
public:
    enum class LogLevel { none = 0, error = 1, warning = 2, info = 3, debug = 4 };

    explicit Logger(std::ostream* output_stream, LogLevel log_level) noexcept;

    ~Logger() noexcept;

    auto static set_base_path(const std::filesystem::path& base_path) -> bool;
    auto static create_logger(const std::filesystem::path& filepath, Logger::LogLevel log_level) -> std::unique_ptr<Logger>;
    auto static create_console_logger(Logger::LogLevel log_level) -> std::unique_ptr<Logger>;
    auto static set_default(std::unique_ptr<Logger> logger) -> void;
    auto static get_default() -> Logger*;

    auto write(const std::string& string) const -> void;
    auto debug(const std::string& string) const -> void;
    auto info(const std::string& string) const -> void;
    auto warning(const std::string& string) const -> void;
    auto error(const std::string& string) const -> void;

private:
    static std::unique_ptr<Logger> default_logger_;
    static std::filesystem::path base_dir_;
    std::unique_ptr<std::ostream> output_stream_;
    LogLevel log_level_;
};
