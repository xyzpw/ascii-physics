#pragma once

#include <unordered_map>
#include <string>

struct ParsedArgs {
    std::unordered_map<std::string, std::string> args;

    std::string getKeyValue(std::string key);
    std::string getKeyValueOther(std::string key, std::string other);
    bool checkKeyExists(std::string key);
    bool checkKeyExistsOther(std::string key, std::string other);
    void append(std::string key, std::string value);
};

ParsedArgs parseCmdArgs(int argc, char* argv[]);
