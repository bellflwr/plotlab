#include "point.hpp"
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;

namespace {
auto square(int num) -> int {
    return num * num;
}
} // namespace

namespace plotlab {
void to_json(json& j, const point& p) {
    j = json{{"x", p.x}, {"y", p.y}};
}

void from_json(const json& j, point& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

auto point::get_distance_squared(int other_x, int other_y) const -> int {
    return square(x - other_x) + square(y - other_y);
}

auto point::get_distance_squared(const sf::Vector2i& other) const -> int {
    return square(x - other.x) + square(y - other.y);
}

auto point::string() const -> std::string {
    std::ostringstream buf;
    buf << "(" << x << ", " << y << ")";
    return buf.str();
}
} // namespace plotlab