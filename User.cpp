#include "User.h"
#include <iostream>

using namespace std;
User::User(int id, const string &name, const string &email) : id(id), name(name), email(email) {}

int User::getId() const { return id; }

ostream &operator<<(ostream &os, const User &user)
{
    os << "[사용자] "
       << "ID: " << user.id
       << " | 이름: " << user.name
       << " | 이메일: " << user.email;
    return os;
}
