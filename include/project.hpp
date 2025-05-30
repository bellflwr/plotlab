#ifndef PROJECT_H
#define PROJECT_H

#include "directives.hpp"
#include <filesystem>
#include <point.hpp>

#include <vector>

namespace plotlab {

struct project {
    int canvas_width;
    int canvas_height;
    std::vector<directive> directives;

    auto string() -> std::string;
};

void to_json(json& j, const project& p);

void from_json(const json& j, project& p);

void read_project_from_file(const std::filesystem::path& path, project& dest);

} // namespace plotlab

#endif