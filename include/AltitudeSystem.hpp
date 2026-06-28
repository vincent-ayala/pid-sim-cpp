#pragma once

// Simulates a simple 1D vertical system (drone altitude)
// This is a toy physics model, not real aerodynamics
#include "Config.hpp"

class AltitudeSystem {
public:
    explicit AltitudeSystem(double initial_altitude = 0.0,
                            PhysicsConfig physics = {});

    void update(double throttle_command, double dt);

    [[nodiscard]] double altitude() const { return altitude_; }
    [[nodiscard]] double velocity() const { return velocity_; }

private:
    double altitude_;
    double velocity_{0.0};
    PhysicsConfig physics_;

    static constexpr double gravity_{-9.81};
};