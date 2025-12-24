#pragma once

#include <chrono>
#include <functional>
#include <tuple>

template<typename... Args>
class Timer
{
public:

    std::chrono::milliseconds duration;
    std::chrono::high_resolution_clock::time_point initialTime;

    bool repetition = false;
    bool autoStart = false;
    bool isRunning = false;

    std::function<void(Args...)> callback;
    std::tuple<Args...> args;

    Timer() = default;
    Timer(double duration, std::function<void(Args...)> callback = nullptr, bool repetition = false, bool autoStart = false, Args&&... args)
        : duration(static_cast<int>(duration)),
        callback(callback),
        repetition(repetition),
        autoStart(autoStart),
        args(std::forward<Args>(args)...)
    {
        if (autoStart)
            active();
    }
    int elapsedTime()
    {
        if (isRunning)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - initialTime).count();
            return static_cast<int>(elapsed);
        }
        return -1;
    }
    void active()
    {
        isRunning = true;
        initialTime = std::chrono::high_resolution_clock::now();
    }
    void deactive()
    {
        isRunning = false;

        if (repetition)
            active();
    }
    void update()
    {
        if (isRunning)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();

            if (currentTime - initialTime >= duration)
            {
                if (callback != nullptr)
                    std::apply(callback, args); // per fare la chiamata a funzione con args

                deactive();
            }
        }
    }
    int elapsedTime()
    {
        if (isRunning)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - initialTime).count();
            return static_cast<int>(elapsed);
        }
        return 0.0f;
    }
};
