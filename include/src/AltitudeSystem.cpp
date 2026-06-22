#include "AltitudeSystem.hpp"

AltitudeSystem::AltitudeSystem(double initial_altitude)
    : altitude_{initial_altitude} {}

void AltitudeSystem::update(double throttle_command, double dt) {
    // Net acceleration = thrust + gravity - drag
    const double acceleration = (throttle_command * thrust_scale_)
                              + gravity_
                              - (drag_ * velocity_);

    velocity_ += acceleration * dt;
    altitude_ += velocity_ * dt;

    // Ground clamp — can't go below 0
    if (altitude_ < 0.0) {
        altitude_ = 0.0;
        velocity_ = 0.0;
    }
}