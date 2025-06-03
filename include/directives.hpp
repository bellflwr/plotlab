#ifndef DIRECTIVES_H
#define DIRECTIVES_H

#include "point.hpp"
#include <variant>
namespace plotlab {

struct draw_directive {
    bool draw;
};

struct point_directive {
    point dest;
};

struct bezier_directive {
    point h1;
    point h2;
    point dest;

    void get_point(float t, point& start, point& out);
};

using directive = std::variant<draw_directive, point_directive, bezier_directive>;

void to_json(json& j, const directive& p);

void from_json(const json& j, directive& p);

auto directive_to_string(directive& p) -> std::string;

} // namespace plotlab

#endif /* DIRECTIVES_H */
