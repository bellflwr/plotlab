#ifndef POINT_H
#define POINT_H

#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace plotlab {

struct point {
    int x{};
    int y{};

    [[nodiscard]] auto string() const -> std::string;
};

void to_json(json& j, const point& p);

void from_json(const json& j, point& p);

} // namespace plotlab

#endif