#include "project.hpp"
#include "directives.hpp"
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

using json = nlohmann::json;

namespace {
void string_replace_all(std::string& str, const std::string& from,
                        const std::string& to) {
    if (from.empty()) {
        return;
    }

    size_t start_pos = 0;

    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like
                                  // replacing 'x' with 'yx'
    }
}
} // namespace

namespace plotlab {

void to_json(json& j, const project& p) {
    j = json{
        {"canvas_width", p.canvas_width},
        {"canvas_height", p.canvas_height},
        {"directives", p.directives},
    };
}

void from_json(const json& j, project& p) {
    j.at("canvas_height").get_to(p.canvas_height);
    j.at("canvas_width").get_to(p.canvas_width);
    j.at("directives").get_to(p.directives);
}

void read_project_from_file(const std::filesystem::path& path, project& dest) {
    std::ifstream file(path);
    json data;
    file >> data;

    data.get_to(dest);

    // std::cout << data << '\n';
};

auto project::string() -> std::string {
    std::ostringstream buf;
    buf << "project (\n";
    buf << "    canvas_width: " << canvas_width << ",\n";
    buf << "    canvas_height: " << canvas_height << ",\n";
    buf << "    directives: [\n";

    for (auto dir : directives) {
        auto dir_str = directive_to_string(dir);
        string_replace_all(dir_str, "\n", "\n        ");
        buf << "        " << dir_str << ",\n";
    }
    buf << "    ]\n";

    buf << ")";

    return buf.str();
}

} // namespace plotlab
