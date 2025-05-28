#pragma once
#include "json.hpp"
#include "User.hpp"
#include "Request.hpp"
#include "Response.hpp"

using json = nlohmann::json;

Response signup(json& db,const Request& request);

Response login(json& db,const Request& request);

Response deposit(json& db, const Request& request);

Response withdraw(json& db, const Request& request);

Response transfer(json& db, const Request& request);
