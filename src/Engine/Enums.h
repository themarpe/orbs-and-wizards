#pragma once

enum class Direction {LEFT, RIGHT};

enum class PotionType{HEALTH, MANA};

enum class RuneType {FIRE2, FIRE3, ICE2, ICE3};

enum class Entities{Player = -2, Skeleton = -5, Door = -10, HealthPotion = -11, ManaPotion = -12, FireRune2 = -13, FireRune3 = -14, IceRune2 = -15, IceRune3 = -16, RuneKey = -17};

enum class InputCommands{
    NOP = 0,
    SDLK_a_DOWN = 8, SDLK_d_DOWN = 9, SDLK_w_DOWN = 10,
    SDLK_SPACE_DOWN = 11,
    SDLK_1_DOWN = 12, SDLK_2_DOWN = 13, SDLK_3_DOWN = 14,
    SDLK_q_DOWN = 15, SDLK_e_DOWN = 16,

    SDLK_a_UP = 40, SDLK_d_UP = 41, SDLK_w_UP = 42,
    SDLK_SPACE_UP = 43,
    SDLK_1_UP = 44, SDLK_2_UP = 45, SDLK_3_UP = 46,
    SDLK_q_UP = 47, SDLK_e_UP = 48, SDLK_ESCAPE_UP = 49, SDLK_k_UP = 50};
