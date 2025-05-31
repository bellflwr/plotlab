#include "directives.hpp"
#include "plotrender.hpp"
#include "project.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <variant>

namespace plotlab {

const int SELECT_RADIUS = 5.F;
const int SELECT_RADIUS_SQUARED = SELECT_RADIUS * SELECT_RADIUS;

void PlotRender::handle_event(const sf::Event::MouseButtonPressed* event,
                              project& proj) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        attempt_point_create(proj, event->position);
    } else {
        attempt_point_move(proj, event->position);
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

void PlotRender::attempt_point_move(project& proj,
                                    const sf::Vector2i& mouse_pos) {
    int closest_dist_sq = -1;
    point* closest = nullptr;

    for (int i = 0; i < (int)proj.directives.size(); i++) {
        auto* dir_v = &proj.directives.at(i);
        if (std::holds_alternative<point_directive>(*dir_v)) {
            auto* dir = std::get_if<point_directive>(dir_v);
            int dist_sq = dir->dest.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->dest;
            }
        } else if (std::holds_alternative<bezier_directive>(*dir_v)) {
            auto* dir = std::get_if<bezier_directive>(dir_v);

            int dist_sq = dir->dest.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->dest;
            }

            dist_sq = dir->h1.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->h1;
            }

            dist_sq = dir->h2.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->h2;
            }
        }
    }

    if (closest == nullptr) {
        return;
    }

    if (closest_dist_sq > SELECT_RADIUS_SQUARED) {
        return;
    }

    is_holding = true;
    held = closest;
}

void PlotRender::attempt_point_create(project& proj,
                                      const sf::Vector2i& mouse_pos) {
    int closest_dist_sq = -1;
    point* closest = nullptr;
    directive* closest_dir_v = nullptr;
    int closest_idx = -1;

    for (int i = 0; i < (int)proj.directives.size(); i++) {
        auto* dir_v = &proj.directives.at(i);
        if (std::holds_alternative<point_directive>(*dir_v)) {
            auto* dir = std::get_if<point_directive>(dir_v);
            int dist_sq = dir->dest.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->dest;
                closest_dir_v = dir_v;
                closest_idx = i;
            }
        } else if (std::holds_alternative<bezier_directive>(*dir_v)) {
            auto* dir = std::get_if<bezier_directive>(dir_v);

            int dist_sq = dir->dest.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->dest;
                closest_dir_v = dir_v;
                closest_idx = i;
            }

            dist_sq = dir->h1.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->h1;
                closest_dir_v = dir_v;
                closest_idx = i;
            }

            dist_sq = dir->h2.get_distance_squared(mouse_pos);

            if (closest_dist_sq < 0 || closest_dist_sq > dist_sq) {
                closest_dist_sq = dist_sq;
                closest = &dir->h2;
                closest_dir_v = dir_v;
                closest_idx = i;
            }
        }
    }

    if (closest == nullptr) {
        return;
    }

    if (closest_dist_sq > SELECT_RADIUS_SQUARED) {
        return;
    }

    if (std::holds_alternative<point_directive>(*closest_dir_v) ||
        std::holds_alternative<bezier_directive>(*closest_dir_v)) {
        auto* idk_man = &*proj.directives.insert(
            proj.directives.begin() + closest_idx + 1,
            point_directive{point{mouse_pos.x, mouse_pos.y}});

        held = &std::get_if<point_directive>(idk_man)->dest;
        is_holding = true;
    }
}

} // namespace plotlab
