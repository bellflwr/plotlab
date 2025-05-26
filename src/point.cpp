#include "point.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace plotlab {
void to_json(json& j, const point& p) {
    j = json{{"x", p.x}, {"y", p.y}, {"travel", p.travel}};
}

void from_json(const json& j, point& p) {
    j.at("x").get_to(p.x);
    j.at("y").get_to(p.y);
    j.at("travel").get_to(p.travel);
}
} // namespace plotlab