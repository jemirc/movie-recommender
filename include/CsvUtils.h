#pragma once

#include <string>
#include <vector>

namespace CsvUtils
{
bool parseLine(const std::string &line, std::vector<std::string> &fields);
std::string makeLine(const std::vector<std::string> &fields);
}
