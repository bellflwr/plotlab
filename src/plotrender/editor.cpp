#include "directives.hpp"
#include "plotrender.hpp"
#include "project.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <variant>

namespace {
auto square(int num) -> int {
    return num * num;
}
} // namespace

namespace plotlab {

const int SELECT_RADIUS = 5.F;
const int SELECT_RADIUS_SQUARED = SELECT_RADIUS * SELECT_RADIUS;

void PlotRender::handle_event(const sf::Event::MouseButtonPressed* event,
                              project& proj) {
    int closest_dist_sq = -1;
    point* closest = nullptr;
    for (auto& directive : proj.directives) {
        auto* dir_v = &directive;
        if (std::holds_alternative<point_directive>(*dir_v)) {
            auto* dir = std::get_if<point_directive>(dir_v);
            int dist_sq = square(event->position.x - dir->dest.x) +
                          square(event->position.y - dir->dest.y);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->dest;
            }
        } else if (std::holds_alternative<bezier_directive>(*dir_v)) {
            auto* dir = std::get_if<bezier_directive>(dir_v);

            int dist_sq = square(event->position.x - dir->dest.x) +
                          square(event->position.y - dir->dest.y);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->dest;
            }

            dist_sq = square(event->position.x - dir->h1.x) +
                      square(event->position.y - dir->h1.y);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->h1;
            }

            dist_sq = square(event->position.x - dir->h2.x) +
                      square(event->position.y - dir->h2.y);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->h2;
            }
        }
    }

    if (closest == nullptr) {
        return;
    }

    if (closest_dist_sq <= SELECT_RADIUS_SQUARED) {
        is_holding = true;
        held = closest;

        std::cout << closest->x << " " << closest->y << '\n';
    } else {
        std::cout << closest_dist_sq << '\n';
    }
}

void PlotRender::handle_event(const sf::Event::MouseButtonReleased* /*event*/,
                              project& /*proj*/) {
    is_holding = false;
    held = nullptr;
}

void PlotRender::handle_event(const sf::Event::MouseMoved* event,
                              project& /*proj*/) {
    if (is_holding) {
        held->x = event->position.x;
        held->y = event->position.y;
    }
}
} // namespace plotlab
