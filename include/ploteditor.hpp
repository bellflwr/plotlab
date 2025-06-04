#ifndef PLOTEDITOR_H
#define PLOTEDITOR_H

#include "directives.hpp"
#include "point.hpp"
#include "project.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Event.hpp>
#include <array>

namespace plotlab {

class PlotEditor {
  private:
    int pos_x{};
    int pos_y{};
    bool is_down{};

    bool is_holding{};
    point* held{nullptr};

    sf::CircleShape circle{0};

    void draw_point(sf::RenderWindow& window, point* point);
    void draw_circle(sf::RenderWindow& window, int x, int y);
    void draw_circle(sf::RenderWindow& window, int x, int y, sf::Color color);
    void draw_line(sf::RenderWindow& window, int ax, int ay, int bx, int by);
    void draw_line(sf::RenderWindow& window, int ax, int ay, int bx, int by, sf::Color color);
    void draw_arrow(sf::RenderWindow& window, int ax, int ay, int bx, int by, sf::Color color);

    void attempt_point_move(project& proj, const sf::Vector2i& mouse_pos);
    void attempt_point_create(project& proj, const sf::Vector2i& mouse_pos);

    std::array<sf::Vertex, 2> line;

  public:
    PlotRender();
    void handle_event(const sf::Event::MouseButtonPressed* event, project& proj);
    void handle_event(const sf::Event::MouseButtonReleased* event, project& proj);
    void handle_event(const sf::Event::MouseMoved* event, project& proj);
};

#endif /* PLOTEDITOR_H */
