#include "project.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace plotlab {

void to_json(json& j, const project& p) {
    j = json{
        {"points", p.points},
    };
}

void from_json(const json& j, project& p) {
    j.at("points").get_to(p.points);
}

void read_project_from_file(std::filesystem::path path, project& dest) {
    std::ifstream file(path);
    json data;
    file >> data;

    data.get_to(dest);

    std::cout << data << '\n';
};

} // namespace plotlab
