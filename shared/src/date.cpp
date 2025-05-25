#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

std::string date() {
    using namespace std::chrono;
    auto now = system_clock::now();
    std::time_t now_time = system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::gmtime(&now_time), "%Y-%m-%d");
    return ss.str();
}
