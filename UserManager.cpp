#include "UserManager.h"

bool UserManager::addUser(const User &user)
{
    // id가 같은 유저가 이미 있으면 추가 안하려는거임
    for (const User &existingUser : users)
    {
        if (existingUser.getId() == user.getId())
        {
            return false;
        }
    }

    // 새 유저면 vector 뒤에 붙이기
    users.push_back(user);
    return true;
}

User *UserManager::findUserById(int id)
{
    // id 일치하는 유저를 찾아서 주소를 반환
    for (User &user : users)
    {
        if (user.getId() == id)
        {
            return &user;
        }
    }

    return nullptr;
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

const std::vector<User> &UserManager::getUsers() const
{
    // 목록 전체를 복사하지 않게 참조로 반환
    return users;
}

std::size_t UserManager::getUserCount() const
{
    // 현재 등록된 유저 수 반환
    return users.size();
}
