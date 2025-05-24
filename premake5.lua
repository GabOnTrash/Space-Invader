--
-- premake5.lua per Space-Invader
--

workspace "SpaceInvader"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "Game"

----------------------------------------------------------------
-- Progetto: Game (eseguibile)
----------------------------------------------------------------
project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    -- Cartelle di output
    targetdir ("bin/%{cfg.buildcfg}")
    objdir    ("bin-int/%{cfg.buildcfg}")

    -- File sorgente e header
    files {
        "src/**.cpp",
        "src/**.c",        -- se ci fossero file C
        "include/**.h",
        "include/**.hpp"
    }

    -- Include path
    includedirs {
        "include",        -- i tuoi header
        --[[ 1) via vcpkg  ]]
        -- os.findlib e toolchain vcpkg inseriscono automaticamente include e lib

        --[[ 2) via vendor  ]]
        -- "vendor/raylib/include"
    }

    -- Lib path e librerie da linkare (se non usi vcpkg)
    libdirs {
        --"vendor/raylib/lib"
    }
    links {
        "raylib",       -- la libreria Raylib
        -- librerie di sistema Windows richieste da Raylib:
        "winmm",
        "gdi32",
        "opengl32"
    }

    -- Filtri per Windows
    filter "system:windows"
        systemversion "latest"

    -- Impostazioni per Debug
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    -- Impostazioni per Release
    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"

----------------------------------------------------------------
-- (Opzionale) Progetto: Tools / Utility / Editor se ne avessi
----------------------------------------------------------------
--[[
project "Editor"
    kind "ConsoleApp"
    language "C++"
    files { "tools/**.cpp" }
    includedirs { "include" }
    links { "raylib", "winmm", "gdi32", "opengl32" }
--]]

