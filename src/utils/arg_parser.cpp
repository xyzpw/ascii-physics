#include <string>
#include "utils/arg_parser.h"

std::string ParsedArgs::getKeyValue(std::string key)
{
    return this->args.at(key);
}

std::string ParsedArgs::getKeyValueOther(std::string key, std::string other)
{
    if (this->args.count(key)){
        return this->args.at(key);
    }
    return this->args.at(other);
}

bool ParsedArgs::checkKeyExists(std::string key)
{
    return this->args.count(key);
}

bool ParsedArgs::checkKeyExistsOther(std::string key, std::string other)
{
    return this->args.count(key) || this->args.count(other);
}

void ParsedArgs::append(std::string key, std::string value)
{
    this->args[key] = value;
}

ParsedArgs parseCmdArgs(int argc, char* argv[])
{
    ParsedArgs parsedArgs;

    auto checkIsKey = [&](int index){
        std::string val = argv[index];
        return val.at(0) == '-';
    };

    for (int i = 1; i < argc; ++i)
    {
        std::string argStr = argv[i];
        bool isKey = checkIsKey(i);
        bool hasMoreArgs = i + 1 < argc;

        if (!isKey)
            continue;

        argStr = argStr.substr(argStr.find_first_not_of('-'));

        std::string val = "";
        if (hasMoreArgs && !checkIsKey(i+1)){
            val = std::string(argv[i+1]);
        }

        parsedArgs.append(argStr, val);
    }
    return parsedArgs;
}
