#include "app.hpp"
#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics/RenderWindow.hpp>

const int WIDTH = 1200;
const int HEIGHT = 600;
const int REFRESH = 60;

auto main() -> int {
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Plotlab");
    window.setFramerateLimit(REFRESH);

    (void)ImGui::SFML::Init(window);
    ImGuiIO& im_io = ImGui::GetIO();

    plotlab::App app(&window, &im_io);

    app.start();

    ImGui::SFML::Shutdown();
}