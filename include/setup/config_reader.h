#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "utils/arg_parser.h"
#include "constants/config_constants.h"

void applyConfigToArgs(ParsedArgs& args, std::string name);

void applyConfigToParsedArgs(
    ParsedArgs& args,
    const std::vector<std::string>& lines);

void applyConfigStartEvent(World&, const std::vector<std::string>& lines);

std::unordered_map<CONF_KEY, std::vector<std::string>> readCustomConfig(
    const std::string name);
