#include "point.hpp"
#include <nlohmann/json.hpp>
#include <sstream>
#include <string>

using json = nlohmann::json;

namespace plotlab {
void to_json(json& j, const point& p) {
    j = json{{"x", p.x}, {"y", p.y}};
}

void from_json(const json& j, point& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
}

auto point::string() const -> std::string {
    std::ostringstream buf;
    buf << "(" << x << ", " << y << ")";
    return buf.str();
}
} // namespace plotlab