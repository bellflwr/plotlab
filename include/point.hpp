#ifndef POINT_H
#define POINT_H

#include <SFML/System/Vector2.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace plotlab {

struct point {
    int x{};
    int y{};

    [[nodiscard]] auto string() const -> std::string;
    [[nodiscard]] auto get_distance_squared(int other_x, int other_y) const -> int;

    [[nodiscard]] auto get_distance_squared(const sf::Vector2i& other) const -> int;
};

void to_json(json& j, const point& p);

void from_json(const json& j, point& p);

} // namespace plotlab

#endif /* POINT_H */
