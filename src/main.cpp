#include "imgui-SFML.h"
#include "imgui.h"
#include "plotlist.hpp"
#include "project.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

auto main() -> int {
    sf::RenderWindow window(sf::VideoMode({640, 480}), "ImGui + SFML = <3");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    ImGuiIO& io = ImGui::GetIO();

    io.Fonts->Clear();
    io.Fonts->AddFontFromFileTTF("assets/Inter_18pt-Regular.ttf", 18.f);
    ImGui::SFML::UpdateFontTexture(); // important call: updates font texture

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock deltaClock;

    plotlab::PlotList plotlist;
    plotlab::project project;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::ShowDemoWindow();

        plotlist.draw_window();
        if (plotlist.should_change_project()) {
            std::cout << plotlist.get_open_project().path().string() << '\n';
            plotlab::read_project_from_file(plotlist.get_open_project(),
                                            project);
        }

        window.clear();
        window.draw(shape);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}