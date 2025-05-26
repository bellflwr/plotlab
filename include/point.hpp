#ifndef POINT_H
#define POINT_H

#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace plotlab {

struct point {
    uint16_t x{};
    uint16_t y{};
    bool travel{};
};

void to_json(json& j, const point& p);

void from_json(const json& j, point& p);

} // namespace plotlab

#endif