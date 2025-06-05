#ifndef APP_H
#define APP_H

#include "imgui.h"
#include "plot_editor.hpp"
#include "plotlist.hpp"
#include "plotrender.hpp"
#include "pointlist.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <optional>
namespace plotlab {

class App {
  private:
    sf::RenderWindow* window;
    ImGuiIO* im_io;
    sf::Clock deltaClock{};

    plotlab::PlotList plotlist;
    plotlab::PointList pointlist;
    plotlab::PlotRender plotrender;
    plotlab::PlotEditor ploteditor;
    plotlab::project project;
    std::optional<std::filesystem::path> current_project_file;

    void update();
    void handle_events();
    void handle_event(const std::optional<sf::Event>& event);

  public:
    App(sf::RenderWindow* window, ImGuiIO* im_io);
    void start();
};

} // namespace plotlab

#endif /* APP_H */
