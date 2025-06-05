#ifndef PLOTEDITOR_H
#define PLOTEDITOR_H

#include "point.hpp"
#include "project.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <array>

namespace plotlab {

auto get_closest_point(project& proj, const sf::Vector2i& pos, point*& closest) -> bool;
auto get_closest_directive_index(project& proj, const sf::Vector2i& pos, int& closest_idx) -> bool;

class PlotEditor {
  private:
    bool is_holding{};
    point* held{nullptr};

    void attempt_point_move(project& proj, const sf::Vector2i& mouse_pos);
    void attempt_point_create(project& proj, const sf::Vector2i& mouse_pos);
    void attempt_handle_move_or_create(project& proj, const sf::Vector2i& mouse_pos);
    void attempt_prev_handle_move_or_create(project& proj, const sf::Vector2i& mouse_pos);

    std::array<sf::Vertex, 2> line;

  public:
    PlotEditor();
    void handle_event(const sf::Event::MouseButtonPressed* event, project& proj);
    void handle_event(const sf::Event::MouseButtonReleased* event, project& proj);
    void handle_event(const sf::Event::MouseMoved* event, project& proj);
};

} // namespace plotlab

#endif /* PLOTEDITOR_H */
