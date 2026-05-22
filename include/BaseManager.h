#pragma once

#include <cstddef>
#include <string>

class BaseManager
{
public:
    virtual void loadFromFile(const std::string &filename) = 0;
    virtual void saveToFile(const std::string &filename) const = 0;
    virtual std::size_t size() const = 0;

    virtual ~BaseManager() = default;
};
