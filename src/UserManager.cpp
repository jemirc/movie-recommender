#include "UserManager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "MovieConstants.h"

namespace
{
bool shouldSkipLine(const std::string &line)
{
    return line.empty() || line[0] == '#';
}

User parseUserLine(const std::string &line)
{
    std::stringstream ss(line);
    std::string idText;
    std::string name;
    std::string email;
    std::string extraText;

    if (!std::getline(ss, idText, ',') ||
        !std::getline(ss, name, ',') ||
        !std::getline(ss, email, ',') ||
        std::getline(ss, extraText, ','))
    {
        throw std::invalid_argument("사용자 CSV는 " + std::to_string(MovieConstants::USER_CSV_FIELD_COUNT) + "개 값이어야 합니다");
    }

    return User(std::stoi(idText), name, email);
}
}

UserManager::UserManager()
    : nextId(1)
{
}

int UserManager::addUser(const std::string &name, const std::string &email)
{
    int id = nextId;
    users.push_back(User(id, name, email));
    nextId++;
    return id;
}

void UserManager::loadFromFile(const std::string &filename)
{
    users.clear();
    nextId = 1;

    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("사용자 CSV 파일을 열 수 없습니다: " + filename);
    }

    std::string line;
    int lineNumber = 0;
    int maxId = 0;

    while (std::getline(file, line))
    {
        lineNumber++;

        if (shouldSkipLine(line))
        {
            continue;
        }

        try
        {
            const User user = parseUserLine(line);
            const int id = user.getId();
            users.push_back(user);

            if (id > maxId)
            {
                maxId = id;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << filename << " " << lineNumber << "번 줄 건너뜀: " << e.what() << std::endl;
        }
    }

    nextId = maxId + 1;
}

void UserManager::saveToFile(const std::string &filename) const
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("사용자 CSV 파일을 저장할 수 없습니다: " + filename);
    }

    for (const User &user : users)
    {
        file << user.getId() << ','
             << user.getName() << ','
             << user.getEmail() << '\n';
    }
}

std::size_t UserManager::size() const
{
    return users.size();
}

const User *UserManager::findUserById(int id) const
{
    // const 버전은 읽기만 가능하게 const 포인터로 돌려줌
    for (const User &user : users)
    {
        if (user.getId() == id)
        {
            return &user;
        }
    }

    return nullptr;
}

void UserManager::printAllUsers() const
{
    // 유저 정보 출력도 User의 operator<<를 활용해서 통일하는거임
    for (const User &user : users)
    {
        std::cout << user << std::endl;
    }
}

std::size_t UserManager::getUserCount() const
{
    // 현재 등록된 유저 수 반환
    return size();
}
