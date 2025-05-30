#include "pointlist.hpp"
#include "directives.hpp"
#include "imgui.h"
#include "project.hpp"

namespace plotlab {
PointList::PointList() = default;

void PointList::draw_window(project& proj) {
    if (ImGui::Begin("Points")) {

        int id{};
        for (auto& directive : proj.directives) {
            auto* dir_v = &directive;
            draw_directive(dir_v, id);
        }

        if (ImGui::BeginPopupContextItem("Select Directive")) {
            if (ImGui::Selectable("Draw")) {
                directive new_dir = plotlab::draw_directive{};
                proj.directives.push_back(new_dir);
            }
            if (ImGui::Selectable("Point")) {
                directive new_dir = point_directive{};
                proj.directives.push_back(new_dir);
            }
            if (ImGui::Selectable("Bezier")) {
                directive new_dir = bezier_directive{};
                proj.directives.push_back(new_dir);
            }

            ImGui::EndPopup();
        }

        if (ImGui::Button("Add")) {
            ImGui::OpenPopup("Select Directive");
        }
    }
    ImGui::End();
}

void PointList::draw_directive(directive* dir, int& id) {

    if (std::holds_alternative<point_directive>(*dir)) {
        draw_directive(std::get_if<point_directive>(dir), id);
    } else if (std::holds_alternative<struct draw_directive>(*dir)) {
        draw_directive(std::get_if<struct draw_directive>(dir), id);
    } else if (std::holds_alternative<bezier_directive>(*dir)) {
        draw_directive(std::get_if<bezier_directive>(dir), id);
    }
}

void PointList::draw_directive(point_directive* dir, int& id) {
    ImGui::PushID(id++);

    if (ImGui::CollapsingHeader("Point", ImGuiTreeNodeFlags_None)) {

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->dest.x, 0, 0);
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->dest.y, 0, 0);
        ImGui::PopID();
    }
    ImGui::PopID();
}

void PointList::draw_directive(struct draw_directive* dir, int& id) {
    ImGui::PushID(id++);

    if (ImGui::CollapsingHeader("Draw", ImGuiTreeNodeFlags_None)) {

        ImGui::PushID(id++);
        ImGui::Checkbox("", &dir->draw);
        ImGui::PopID();
    }
    ImGui::PopID();
}

void PointList::draw_directive(bezier_directive* dir, int& id) {
    ImGui::PushID(id++);

    if (ImGui::CollapsingHeader("Bezier", ImGuiTreeNodeFlags_None)) {

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->h1.x, 0, 0);
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->h1.y, 0, 0);
        ImGui::PopID();

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->h2.x, 0, 0);
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->h2.y, 0, 0);
        ImGui::PopID();

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->dest.x, 0, 0);
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(id++);
        ImGui::InputInt("", &dir->dest.y, 0, 0);
        ImGui::PopID();
    }
    ImGui::PopID();
}

} // namespace plotlab