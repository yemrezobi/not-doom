#include <chrono>
// IWYU pragma: no_include <bits/chrono.h> // for operator-, floor, milliseconds, system_clock
#include <format>
#include <iostream>
#include <ostream>

#include "Logger.hpp"

std::unique_ptr<Logger> Logger::default_logger_ = nullptr;
std::filesystem::path Logger::base_dir_ = std::filesystem::path();

auto Logger::set_base_path(const std::filesystem::path& base_dir) -> bool
{
    if (!std::filesystem::exists(base_dir)) {
        return false;
    }
    base_dir_ = base_dir;
    return true;
}

auto Logger::create_logger(const std::filesystem::path& filepath, Logger::LogLevel log_level) -> std::unique_ptr<Logger>
{
    const std::filesystem::path absolute_path = base_dir_ / filepath;
    std::ostream* output_stream = new std::ofstream(absolute_path, std::ios_base::out);
    if (!(*output_stream)) [[unlikely]] {
        return nullptr;
    }
    return std::make_unique<Logger>(output_stream, log_level);
}

auto Logger::create_console_logger(Logger::LogLevel log_level) -> std::unique_ptr<Logger>
{
    if (!std::cout) [[likely]] {
        return nullptr;
    }
    else {
        return std::make_unique<Logger>(&std::cout, log_level);
    }
}

auto Logger::set_default(std::unique_ptr<Logger> logger) -> void
{
    default_logger_ = std::move(logger);
}

auto Logger::get_default() -> Logger*
{
    return default_logger_.get();
}

Logger::Logger(std::ostream* output_stream, LogLevel log_level) noexcept : output_stream_(output_stream), log_level_{log_level}
{
}

Logger::~Logger() noexcept
{
    if (output_stream_.get() == &std::cout) {
        output_stream_.release();
    }
}

auto Logger::write(const std::string& string) const -> void
{
    if (output_stream_ == nullptr) {
        return;
    }
    *output_stream_ << string << '\n';
}

auto Logger::debug(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::debug) {
        write(std::format("{0:%F %T} [DEBUG]: {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
    }
}

auto Logger::info(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::info) {
        write(std::format("{0:%F %T} [INFO] : {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
    }
}

auto Logger::warning(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::warning) {
        write(std::format("{0:%F %T} [WARN] : {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
    }
}

auto Logger::error(const std::string& string) const -> void
{
    if (log_level_ >= LogLevel::error) {
        write(std::format("{0:%F %T} [ERROR]: {1}",
            std::chrono::floor<std::chrono::milliseconds>(std::chrono::system_clock::now()), string));
        output_stream_->flush();
    }
}
