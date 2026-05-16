#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "BaseManager.h"
#include "User.h"

// User 객체 여러개를 관리할 전용 클래스
class UserManager : public BaseManager
{
private:
    std::vector<User> users; // 유저 목록 저장소
    int nextId;              // 다음에 추가될 유저 ID

public:
    UserManager();

    int addUser(const std::string &name, const std::string &email); // ID는 자동으로 부여
    void loadFromFile(const std::string &filename) override;
    void saveToFile(const std::string &filename) const override;
    std::size_t size() const override;
    const User *findUserById(int id) const;                         // 읽기 전용으로 유저 찾을때
    void printAllUsers() const;                                     // 전체 유저 목록 출력

    std::size_t getUserCount() const; // 현재 유저 수 확인용
};
