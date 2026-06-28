#include <iostream>
#include <fstream>
#include <format>
#include <stdexcept>
#include "Config.hpp"
#include "PidController.hpp"
#include "AltitudeSystem.hpp"

int main(int argc, char* argv[]) {
    // Accept optional config path as command line argument
    // defaults to "config.ini" in the working directory
    const std::string config_path = (argc > 1) ? argv[1] : "config.ini";

    AppConfig cfg;
    try {
        cfg = load_config(config_path);
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    const int steps = static_cast<int>(cfg.sim.duration / cfg.sim.dt);

    PidController  pid{cfg.pid};
    AltitudeSystem drone{0.0, cfg.physics};

    std::ofstream log{LOG_DIR "/altitude.csv"};
    log << "time,setpoint,altitude,velocity,throttle\n";

    for (int step = 0; step < steps; ++step) {
        const double t        = step * cfg.sim.dt;
        const double throttle = pid.compute(cfg.sim.setpoint, drone.altitude(), cfg.sim.dt);
        drone.update(throttle, cfg.sim.dt);

        log << std::format("{:.2f},{:.2f},{:.4f},{:.4f},{:.4f}\n",
            t, cfg.sim.setpoint, drone.altitude(), drone.velocity(), throttle);
    }

    std::cout << std::format("Simulation complete — config: {}, log: {}/altitude.csv\n",
        config_path, LOG_DIR);
    return 0;
}