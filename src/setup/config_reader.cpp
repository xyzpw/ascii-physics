#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <utility>
#include <stdexcept>
#include "structs/world.h"
#include "structs/vector2d.h"
#include "utils/arg_parser.h"
#include "utils/file_utils.h"
#include "utils/screen_utils.h"
#include "constants/config_constants.h"
#include "constants/arg_name_constants.h"

using std::stod;

const std::regex RE_CONFIG_LINE_PATTERN{"^([a-zA-Z0-9]+?)\\s((?:-?\\d*?.?)?[0-9]+?)$"};
const std::regex RE_CONFIG_EVENT_PATTERN{"^([a-zA-Z0-9]+?)\\s([a-zA-Z0-9,.-]+?)$"};
const std::regex RE_VECTOR2D_PATTERN{"^(-?\\d+?(?:\\.\\d+?)?),(-?\\d+?(?:\\.\\d+?)?)"};

bool checkIsAlphaNumeric(std::string text);
std::pair<std::string, std::string> parseConfigLine(const std::string& text,
                                                    const std::regex& pattern);
void removeLineComment(std::string& text);
Vector2D parseVector2D(std::string text);

void applyConfigVars(World& world, ParsedArgs& args,
                     const std::vector<std::string>& lines)
{
    if (lines.size() == 0){
        return;
    }

    bool hasFinishedArgs = false; // has finished config args (we do this first)

    for (int i = 0; i < lines.size(); ++i)
    {
        const std::string ln = lines.at(i);
        std::string lnVar = ln.substr(0, ln.find_first_of(' '));
        std::string lnVal = ln.substr(ln.find_first_of(' ') + 1);

        // Error if line contains more than 1 space.
        if (lnVal.find_first_of(' ') != std::string::npos){
            throw std::runtime_error("invalid config usage: '" + ln + "'");
        }

        for (const auto& it : CONF_VAR_ARG_LIST){
            if (args.checkKeyExists(it.second) || lnVar != it.first) continue;
            args.append(it.second, lnVal);
        }

        if (!hasFinishedArgs && i == lines.size() - 1){
            i = 0;
            hasFinishedArgs = true;
            continue;
        }

        if (!hasFinishedArgs)
            continue;

        if (lnVar == CONF_VAR_NAME_OBSTACLE){
            double size = 1.0;
            if (args.checkKeyExists(ARG_NAME_CHAR_SIZE)){
                size = stod(args.getKeyValue(ARG_NAME_CHAR_SIZE));
                world.metersPerChar = size; // set world char size before adding obstacle
            }

            Vector2D vec = parseVector2D(lnVal);
            Position pos = vectorToPosition(vec, size);
            world.addObstacle(pos);
        }
    }
}

void applyConfigStartEvent(World& world, const std::vector<std::string>& lines)
{
    if (lines.size() == 0){
        return;
    }

    auto& commands = world.startupCommands;
    commands.active = true;

    std::smatch reMatch;

    // Parse text to vector2d.
    auto parseVecFn = [&](const std::string& val){
        if (std::regex_search(val, reMatch, RE_VECTOR2D_PATTERN)){
            return Vector2D{stod(reMatch[1].str()), stod(reMatch[2].str())};
        }
        throw std::runtime_error("config contains invalid vector values");
    };

    for (const auto& ln : lines)
    {
        auto lineInfo = parseConfigLine(ln, RE_CONFIG_EVENT_PATTERN);

        if (lineInfo.first == CONF_CMD_LAUNCH){
            commands.launch = parseVecFn(lineInfo.second);
            continue;
        }
        else if (lineInfo.first == CONF_CMD_WIND_VELOCITY){
            commands.windVelocity = stod(lineInfo.second);
            continue;
        }
    }
}

Vector2D parseVector2D(std::string text)
{
    std::smatch reMatch;

    if (std::regex_search(text, reMatch, RE_VECTOR2D_PATTERN)){
        return Vector2D{stod(reMatch[1].str()), stod(reMatch[2].str())};
    }
    throw std::runtime_error("config contains invalid vector values");
}

bool checkIsAlphaNumeric(std::string text)
{
    std::regex rePattern{"^[0-9a-zA-Z]+$"};
    std::smatch reMatch;

    if (std::regex_search(text, reMatch, rePattern)) {
        return true;
    }

    return false;
}

void removeLineComment(std::string& text)
{
    auto index = text.find_first_of('#');
    if (index == std::string::npos) return;

    text = text.substr(0, index);

    text = text.substr(0, text.find_last_not_of(' ') + 1);
}

// return pair of line as (key, val), throws exception on regex failure.
std::pair<std::string, std::string> parseConfigLine(const std::string& text,
                                                    const std::regex& pattern)
{
    std::smatch reMatch;

    if (std::regex_search(text, reMatch, pattern)){
        return std::make_pair(reMatch[1].str(), reMatch[2].str());
    }

    throw std::runtime_error("error parsing config line '" + text + "'");
}

std::unordered_map<CONF_KEY, std::vector<std::string>> readCustomConfig(
    const std::string name)
{
    std::unordered_map<CONF_KEY, std::vector<std::string>> config;
    config[CONF_KEY_VAR] = {};
    config[CONF_KEY_ON_START] = {};

    auto configFileLines = readFileLines(getConfigNamePath(name));

    CONF_KEY currentConfKey = CONF_KEY_VAR;

    // Raise exception for incorrect event usage.
    auto badEventUsageFn = [&](){
        throw std::runtime_error("incorrect event usage");
    };

    // Check if string has leading whitespaces.
    auto hasLSpaceFn = [&](const std::string& text){
        return text.length() < 1 || text.at(0) == ' ';
    };

    for (int index = 0; index < configFileLines.size(); ++index)
    {
        std::string& line = configFileLines[index];
        const bool isLastIndex = index + 1 == configFileLines.size();

        removeLineComment(line);

        if (line == "")
            continue;

        if (line == CONF_ON_START) {
            currentConfKey = CONF_KEY_ON_START;
            continue;
        }

        line = line.substr(line.find_first_not_of(' ')); // remove leading spaces

        config.at(currentConfKey).push_back(line);

        // Change current config key to var if the next line is not indented.
        if (currentConfKey != CONF_KEY_VAR && !isLastIndex &&
            !hasLSpaceFn(configFileLines[index + 1]))
        {
            currentConfKey = CONF_KEY_VAR;
        }
    }

    return config;
}
