#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

// Trim leading and trailing whitespace from a string
static std::string trim(const std::string& s) {
    const auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    const auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

AppConfig load_config(const std::string& path) {
    std::ifstream file{path};
    if (!file.is_open()) {
        throw std::runtime_error{"Cannot open config file: " + path};
    }

    AppConfig cfg{};
    std::string line;
    std::string section;

    while (std::getline(file, line)) {
        line = trim(line);

        // Skip blank lines and comments
        if (line.empty() || line[0] == ';' || line[0] == '#') continue;

        // Section header e.g. [pid]
        if (line.front() == '[' && line.back() == ']') {
            section = trim(line.substr(1, line.size() - 2));
            continue;
        }

        // Key = value pair
        const auto delim = line.find('=');
        if (delim == std::string::npos) continue;

        const std::string key   = trim(line.substr(0, delim));
        const std::string value = trim(line.substr(delim + 1));
        const double      val   = std::stod(value);

        if (section == "simulation") {
            if      (key == "setpoint") cfg.sim.setpoint = val;
            else if (key == "dt")       cfg.sim.dt       = val;
            else if (key == "duration") cfg.sim.duration = val;
        }
        else if (section == "pid") {
            if      (key == "kp")           cfg.pid.kp           = val;
            else if (key == "ki")           cfg.pid.ki           = val;
            else if (key == "kd")           cfg.pid.kd           = val;
            else if (key == "output_min")   cfg.pid.output_min   = val;
            else if (key == "output_max")   cfg.pid.output_max   = val;
        }
        else if (section == "physics") {
            if      (key == "thrust_scale") cfg.physics.thrust_scale = val;
            else if (key == "drag")         cfg.physics.drag         = val;
        }
    }

    return cfg;
}