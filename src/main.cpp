#include "imgui-SFML.h"
#include "imgui.h"
#include "plotlist.hpp"
#include "plotrender.hpp"
#include "pointlist.hpp"
#include "project.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <cstddef>
#include <optional>

const int WIDTH = 1200;
const int HEIGHT = 600;
const int REFRESH = 60;
const float FONT_SIZE = 18.F;

auto main() -> int {
    sf::RenderWindow window(sf::VideoMode({WIDTH, HEIGHT}), "Plotlab");

    window.setFramerateLimit(REFRESH);
    (void)ImGui::SFML::Init(window);

    ImGuiIO& im_io = ImGui::GetIO();

    im_io.Fonts->Clear();
    im_io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    im_io.Fonts->AddFontFromFileTTF("assets/Inter_18pt-Regular.ttf", FONT_SIZE);
    (void)ImGui::SFML::UpdateFontTexture();

    sf::Clock deltaClock;

    plotlab::PlotList plotlist;
    plotlab::PointList pointlist;
    plotlab::PlotRender plotrender;
    plotlab::project project;
    std::optional<std::filesystem::path> current_project_file{};

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            // catch the resize events
            if (const auto* resized = event->getIf<sf::Event::Resized>()) {
                // update the view to the new size of the window
                sf::FloatRect visibleArea({0.F, 0.F}, sf::Vector2f(resized->size));

                window.setView(sf::View(visibleArea));
            }

            if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    plotrender.handle_event(mouseButtonPressed, project);
                }
            }
            if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
                if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                    plotrender.handle_event(mouseButtonReleased, project);
                }
            }
            if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                plotrender.handle_event(mouseMoved, project);
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::S && keyPressed->control) {
                    if (current_project_file) {
                        plotlab::write_project_to_file(*current_project_file, project);
                    }
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

        plotlist.draw_window();
        if (plotlist.should_change_project()) {
            current_project_file = plotlist.get_open_project().path();
            plotlab::read_project_from_file(plotlist.get_open_project(), project);
        }
        pointlist.draw_window(project);
        ImGui::ShowMetricsWindow();

        window.clear();
        plotrender.render(window, project);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
}