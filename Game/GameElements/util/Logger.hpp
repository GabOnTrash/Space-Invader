#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>
#include <filesystem>

class Logger
{
public:
    enum class Level { INFO, DEBUG, WARN, ERROR };

    static Logger& Get()
    {
        static Logger logger;
        return logger;
    }

    void Init(const std::string& fileName)
    {
        path = fileName;
        std::lock_guard<std::mutex> lock(m_mux);
        m_file.open(path, std::ios::out | std::ios::app);
    }

    void CleanUp()
    {
        if (m_file.is_open())
            m_file.close();

        m_file.open(path, std::ios::out | std::ios::trunc);

        if (m_file.is_open())
            m_file << GetTimeStamp() + " [" + LevelToString(Level::INFO).first + "] " + "File was too big, or cleanup called, file was reset.\n";
    }

    void LogConsole(Level level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(m_mux);

        auto type_color = LevelToString(level);
        std::string output = type_color.second + " [" + type_color.first + "] " + message + "\033[0m";

        std::cout << "\r\033[2K" << output << "\n> " << std::flush;
    }
    void LogFile(Level level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(m_mux);
        if (std::filesystem::file_size(path) >= FILE_SIZE)
            CleanUp();

        std::string output = GetTimeStamp() + " [" + LevelToString(level).first + "] " + message;

        if (m_file.is_open())
            m_file << output << std::endl;
    }

    void Log(Level level, const std::string& message)
    {
        LogFile(level, message);
        LogConsole(level, message);
    }

private:
    Logger() = default;
    ~Logger() { if (m_file.is_open()) m_file.close(); }

    std::string GetTimeStamp()
    {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");

        return ss.str();
    }

    std::pair<std::string, std::string> LevelToString(Level level)
    {
        switch (level)
        {
        case Level::INFO:   return { "INFO", "\033[1;32m" };
        case Level::DEBUG:  return { "DEBUG", "\033[1;36m" };
        case Level::WARN:   return { "WARN", "\033[1;33m" };
        case Level::ERROR:  return { "ERROR", "\033[1;31m" };
        default:            return { "UNKNOWN", "\033[0m" };
        }
    }

    std::string path;
    std::ofstream m_file;
    std::mutex m_mux;
    const uintmax_t FILE_SIZE = 1024 * 1024 * 10;
};

#define LOG_INFO_EVERYWHERE(msg) Logger::Get().Log(Logger::Level::INFO, msg)
#define LOG_DEBUG_EVERYWHERE(msg) Logger::Get().Log(Logger::Level::DEBUG, msg)
#define LOG_WARN_EVERYWHERE(msg) Logger::Get().Log(Logger::Level::WARN, msg)
#define LOG_ERROR_EVERYWHERE(msg) Logger::Get().Log(Logger::Level::ERROR, msg)

#define LOG_INFO_FILE(msg) Logger::Get().LogFile(Logger::Level::INFO, msg)
#define LOG_DEBUG_FILE(msg) Logger::Get().LogFile(Logger::Level::DEBUG, msg)
#define LOG_WARN_FILE(msg) Logger::Get().LogFile(Logger::Level::WARN, msg)
#define LOG_ERROR_FILE(msg) Logger::Get().LogFile(Logger::Level::ERROR, msg)

#define LOG_INFO_CONSOLE(msg) Logger::Get().LogConsole(Logger::Level::INFO, msg)
#define LOG_DEBUG_CONSOLE(msg) Logger::Get().LogConsole(Logger::Level::DEBUG, msg)
#define LOG_WARN_CONSOLE(msg) Logger::Get().LogConsole(Logger::Level::WARN, msg)
#define LOG_ERROR_CONSOLE(msg) Logger::Get().LogConsole(Logger::Level::ERROR, msg)