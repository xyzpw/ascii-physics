#pragma once

#include <string>
#include <vector>

inline const std::string ARG_NAME_MASS = "m";
inline const std::string ARG_NAME_G = "g";
inline const std::string ARG_NAME_COR = "cor";
inline const std::string ARG_NAME_CHAR_SIZE = "char-size";
inline const std::string ARG_NAME_LAUNCH_VELOCITY = "lvelocity";
inline const std::string ARG_NAME_LAUNCH_ANGLE = "langle";
inline const std::string ARG_NAME_STAT_COLLISIONS = "stat-collisions";
inline const std::string ARG_NAME_STAT_MAXSPEED = "stat-maxspeed";
inline const std::string ARG_NAME_STAT_HMAX = "stat-hmax";
inline const std::string ARG_NAME_HIT_FLASH = "hit-flash";
inline const std::string ARG_NAME_PRESET = "preset";
inline const std::string ARG_NAME_LIST_PRESET = "list-preset";
inline const std::string ARG_NAME_TRAILS = "trails";
inline const std::string ARG_NAME_FILE = "f";

const std::vector<std::pair<std::string, std::string>> helpMenu = {
    {"-m <n>", "mass of object (in kilograms)"},
    {"-g <n>", "acceleration due to gravity (in m/s [def. 9.80665])"},
    {"-" + ARG_NAME_FILE + " <file>", "name of custom simulation file"},
    {"--cor <n>", "coefficient of restitution"},
    {"--char-size <n>", "size of each character on screen (in meters)"},
    {"--lvelocity <n>", "launch velocity"},
    {"--langle <n>", "launch angle (in degrees)"},
    {"--stat-collisions", "display the number of collisions for the selected object"},
    {"--stat-maxspeed", "display the maximum speed achieved for the selected object"},
    {"--stat-hmax", "display the maximum height achieved for the selected object"},
    {"--hit-flash", "objects flash red during collisions"},
    {"--preset <item>", "select object preset"},
    {"--list-preset", "list presets"},
    {"--trails", "show object trails"},
};

const std::vector<std::pair<std::string, std::string>> presetMenu = {
    {"tennis", "mass 57 g, diameter 6.6 cm, cor 0.84"},
};
