#include <iostream>
#include <string>
#include "setup/input_handler.h"
#include "utils/arg_parser.h"

std::string getArgOrPrompt(ParsedArgs& args, std::string key,
                               std::string defPrompt)
{
    if (args.checkKeyExists(key)){
        return args.getKeyValue(key);
    }

    std::string result;

    std::cout << defPrompt;
    std::getline(std::cin, result);

    return result;
}
