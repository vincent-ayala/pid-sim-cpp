#include <iostream>
#include <fstream>
#include <format> // C++20 — like printf but type-safe
#include "PidController.hpp"
#include "AltitudeSystem.hpp"

int main() {
    // --- Configuration ---
    constexpr double setpoint    = 10.0;  // Target altitude in meters
    constexpr double dt          = 0.05;  // Simulation timestep: 50ms (20 Hz)
    constexpr double duration    = 20.0;  // Simulate 20 seconds
    constexpr int    steps       = static_cast<int>(duration / dt);

    // --- PID tuning (experiment with these!) ---
    PidConfig cfg {
        .kp = 2.0,
        .ki = 0.5,
        .kd = 1.0,
        .output_min = -1.0,
        .output_max = 1.0,
    };

    PidController pid{cfg};
    AltitudeSystem drone{0.0}; // starts on the ground

    // --- CSV log ---
    std::ofstream log{"logs/altitude.csv"};
    log << "time,setpoint,altitude,velocity,throttle\n";

    // --- Simulation loop ---
    for (int step = 0; step < steps; ++step) {
        const double t = step * dt;

        const double throttle = pid.compute(setpoint, drone.altitude(), dt);
        drone.update(throttle, dt);

        // C++20 std::format — much cleaner than printf or manual stream formatting
        log << std::format("{:.2f},{:.2f},{:.4f},{:.4f},{:.4f}\n",
            t, setpoint, drone.altitude(), drone.velocity(), throttle);
    }

    std::cout << "Simulation complete. Check logs/altitude.csv\n";
    return 0;
}