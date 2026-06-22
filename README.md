# 🚁 pid-sim-cpp

A lightweight PID controller simulator written in modern C++ (C++20), built as a learning project and foundation for embedded drone applications.

Simulates a 1D altitude-hold system — a drone trying to reach and maintain a target altitude — using a configurable PID control loop. Outputs a CSV log you can plot to visualise the controller's behaviour.

---

## Why this exists

This project serves two purposes:

1. **Learning modern C++** — relearning C++ idioms from C++11 through C++20 (move semantics, smart pointers, `std::format`, `std::clamp`, structured bindings, etc.) after years away from the language.
2. **Drone/embedded foundation** — PID controllers are at the heart of most flight control systems. This simulator is designed to be portable, dependency-free, and easy to adapt for real embedded targets (Raspberry Pi, Pixhawk, STM32, etc.).

---

## Features

- Generic, reusable `PidController` class with configurable gains and output clamping
- Simple 1D physics simulation of a drone's vertical axis (thrust, gravity, drag)
- CSV telemetry output (time, setpoint, altitude, velocity, throttle)
- Zero external dependencies — standard C++20 only
- Clean project structure ready to extend (multi-axis, disturbances, state machines)

---

## Project structure

```
pid-sim-cpp/
├── CMakeLists.txt
├── LICENSE
├── README.md
├── include/
│   ├── PidController.hpp    # Generic PID controller
│   └── AltitudeSystem.hpp   # Simulated 1D drone physics
├── src/
│   ├── PidController.cpp
│   ├── AltitudeSystem.cpp
│   └── main.cpp             # Simulation entry point
└── logs/
    └── altitude.csv         # Generated on each run
```

---

## Requirements

- C++20 compatible compiler (GCC 12+, Clang 14+)
- CMake 3.20+

Developed and tested on Raspberry Pi 5 (Raspberry Pi OS, GCC 12).

---

## Build & run

```bash
git clone https://github.com/vincent-ayala/pid-sim-cpp.git
cd pid-sim-cpp
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
./pid_sim
```

Output is written to `logs/altitude.csv`.

---

## Tuning the PID gains

Open `src/main.cpp` and adjust the `PidConfig` struct:

```cpp
PidConfig cfg {
    .kp = 2.0,   // Proportional — how hard to push toward the target
    .ki = 0.5,   // Integral     — corrects persistent steady-state error
    .kd = 1.0,   // Derivative   — dampens oscillation and overshoot
    .output_min = -1.0,
    .output_max =  1.0,
};
```

**Suggested experiments:**

| Experiment | What to change | What to observe |
|---|---|---|
| Pure P control | Set `ki=0`, `kd=0` | Oscillation or steady-state offset |
| Add damping | Increase `kd` | Smoother approach, less overshoot |
| Fix steady-state error | Add `ki` | Drone reaches exactly the setpoint |
| Mid-flight disturbance | Add a velocity kick in `AltitudeSystem::update` at t=10s | Recovery behaviour |
| Step change | Change `setpoint` mid-simulation | Response to a new target altitude |

---

## Visualising the output

The CSV log can be plotted with Python:

```python
import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("logs/altitude.csv")
plt.plot(df["time"], df["setpoint"], "--", label="Setpoint")
plt.plot(df["time"], df["altitude"], label="Altitude")
plt.xlabel("Time (s)")
plt.ylabel("Altitude (m)")
plt.legend()
plt.grid(True)
plt.show()
```

---

## Roadmap

- [ ] Multi-axis control (pitch, roll, yaw)
- [ ] Configurable disturbance injection (wind gusts)
- [ ] Flight mode state machine (IDLE → ARMED → TAKEOFF → HOVER → LAND)
- [ ] Serial output mode for real hardware integration
- [ ] Unit tests with Google Test or Catch2

---

## License

MIT — see [LICENSE](LICENSE) for details.
