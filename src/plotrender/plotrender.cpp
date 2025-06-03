#include "plotrender.hpp"
#include "directives.hpp"
#include "project.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <variant>

namespace plotlab {
const int POINT_RADIUS = 3.F;
const int POINT_OUTLINE_THICKNESS = 1.F;
const int BEZIER_SUBDIVS = 20;
const sf::Color HANDLE_COLOR = sf::Color{100, 100, 100};

PlotRender::PlotRender() {
    line = {sf::Vertex{sf::Vector2f(0.F, 0.F)}, sf::Vertex{sf::Vector2f(0.F, 0.F)}};

    circle.setRadius(POINT_RADIUS);
    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::White);
    circle.setOutlineThickness(POINT_OUTLINE_THICKNESS);
}

void PlotRender::render(sf::RenderWindow& window, project& proj) {
    pos_x = 0;
    pos_y = 0;
    is_down = false;

    for (auto dir : proj.directives) {
        render_directive(window, dir);
    }

    if (is_holding) {
        draw_circle(window, held->x, held->y, sf::Color::Yellow);
    }
}

void PlotRender::render_directive(sf::RenderWindow& window, directive& dir) {
    if (std::holds_alternative<point_directive>(dir)) {
        render_point_dir(window, std::get<point_directive>(dir));
    } else if (std::holds_alternative<draw_directive>(dir)) {
        render_draw_dir(window, std::get<draw_directive>(dir));
    } else if (std::holds_alternative<bezier_directive>(dir)) {
        render_bezier_dir(window, std::get<bezier_directive>(dir));
    }
}

void PlotRender::render_draw_dir(sf::RenderWindow& /*window*/, draw_directive& dir) {
    is_down = dir.draw;
}

void PlotRender::render_point_dir(sf::RenderWindow& window, point_directive& dir) {
    draw_point(window, &dir.dest);

    if (!is_down) {
        pos_x = dir.dest.x;
        pos_y = dir.dest.y;
        return;
    }

    draw_arrow(window, pos_x, pos_y, dir.dest.x, dir.dest.y, sf::Color::White);
    // draw_line(window, pos_x, pos_y, dir.dest.x, dir.dest.y);

    pos_x = dir.dest.x;
    pos_y = dir.dest.y;
}

void PlotRender::render_bezier_dir(sf::RenderWindow& window, bezier_directive& dir) {
    if (!is_down) {
        pos_x = dir.dest.x;
        pos_y = dir.dest.y;
        return;
    }

    draw_point(window, &dir.dest);
    draw_point(window, &dir.h1);
    draw_point(window, &dir.h2);

    draw_line(window, pos_x, pos_y, dir.h1.x, dir.h1.y, HANDLE_COLOR);
    draw_line(window, dir.dest.x, dir.dest.y, dir.h2.x, dir.h2.y, HANDLE_COLOR);

    auto start_pos = point{pos_x, pos_y};
    auto pos_a = point{pos_x, pos_y};
    point pos_b;

    for (int i = 1; i <= BEZIER_SUBDIVS; i++) {
        float t = (float)i / (float)BEZIER_SUBDIVS;
        dir.get_point(t, start_pos, pos_b);

        if (i < BEZIER_SUBDIVS) {
            draw_line(window, pos_a.x, pos_a.y, pos_b.x, pos_b.y);
        } else {
            draw_arrow(window, pos_a.x, pos_a.y, pos_b.x, pos_b.y, sf::Color::White);
        }

        pos_a.x = pos_b.x;
        pos_a.y = pos_b.y;
    }

    pos_x = dir.dest.x;
    pos_y = dir.dest.y;
}

} // namespace plotlab
