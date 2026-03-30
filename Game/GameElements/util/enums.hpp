#pragma once 

enum class GameState
{
    ON_START_MENU,
    ON_PAUSE_MENU,
    ON_KILLED_MENU,
    ON_CONTROLS_MENU,
    ON_AUDIO_MENU,
    ON_CONNECTION_MENU,
    RUNNING_SINGLE_PLAYER,
    RUNNING_MULTI_PLAYER,
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
    PLAYER_SHOOT
};