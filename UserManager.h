#pragma once

#include <cstddef>
#include <vector>

#include "User.h"

// User 객체 여러개를 관리할 전용 클래스
class UserManager
{
private:
    std::vector<User> users; // 유저 목록 저장소

public:
    UserManager() = default; // 아직은 따로 초기화할 값이 없음

    bool addUser(const User &user);              // 같은 id 유저는 중복 추가 막는거임
    User *findUserById(int id);                  // 수정 가능한 유저 찾을때
    const User *findUserById(int id) const;      // 읽기 전용으로 유저 찾을때
    void printAllUsers() const;                  // 전체 유저 목록 출력

    const std::vector<User> &getUsers() const;   // 전체 유저 목록 참조 반환
    std::size_t getUserCount() const;            // 현재 유저 수 확인용
};
