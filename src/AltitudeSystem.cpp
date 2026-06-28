#include "AltitudeSystem.hpp"

AltitudeSystem::AltitudeSystem(double initial_altitude, PhysicsConfig physics)
    : altitude_{initial_altitude}
    , physics_{physics} {}

void AltitudeSystem::update(double throttle_command, double dt) {
    const double acceleration = (throttle_command * physics_.thrust_scale)
                              + gravity_
                              - (physics_.drag * velocity_);

    velocity_ += acceleration * dt;
    altitude_ += velocity_ * dt;

    if (altitude_ < 0.0) {
        altitude_ = 0.0;
        velocity_ = 0.0;
    }
}