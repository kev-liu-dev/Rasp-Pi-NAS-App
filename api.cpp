#include "api.h"
#include "httplib.h"
#include "../json.hpp"

bool login(const std::string& username, const std::string& password) {
    httplib::Client cli("http://localhost:8080");
    std::string query = "/api/login?username=" + username + "&password=" + password;
    auto res = cli.Get(query.c_str());
    return res && res->status == 200 && res->body == "true";
}

bool signup(const std::string& username, const std::string& password) {
    httplib::Client cli("http://localhost:8080");

    std::string query = "/api/signup?username=" + username + "&password=" + password;
    auto res = cli.Get(query.c_str());

    return res && res->status == 200 && res->body == "true";
}

std::string restore_password(const std::string& username) {
    httplib::Client cli("http://localhost:8080");
    std::string query = "/api/restore?username=" + username;
    auto res = cli.Get(query.c_str());
    if (res && res->status == 200) {
        return res->body;
    }
    return "";
}
