#include "plot_editor.hpp"
#include "directives.hpp"
#include "project.hpp"
#include <SFML/System/Vector2.hpp>
#include <variant>

namespace plotlab {

PlotEditor::PlotEditor() = default;

void PlotEditor::attempt_point_move(project& proj, const sf::Vector2i& mouse_pos) {
    point* closest = nullptr;

    if (!get_closest_point(proj, mouse_pos, closest)) {
        return;
    }

    is_holding = true;
    held = closest;
}

void PlotEditor::attempt_point_create(project& proj, const sf::Vector2i& mouse_pos) {
    int closest_idx = -1;

    if (!get_closest_directive_index(proj, mouse_pos, closest_idx)) {
        return;
    }

    auto insertion_idx = proj.directives.begin() + closest_idx + 1;
    auto new_point = point_directive{point{mouse_pos.x, mouse_pos.y}};
    auto* inserted_directive = &*proj.directives.insert(insertion_idx, new_point);

    held = &std::get_if<point_directive>(inserted_directive)->dest;
    is_holding = true;
}

void PlotEditor::attempt_handle_move_or_create(project& proj, const sf::Vector2i& mouse_pos) {
    // If there isn't a next directive, ignore
    // If the next directive isn't a bezier, make it a bezier.
    // Grab the first handle of the next bezier

    int closest_idx = -1;

    if (!get_closest_directive_index(proj, mouse_pos, closest_idx)) {
        return;
    }

    if (closest_idx + 1 >= (int)proj.directives.size()) {
        return;
    }

    auto* next_dir_v = &proj.directives.at(closest_idx + 1);

    if (auto* next_dir = std::get_if<point_directive>(next_dir_v)) {
        proj.directives.at(closest_idx + 1) = bezier_directive{point{0, 0}, next_dir->dest, next_dir->dest};
    } else if (std::holds_alternative<draw_directive>(*next_dir_v)) {
        return;
    }

    next_dir_v = &proj.directives.at(closest_idx + 1);

    held = &std::get_if<bezier_directive>(next_dir_v)->h1;
    is_holding = true;
}

void PlotEditor::attempt_prev_handle_move_or_create(project& proj, const sf::Vector2i& mouse_pos) {
    // If the current directive isn't a bezier, make it a bezier.
    // Grab the second handle of the current bezier

    int closest_idx = -1;

    if (!get_closest_directive_index(proj, mouse_pos, closest_idx)) {
        return;
    }

    if (closest_idx == 0) {
        return;
    }

    auto* prev_dir_v = &proj.directives.at(closest_idx - 1);

    point prev_dest{0, 0};

    if (auto* prev_dir = std::get_if<point_directive>(prev_dir_v)) {
        prev_dest = prev_dir->dest;
    } else if (auto* prev_dir = std::get_if<bezier_directive>(prev_dir_v)) {
        prev_dest = prev_dir->dest;
    } else if (std::get_if<draw_directive>(prev_dir_v) != nullptr) {
        return;
    }

    auto* dir_v = &proj.directives.at(closest_idx);

    if (auto* dir = std::get_if<point_directive>(dir_v)) {
        proj.directives.at(closest_idx) = bezier_directive{prev_dest, dir->dest, dir->dest};
    } else if (std::holds_alternative<draw_directive>(*dir_v)) {
        return;
    }

    dir_v = &proj.directives.at(closest_idx);

    held = &std::get_if<bezier_directive>(dir_v)->h2;
    is_holding = true;
}

} // namespace plotlab
