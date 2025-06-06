#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <utility>
#include <stdexcept>
#include "structs/world.h"
#include "utils/arg_parser.h"
#include "utils/file_utils.h"
#include "constants/config_constants.h"
#include "constants/arg_name_constants.h"

using std::stod;

const std::regex RE_CONFIG_LINE_PATTERN{"^([a-zA-Z0-9]+?)\\s((?:-?\\d*?.?)?[0-9]+?)$"};

bool checkIsAlphaNumeric(std::string text);

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
        if ("" == line) continue;

        std::pair<std::string, std::string> lineInfo = getLineInfo(line);

        for (const auto& it : CONF_VAR_LIST){
            addConfigArg(it.first, lineInfo, it.second);
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
