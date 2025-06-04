#include "plotrender.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace plotlab {
const int POINT_RADIUS = 3.F;

void PlotRender::draw_line(sf::RenderWindow& window, int ax, int ay, int bx, int by, sf::Color color) {
    line[0].position.x = (float)ax;
    line[0].position.y = (float)ay;
    line[1].position.x = (float)bx;
    line[1].position.y = (float)by;
    line[0].color = color;
    line[1].color = color;

    window.draw(line.data(), 2, sf::PrimitiveType::Lines);
}
void PlotRender::draw_line(sf::RenderWindow& window, int ax, int ay, int bx, int by) {
    draw_line(window, ax, ay, bx, by, sf::Color::White);
}

void PlotRender::draw_circle(sf::RenderWindow& window, int x, int y) {
    draw_circle(window, x, y, sf::Color::White);
}

void PlotRender::draw_circle(sf::RenderWindow& window, int x, int y, sf::Color color) {
    sf::Vector2f pos((float)x - POINT_RADIUS, (float)y - POINT_RADIUS);
    circle.setPosition(pos);
    circle.setOutlineColor(color);
    window.draw(circle);
}

void PlotRender::draw_point(sf::RenderWindow& window, point* point) {
    draw_circle(window, point->x, point->y);
}

void PlotRender::draw_arrow(sf::RenderWindow& window, int ax, int ay, int bx, int by, sf::Color color) {
    draw_line(window, ax, ay, bx, by, color);

    sf::Vector2f vec{(float)(bx - ax), (float)(by - ay)};

    if (vec.x == 0 && vec.y == 0) {
        return;
    }

    sf::Vector2f side1 = vec.normalized().rotatedBy(sf::degrees(45)) * 10.F;
    draw_line(window, bx, by, bx - (int)std::round(side1.x), by - (int)std::round(side1.y));

    sf::Vector2f side2 = vec.normalized().rotatedBy(sf::degrees(-45)) * 10.F;
    draw_line(window, bx, by, bx - (int)std::round(side2.x), by - (int)std::round(side2.y));
}

} // namespace plotlab
