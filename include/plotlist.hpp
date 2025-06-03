#ifndef PLOTLIST_H
#define PLOTLIST_H

#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace plotlab {
const fs::path plot_directory = "/home/bellflwr/Documents/Blotter/Plots";

void ensure_plot_directory();

class PlotList {
  private:
    std::vector<fs::directory_entry> files;
    void draw_listbox();
    int selected_item_idx{};
    fs::directory_entry selected_item;
    fs::directory_entry current_open_project;
    bool selected_item_changed{};
    bool current_project_changed{};

  public:
    PlotList();
    void reload_files();
    void draw_window();
    auto get_open_project() -> fs::directory_entry;
    auto should_change_project() -> bool;
};
} // namespace plotlab

#endif /* PLOTLIST_H */
