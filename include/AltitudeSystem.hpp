#pragma once

// Simulates a simple 1D vertical system (drone altitude)
// This is a toy physics model, not real aerodynamics
class AltitudeSystem {
public:
    explicit AltitudeSystem(double initial_altitude = 0.0);

    // Apply a throttle command [-1, 1] and advance physics by dt seconds
    void update(double throttle_command, double dt);

    [[nodiscard]] double altitude() const { return altitude_; }
    [[nodiscard]] double velocity() const { return velocity_; }

private:
    double altitude_;
    double velocity_{0.0};

    // Physics constants (tweak these to make the sim harder/easier to control)
    static constexpr double gravity_{-9.81};       // m/s²
    static constexpr double thrust_scale_{15.0};   // how much throttle affects acceleration
    static constexpr double drag_{0.4};            // dampens velocity (air resistance)
};