#pragma once 

enum class GameState
{
    ON_START_MENU,
    RUNNING_GAME,
    ON_PAUSED_MENU = 2,
    KILLED = 3,
};

enum class MeteorState
{
    INTEGRITY = 0,
    DAMAGED = 1,
};

enum Difficulty // normal enum on purpose
{
    EASY = 5,
    NORMAL = 4,
    HARD = 3
};

enum class ModifierType
{
    SLOWER = 0,
    LOSEHEART,
    TRIPLELASER,
    GAINHEART,
    BIGLASER
};

enum class MultiplayerPacketType : uint32_t
{
    PLAYER_ADDED,
    PLAYER_ADD_OTHERS,
    PLAYER_NOTIFY_ID,
    PLAYER_REMOVED,
    PLAYER_MOVE,
    PLAYER_SHOOT,
    PLAYER_DASH,
};