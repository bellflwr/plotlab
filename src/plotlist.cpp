#include "plotlist.hpp"
#include "imgui.h"
#include <filesystem>

namespace fs = std::filesystem;
using plotlab::PlotList;

void plotlab::ensure_plot_directory() {
    if (!fs::is_directory(plotlab::plot_directory) || !fs::exists(plotlab::plot_directory)) {
        fs::create_directories(plotlab::plot_directory);
    }
}

PlotList::PlotList() {
    reload_files();
}

void PlotList::reload_files() {
    files.clear();

    for (auto const& dir_entry : fs::directory_iterator{plot_directory}) {
        if (dir_entry.is_regular_file()) {
            std::string ext = dir_entry.path().extension().string();
            if (ext == ".bplot") {
                files.push_back(dir_entry);
            }
        }
    }
}

void PlotList::draw_listbox() {
    ImGui::BeginListBox("plot_table", ImVec2(-FLT_MIN, (float)files.size() * ImGui::GetTextLineHeightWithSpacing()));

    int highlighted_item_idx = -1;

    for (int i = 0; i < (int)files.size(); i++) {
        auto dir_entry = files.at(i);

        bool is_selected = (selected_item_idx == i);
        ImGuiSelectableFlags flags = (highlighted_item_idx == i) ? ImGuiSelectableFlags_Highlight : 0;

        if (ImGui::Selectable(dir_entry.path().filename().c_str(), is_selected, flags)) {
            selected_item_idx = i;
            selected_item = dir_entry;
        }

        if (is_selected) {
            ImGui::SetItemDefaultFocus();
        }
    }
    ImGui::EndListBox();
}

void PlotList::draw_window() {
    ImGui::Begin("Plots");

    draw_listbox();

    if (ImGui::Button("Open")) {
        if (selected_item != current_open_project) {
            current_open_project = selected_item;
            current_project_changed = true;
        }
    }

    ImGui::End();
}

auto PlotList::get_open_project() -> fs::directory_entry {
    return current_open_project;
}

// Anytime this function is called, it MUST act upon the change of selection
auto PlotList::should_change_project() -> bool {
    bool should_change = current_project_changed;
    current_project_changed = false;
    return should_change;
}