#ifndef PROJECT_H
#define PROJECT_H

#include <filesystem>
#include <point.hpp>

#include <string>
#include <vector>

namespace plotlab {

struct project {
    std::vector<point> points;
};

void to_json(json& j, const project& p);

void from_json(const json& j, project& p);

void read_project_from_file(std::filesystem::path path, project& dest);

} // namespace plotlab

#endif