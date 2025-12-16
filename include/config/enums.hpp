#pragma once 

enum GameState
{
    START = 0,
    RUNNING = 1,
    PAUSED = 2,
    KILLED = 3,
};

enum MeteorState
{
    INTEGRITY = 0,
    DAMAGED = 1,
    DESTRYED = 2
};

enum Difficulty
{
    EASY = 5,
    NORMAL = 4,
    HARD = 3
};