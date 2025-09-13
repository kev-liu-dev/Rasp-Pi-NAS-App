#ifndef API_H
#define API_H

#include <string>
#include <vector>

bool login(const std::string& username, const std::string& password);
bool signup(const std::string& username, const std::string& password);
std::string restore_password(const std::string& username);

#endif // API_H
