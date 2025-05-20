#pragma once
#include "json.hpp"
#include "User.hpp"

using json = nlohmann::json;

json registerUser(json& db, const json& data);
json loginUser(json& db, const json& data);
json getUserData(json& db, const json& data);
