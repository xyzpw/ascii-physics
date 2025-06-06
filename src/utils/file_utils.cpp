#include <filesystem>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "utils/file_utils.h"

std::vector<std::string> readFileLines(std::string path)
{
    std::vector<std::string> fileLines;

    bool fileExists = std::filesystem::exists(path);

    if (!fileExists){
        throw std::runtime_error("path '" + path + "' does not exist");
    }

    std::ifstream file(path);
    std::string fLine;

    while (std::getline(file, fLine)){
        fileLines.push_back(fLine);
    }

    file.close();

    return fileLines;
}
