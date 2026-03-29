// User.h를 불러올때 한번만 불러오게 (이중 불러오기 방지)
#pragma once
#include <string>

// 헤더 파일에서는 네임스페이스 오염 때문에 std namespace로 안씀
// User.h를 가져다 쓰는 다른 모든 파일에도 강제로 std가 씌어지기 때문
class User
{
private:
    int id;
    std::string name;
    std::string email;

public:
    User(int id, const std::string &name, const std::string &email);

    int getId() const;
    std::string getName() const;
    std::string getEmail() const;
    void display() const;
};