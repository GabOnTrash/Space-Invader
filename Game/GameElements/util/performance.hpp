#pragma once

#include <unistd.h>
#include <fstream>
#include <string>
#include <raylib.h>

inline long GetMemoryUsageMB()
{
    static long mem = 0;

    static double lastUpdateTime = 0;
    double currentTime = GetTime();

    if (currentTime - lastUpdateTime > 1.0)
    {
        long pages = 0;
        std::ifstream file("/proc/self/statm");
        if (file >> pages)
        {
            file >> pages;
            long totalKB = pages * (sysconf(_SC_PAGESIZE) / 1024);
            double rawMB = static_cast<double>(totalKB) / 1024.0;
            mem = std::round(rawMB * 10.0) / 10.0;
        }

        lastUpdateTime = currentTime;
    }

    return mem;
}