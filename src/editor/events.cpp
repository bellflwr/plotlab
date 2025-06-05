#include "plot_editor.hpp"
#include "project.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace plotlab {

void PlotEditor::handle_event(const sf::Event::MouseButtonPressed* event, project& proj) {
    if (event->button != sf::Mouse::Button::Left) {
        return;
    }

    bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift);
    bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl);

    if (ctrl) {
        if (shift) {
            attempt_prev_handle_move_or_create(proj, event->position);
        } else {
            attempt_handle_move_or_create(proj, event->position);
        }
    } else {
        if (shift) {
            attempt_point_create(proj, event->position);
        } else {
            attempt_point_move(proj, event->position);
        }
    }
}

void PlotEditor::handle_event(const sf::Event::MouseButtonReleased* event, project& /*proj*/) {
    if (event->button != sf::Mouse::Button::Left) {
        return;
    }

    is_holding = false;
    held = nullptr;
}

void PlotEditor::handle_event(const sf::Event::MouseMoved* event, project& /*proj*/) {
    if (is_holding) {
        held->x = event->position.x;
        held->y = event->position.y;
    }
}

} // namespace plotlab
