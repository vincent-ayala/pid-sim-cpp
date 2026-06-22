#pragma once

// A generic PID controller.
// Modern C++ highlights: const correctness, nodiscard, structured config via struct
struct PidConfig {
    double kp{1.0};   // Proportional gain
    double ki{0.0};   // Integral gain
    double kd{0.0};   // Derivative gain
    double output_min{-1.0};  // Clamp output to avoid saturation
    double output_max{1.0};
};

class PidController {
public:
    explicit PidController(PidConfig config);

    // Call this every loop iteration.
    // dt = time delta in seconds since last call
    [[nodiscard]] double compute(double setpoint, double measured, double dt);

    void reset(); // Clear integral and previous error (e.g. on mode switch)

    [[nodiscard]] const PidConfig& config() const { return config_; }

private:
    PidConfig config_;
    double integral_{0.0};
    double prev_error_{0.0};
    bool first_run_{true};
};