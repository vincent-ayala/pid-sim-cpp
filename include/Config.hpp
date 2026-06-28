#pragma once
#include <string>
#include "PidController.hpp"

struct SimConfig {
    double setpoint{10.0};
    double dt{0.05};
    double duration{20.0};
};

struct PhysicsConfig {
    double thrust_scale{15.0};
    double drag{0.4};
};

struct AppConfig {
    SimConfig    sim;
    PidConfig    pid;
    PhysicsConfig physics;
};

// Loads config from a .ini file.
// Throws std::runtime_error if the file cannot be opened.
[[nodiscard]] AppConfig load_config(const std::string& path);