#include "app.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include <SFML/Graphics/RenderWindow.hpp>

const float FONT_SIZE = 18.F;

namespace plotlab {

App::App(sf::RenderWindow* window, ImGuiIO* im_io) : window(window), im_io(im_io) {
    im_io->Fonts->Clear();
    im_io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    im_io->Fonts->AddFontFromFileTTF("assets/Inter_18pt-Regular.ttf", FONT_SIZE);
    (void)ImGui::SFML::UpdateFontTexture();
}

void App::start() {
    while (window->isOpen()) {
        update();
    }

    ImGui::SFML::Shutdown();
}

void App::update() {
    handle_events();

    ImGui::SFML::Update(*window, deltaClock.restart());

    ImGui::DockSpaceOverViewport(0, NULL, ImGuiDockNodeFlags_PassthruCentralNode);

    plotlist.draw_window();
    if (plotlist.should_change_project()) {
        current_project_file = plotlist.get_open_project().path();
        plotlab::read_project_from_file(plotlist.get_open_project(), project);
    }
    pointlist.draw_window(project);
    ImGui::ShowMetricsWindow();

    window->clear();
    plotrender.render(*window, project);
    ImGui::SFML::Render(*window);
    window->display();
}

void App::handle_events() {
    while (const auto event = window->pollEvent()) {
        ImGui::SFML::ProcessEvent(*window, *event);
        handle_event(event);
    }
}

void App::handle_event(const std::optional<sf::Event>& event) {
    if (event->is<sf::Event::Closed>()) {
        window->close();
    }

    else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
        sf::FloatRect visibleArea({0.F, 0.F}, sf::Vector2f(resized->size));

        window->setView(sf::View(visibleArea));
    }

    else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
            ploteditor.handle_event(mouseButtonPressed, project);
        }
    }

    else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
            ploteditor.handle_event(mouseButtonReleased, project);
        }
    }

    else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
        ploteditor.handle_event(mouseMoved, project);
    }

    else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::S && keyPressed->control) {
            if (current_project_file) {
                plotlab::write_project_to_file(*current_project_file, project);
            }
        }
    }
};

} // namespace plotlab
