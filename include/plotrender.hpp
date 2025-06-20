#ifndef PLOTRENDER_H
#define PLOTRENDER_H

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

class PlotRender {
  private:
    int pos_x{};
    int pos_y{};
    bool is_down{};

    sf::CircleShape circle{0};

    void draw_point(sf::RenderWindow& window, point* point);
    void draw_circle(sf::RenderWindow& window, int x, int y);
    void draw_circle(sf::RenderWindow& window, int x, int y, sf::Color color);
    void draw_line(sf::RenderWindow& window, int ax, int ay, int bx, int by);
    void draw_line(sf::RenderWindow& window, int ax, int ay, int bx, int by, sf::Color color);
    void draw_arrow(sf::RenderWindow& window, int ax, int ay, int bx, int by, sf::Color color);

    std::array<sf::Vertex, 2> line;

  public:
    PlotRender();
    void render(sf::RenderWindow& window, project& proj);
    void render_directive(sf::RenderWindow& window, directive& dir);
    void render_draw_dir(sf::RenderWindow& window, draw_directive& dir);
    void render_point_dir(sf::RenderWindow& window, point_directive& dir);
    void render_bezier_dir(sf::RenderWindow& window, bezier_directive& dir);
};

} // namespace plotlab

#endif /* PLOTRENDER_H */
