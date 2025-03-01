#pragma once

#include <vector>
#include <fstream>
#include <stdexcept>
#include <filesystem>

namespace CCUtilities
{
    std::vector<char> readBinaryFile(std::filesystem::path filename);
}