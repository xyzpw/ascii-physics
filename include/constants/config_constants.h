#pragma once

/*
    NOTE: Variables from `constants/arg_name_constants.h` are used!
*/

#include <string>
#include <vector>
#include <utility>
#include "constants/arg_name_constants.h"

enum CONF_KEY {
    CONF_KEY_VAR,
    CONF_KEY_ON_START,
};

inline const std::string CONF_DIR = "custom";
inline const std::string CONF_VAR_NAME_MASS = "mass";
inline const std::string CONF_VAR_NAME_CHARSIZE = "metersPerChar";
inline const std::string CONF_VAR_NAME_COR = "cor";
inline const std::string CONF_VAR_NAME_TRAILS = "trails";
inline const std::string CONF_VAR_NAME_G = "g";
inline const std::string CONF_ON_START = "on start:";
inline const std::string CONF_CMD_LAUNCH = "launch";
inline const std::string CONF_CMD_WIND_VELOCITY = "windVelocity";

// Config var names and what cmd arg key they point to.
// Example: CONF_VAR_NAME_MASS, ARG_NAME_MASS
inline const std::vector<std::pair<std::string, std::string>> CONF_VAR_ARG_LIST {
    {CONF_VAR_NAME_MASS, ARG_NAME_MASS},
    {CONF_VAR_NAME_CHARSIZE, ARG_NAME_CHAR_SIZE},
    {CONF_VAR_NAME_COR, ARG_NAME_COR},
    {CONF_VAR_NAME_TRAILS, ARG_NAME_TRAILS},
    {CONF_VAR_NAME_G, ARG_NAME_G},
};

// Return the config path for a custom simulation given its name.
inline std::string getConfigNamePath(std::string name)
{
    std::string path = CONF_DIR + "/" + name + ".conf";
    return path;
}
