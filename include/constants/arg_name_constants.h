#pragma once

#include <string>
#include <vector>

inline const std::string ARG_NAME_COR = "cor";
inline const std::string ARG_NAME_CHAR_SIZE = "char-size";
inline const std::string ARG_NAME_LAUNCH_VELOCITY = "lvelocity";
inline const std::string ARG_NAME_LAUNCH_ANGLE = "langle";
inline const std::string ARG_NAME_STAT_COLLISIONS = "stat-collisions";
inline const std::string ARG_NAME_STAT_MAXSPEED = "stat-maxspeed";
inline const std::string ARG_NAME_HIT_FLASH = "hit-flash";

const std::vector<std::pair<std::string, std::string>> helpMenu = {
    {"-m <n>", "mass of object (in kilograms)"},
    {"-g <n>", "acceleration due to gravity (in m/s [def. 9.80665])"},
    {"--cor <n>", "coefficient of restitution"},
    {"--char-size <n>", "size of each character on screen (in meters)"},
    {"--lvelocity <n>", "launch velocity"},
    {"--langle <n>", "launch angle (in degrees)"},
    {"--stat-collisions", "display the number of collisions for the selected object"},
    {"--stat-maxspeed", "display the maximum speed achieved for the selected object"},
    {"--hit-flash", "objects flash red during collisions"},
};
