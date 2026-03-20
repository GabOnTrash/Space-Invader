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
            m_file << GetTimeStamp() + " [" + LevelToString(Level::INFO) + "] " + "File was too big, cleanup was done.";
    }

    void Log(Level level, const std::string& message)
    {
        std::lock_guard<std::mutex> lock(m_mux);

        if (std::filesystem::file_size(path) >= FILE_SIZE)
            CleanUp();

        std::string output = GetTimeStamp() + " [" + LevelToString(level) + "] " + message;

        std::cout << output << std::endl;
        if (m_file.is_open())
            m_file << output << std::endl;
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

    std::string LevelToString(Level level)
    {
        switch (level)
        {
            case Level::INFO:   return "INFO";
            case Level::DEBUG:  return "DEBUG";
            case Level::WARN:   return "WARN";
            case Level::ERROR:  return "ERROR";
            default:            return "UNKNOWN";
        }
    }

    std::string path;
    std::ofstream m_file;
    std::mutex m_mux;
    const uintmax_t FILE_SIZE = 1024 * 1024 * 10;
};

#define LOG_INFO(msg) Logger::Get().Log(Logger::Level::INFO, msg)
#define LOG_DEBUG(msg) Logger::Get().Log(Logger::Level::DEBUG, msg)
#define LOG_WARN(msg) Logger::Get().Log(Logger::Level::WARN, msg)
#define LOG_ERROR(msg) Logger::Get().Log(Logger::Level::ERROR, msg)