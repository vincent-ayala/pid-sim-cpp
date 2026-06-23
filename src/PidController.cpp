#include "PidController.hpp"
#include <algorithm> // std::clamp — a very handy C++17 addition

PidController::PidController(PidConfig config)
    : config_{config} {}

double PidController::compute(double setpoint, double measured, double dt) {
    const double error = setpoint - measured;

    // --- Proportional term ---
    const double p = config_.kp * error;

    // --- Integral term (accumulated error over time) ---
    integral_ += error * dt;
    const double i = config_.ki * integral_;

    // --- Derivative term (rate of change of error) ---
    // Skip on first run — we have no previous error to compare against
    double d = 0.0;
    if (!first_run_) {
        d = config_.kd * (error - prev_error_) / dt;
    }
    first_run_ = false;
    prev_error_ = error;

    // Sum and clamp to output limits
    const double output = p + i + d;
    return std::clamp(output, config_.output_min, config_.output_max);
}

void PidController::reset() {
    integral_   = 0.0;
    prev_error_ = 0.0;
    first_run_  = true;
}