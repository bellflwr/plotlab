#include "directives.hpp"
#include "plot_editor.hpp"
#include "project.hpp"
#include <SFML/System/Vector2.hpp>
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

auto get_closest_point(project& proj, const sf::Vector2i& pos, point*& closest) -> bool {
    int closest_dist_sq = std::numeric_limits<int>::max();
    closest = nullptr;

    for (auto& directive : proj.directives) {
        if (auto* dir = std::get_if<point_directive>(&directive)) {
            try_closest_point(&dir->dest, pos, closest_dist_sq, closest);
        } else if (auto* dir = std::get_if<bezier_directive>(&directive)) {
            try_closest_point(&dir->dest, pos, closest_dist_sq, closest);
            try_closest_point(&dir->h1, pos, closest_dist_sq, closest);
            try_closest_point(&dir->h2, pos, closest_dist_sq, closest);
        }
    }

    return closest_dist_sq <= SELECT_RADIUS_SQUARED;
}

auto get_closest_directive_index(project& proj, const sf::Vector2i& pos, int& closest_idx) -> bool {
    int closest_dist_sq = std::numeric_limits<int>::max();

    int j = 0;
    for (auto& directive : proj.directives) {
        if (auto* dir = std::get_if<point_directive>(&directive)) {
            try_closest_directive(&dir->dest, pos, closest_dist_sq, closest_idx, j);
        } else if (auto* dir = std::get_if<bezier_directive>(&directive)) {
            try_closest_directive(&dir->dest, pos, closest_dist_sq, closest_idx, j);
        }
        j++;
    }

    return closest_dist_sq <= SELECT_RADIUS_SQUARED;
}

} // namespace plotlab
