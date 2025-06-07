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
#include "constants/config_constants.h"
#include "constants/arg_name_constants.h"

using std::stod;

const std::regex RE_CONFIG_LINE_PATTERN{"^([a-zA-Z0-9]+?)\\s((?:-?\\d*?.?)?[0-9]+?)$"};
const std::regex RE_CONFIG_EVENT_PATTERN{"^([a-zA-Z0-9]+?)\\s([a-zA-Z0-9,.-]+?)$"};
const std::regex RE_VECTOR2D_PATTERN{"^(-?\\d*?\\.?\\d+?),(-?\\d*?\\.?\\d+)"};

bool checkIsAlphaNumeric(std::string text);
bool checkIsLineComment(const std::string& text);
std::pair<std::string, std::string> parseConfigLine(const std::string& text,
                                                    const std::regex& pattern);

void applyConfigToArgs(ParsedArgs& args, std::string name)
{
    if (!checkIsAlphaNumeric(name)){
        throw std::runtime_error("invalid simulation name");
    }

    auto simulationConfigLines = readFileLines("custom/" + name + ".conf");

    std::smatch reMatch;

    auto getLineInfo = [&](const std::string line) {
        if (std::regex_search(line, reMatch, RE_CONFIG_LINE_PATTERN)) {
            return std::make_pair(reMatch[1], reMatch[2]);
        }
        throw std::runtime_error("invalid config for '" + name + "': '" + line + "'");
    };

    auto addConfigArg = [&](const auto& var, const auto& info, const auto& key){
        if (args.checkKeyExists(key) || var != info.first) return;
        args.append(key, info.second);
    };

    for (const auto& line : simulationConfigLines)
    {
        if ("" == line || checkIsLineComment(line)) continue;

        std::pair<std::string, std::string> lineInfo = getLineInfo(line);

        for (const auto& it : CONF_VAR_LIST){
            addConfigArg(it.first, lineInfo, it.second);
        }
    }
}

void applyConfigToParsedArgs(
    ParsedArgs& args,
    const std::vector<std::string>& lines)
{
    auto addVarFn = [&](const auto& var, const auto& info, const auto& key){
        if (args.checkKeyExists(key) || var != info.first) return;
        args.append(key, info.second);
    };

    for (const auto& ln : lines)
    {
        auto lineInfo = parseConfigLine(ln, RE_CONFIG_LINE_PATTERN);

        for (const auto& it : CONF_VAR_LIST){
            addVarFn(it.first, lineInfo, it.second);
        }
    }
}

void applyConfigStartEvent(World& world, const std::vector<std::string>& lines)
{
    auto& commands = world.startupCommands;

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
            commands.isLaunchValid = true;
            continue;
        }
    }
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

bool checkIsLineComment(const std::string& text)
{
    int firstNonSpace = text.find_first_not_of(' ');
    if (text.at(text.find_first_not_of(' ')) == '#') return true;
    return false;
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

        if (line == "" || checkIsLineComment(line))
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
