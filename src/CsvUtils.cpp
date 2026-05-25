#include "CsvUtils.h"

namespace
{
std::string escapeField(const std::string &field)
{
    const bool needsQuotes = field.find_first_of(",\"\r\n") != std::string::npos;
    if (!needsQuotes)
    {
        return field;
    }

    std::string escaped = "\"";
    for (char ch : field)
    {
        if (ch == '"')
        {
            escaped += "\"\"";
        }
        else
        {
            escaped += ch;
        }
    }
    escaped += '"';
    return escaped;
}
}

namespace CsvUtils
{
bool parseLine(const std::string &line, std::vector<std::string> &fields)
{
    fields.clear();

    std::string field;
    bool inQuotes = false;

    for (std::size_t index = 0; index < line.size(); ++index)
    {
        const char ch = line[index];

        if (inQuotes)
        {
            if (ch == '"')
            {
                if (index + 1 < line.size() && line[index + 1] == '"')
                {
                    field += '"';
                    ++index;
                }
                else
                {
                    inQuotes = false;
                }
            }
            else
            {
                field += ch;
            }
            continue;
        }

        if (ch == ',')
        {
            fields.push_back(field);
            field.clear();
        }
        else if (ch == '"' && field.empty())
        {
            inQuotes = true;
        }
        else
        {
            field += ch;
        }
    }

    if (inQuotes)
    {
        fields.clear();
        return false;
    }

    fields.push_back(field);
    return true;
}

std::string makeLine(const std::vector<std::string> &fields)
{
    std::string line;

    for (std::size_t index = 0; index < fields.size(); ++index)
    {
        if (index > 0)
        {
            line += ',';
        }

        line += escapeField(fields[index]);
    }

    return line;
}
}
