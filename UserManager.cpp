#include "UserManager.h"

#include <iostream>

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
    (void)filename;
}

void UserManager::saveToFile(const std::string &filename) const
{
    (void)filename;
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
