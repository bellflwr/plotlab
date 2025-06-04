#include "directives.hpp"
#include "ploteditor.hpp"
#include "project.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <limits>
#include <variant>

namespace {
void try_closest_point(plotlab::point* test_point, const sf::Vector2i& mouse_pos, int& closest_dist_sq,
                       plotlab::point*& closest) {
    int dist_sq = test_point->get_distance_squared(mouse_pos);

    if (closest_dist_sq > dist_sq) {
        closest_dist_sq = dist_sq;
        closest = test_point;
    }
}

void try_closest_directive(plotlab::point* test_point, const sf::Vector2i& mouse_pos, int& closest_dist_sq,
                           int& closest_idx, int i) {
    int dist_sq = test_point->get_distance_squared(mouse_pos);

    if (closest_dist_sq > dist_sq) {
        closest_dist_sq = dist_sq;
        closest_idx = i;
    }
}

} // namespace

namespace plotlab {

const int SELECT_RADIUS = 5.F;
const int SELECT_RADIUS_SQUARED = SELECT_RADIUS * SELECT_RADIUS;

PlotEditor::PlotEditor() = default;

void PlotEditor::handle_event(const sf::Event::MouseButtonPressed* event, project& proj) {
    bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

    // if (ctrl) {
    // }

    if (shift) {
        attempt_point_create(proj, event->position);
    } else {
        attempt_point_move(proj, event->position);
    }
}

void PlotEditor::handle_event(const sf::Event::MouseButtonReleased* /*event*/, project& /*proj*/) {
    is_holding = false;
    held = nullptr;
}

void PlotEditor::handle_event(const sf::Event::MouseMoved* event, project& /*proj*/) {
    if (is_holding) {
        held->x = event->position.x;
        held->y = event->position.y;
    }
}

void PlotEditor::attempt_point_move(project& proj, const sf::Vector2i& mouse_pos) {
    int closest_dist_sq = std::numeric_limits<int>::max();
    point* closest = nullptr;

    for (auto& directive : proj.directives) {
        if (auto* dir = std::get_if<point_directive>(&directive)) {
            try_closest_point(&dir->dest, mouse_pos, closest_dist_sq, closest);
        } else if (auto* dir = std::get_if<bezier_directive>(&directive)) {
            try_closest_point(&dir->dest, mouse_pos, closest_dist_sq, closest);
            try_closest_point(&dir->h1, mouse_pos, closest_dist_sq, closest);
            try_closest_point(&dir->h2, mouse_pos, closest_dist_sq, closest);
        }
    }

    if (closest_dist_sq > SELECT_RADIUS_SQUARED) {
        return;
    }

    is_holding = true;
    held = closest;
}

void PlotEditor::attempt_point_create(project& proj, const sf::Vector2i& mouse_pos) {
    int closest_dist_sq = std::numeric_limits<int>::max();
    int closest_idx = -1;

    int j = 0;
    for (auto& directive : proj.directives) {
        if (auto* dir = std::get_if<point_directive>(&directive)) {
            try_closest_directive(&dir->dest, mouse_pos, closest_dist_sq, closest_idx, j);
        } else if (auto* dir = std::get_if<bezier_directive>(&directive)) {
            try_closest_directive(&dir->dest, mouse_pos, closest_dist_sq, closest_idx, j);
        }
        j++;
    }

    if (closest_dist_sq > SELECT_RADIUS_SQUARED) {
        return;
    }

    auto* idk_man = &*proj.directives.insert(proj.directives.begin() + closest_idx + 1,
                                             point_directive{point{mouse_pos.x, mouse_pos.y}});

    held = &std::get_if<point_directive>(idk_man)->dest;
    is_holding = true;
}

} // namespace plotlab
