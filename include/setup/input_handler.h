#pragma once

#include <string>
#include "utils/arg_parser.h"

std::string getArgOrPrompt(ParsedArgs&, std::string key, std::string defPrompt);
