#include "directives.hpp"
#include <cmath>
#include <sstream>
#include <variant>

using std::pow;

namespace plotlab {

void to_json(json& j, const directive& d) {
    if (const auto* dir = std::get_if<draw_directive>(&d)) {
        j = json{{"type", "draw"}, {"draw", dir->draw}};
    } else if (const auto* dir = std::get_if<point_directive>(&d)) {
        j = json{{"type", "point"}, {"dest", dir->dest}};
    } else if (const auto* dir = std::get_if<bezier_directive>(&d)) {
        j = json{{"type", "bezier"}, {"h1", dir->h1}, {"h2", dir->h2}, {"dest", dir->dest}};
    }
};

void from_json(const json& j, directive& p) {
    std::string type;
    j.at("type").get_to(type);

    if (type == "draw") {
        p = draw_directive{};
        j.at("draw").get_to(std::get<draw_directive>(p).draw);
    } else if (type == "point") {
        p = point_directive{};
        j.at("dest").get_to(std::get<point_directive>(p).dest);
    } else if (type == "bezier") {
        p = bezier_directive{};
        j.at("h1").get_to(std::get<bezier_directive>(p).h1);
        j.at("h2").get_to(std::get<bezier_directive>(p).h2);
        j.at("dest").get_to(std::get<bezier_directive>(p).dest);
    }
};

auto directive_to_string(directive& p) -> std::string {
    std::ostringstream buf;

    if (std::holds_alternative<point_directive>(p)) {
        auto dir = std::get<point_directive>(p);

        buf << "point_directive (\n";
        buf << "    dest: " << dir.dest.string() << "\n";
        buf << ")";

        return buf.str();
    }
    if (std::holds_alternative<draw_directive>(p)) {
        auto dir = std::get<draw_directive>(p);

        buf << "draw_directive ( draw: ";
        if (dir.draw) {
            buf << "true )";
        } else {
            buf << "false )";
        }

        return buf.str();
    }
    if (std::holds_alternative<bezier_directive>(p)) {
        auto dir = std::get<bezier_directive>(p);

        buf << "bezier_directive (\n";
        buf << "    h1: " << dir.h1.string() << ",\n";
        buf << "    h2: " << dir.h2.string() << ",\n";
        buf << "    dest: " << dir.dest.string() << "\n";
        buf << ")";

        return buf.str();
    }
    return "unknown_directive";
}

void bezier_directive::get_point(float t, point& start, point& out) {
    float k = 1 - t;

    float x = (pow(k, 3) * start.x) + (3 * pow(k, 2) * t * h1.x) + (3 * k * pow(t, 2) * h2.x) + (pow(t, 3) * dest.x);

    float y = (pow(k, 3) * start.y) + (3 * pow(k, 2) * t * h1.y) + (3 * k * pow(t, 2) * h2.y) + (pow(t, 3) * dest.y);

    out.x = (int)std::round(x);
    out.y = (int)std::round(y);
}

} // namespace plotlab
