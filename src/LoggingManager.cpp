#include <chrono>
// IWYU pragma: no_include <bits/chrono.h> // for operator-, floor, milliseconds, system_clock
#include <format>
#include <iostream>

#include "LoggingManager.hpp"

LoggingManager::LoggingManager(LogLevel log_level) :
    logging_method_{LoggingMethod::none}, log_level_{log_level}, output_stream_{nullptr}
{
}

auto LoggingManager::to_console() -> bool
{
    if (std::cout) {
        logging_method_ = LoggingMethod::console;
        output_stream_ = &std::cout;
        return true;
    }
    else {
        logging_method_ = LoggingMethod::console;
        output_stream_ = nullptr;
        return false;
    }
}

auto LoggingManager::to_file(const std::filesystem::path& filepath) -> bool
{
    const std::filesystem::path absolute_path = runtime_dir_ / filepath;
    file_stream_ = std::ofstream(absolute_path, std::ios_base::app | std::ios_base::out);
    if (file_stream_) {
        logging_method_ = LoggingMethod::file;
        output_stream_ = static_cast<std::ostream*>(&file_stream_);
        return true;
    }
    else {
        logging_method_ = LoggingMethod::none;
        output_stream_ = nullptr;
        return false;
    }
}

auto LoggingManager::write(const std::string& string) const -> void
{
    if (logging_method_ != LoggingMethod::none) {
        *output_stream_ << string << '\n';
    }
}

// TODO: make log level if a compile time check
// log level should not change during runtime anyway
auto LoggingManager::debug(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::debug) {
        write(std::format("{0:%F %T} [DEBUG]: {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
    }
}

auto LoggingManager::info(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::info) {
        write(std::format("{0:%F %T} [INFO] : {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
    }
}

auto LoggingManager::warning(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::warning) {
        write(std::format("{0:%F %T} [WARN] : {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
    }
}

auto LoggingManager::error(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::error) {
        write(std::format("{0:%F %T} [ERROR]: {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
        output_stream_->flush();
    }
}
