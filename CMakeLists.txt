cmake_minimum_required(VERSION 3.20)

project(pid_sim_cpp
    VERSION 0.1.0
    DESCRIPTION "PID controller simulator for drone altitude hold"
    LANGUAGES CXX
)

# ─── C++ standard ─────────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ─── Compiler warnings ────────────────────────────────────────────────────────
add_compile_options(
    -Wall       # enable most warnings
    -Wextra     # enable extra warnings not covered by -Wall
    -Wpedantic  # enforce strict ISO C++ compliance
    -Werror     # treat warnings as errors
)

# ─── Main simulation executable ───────────────────────────────────────────────
add_executable(pid_sim
    src/main.cpp
    src/PidController.cpp
    src/AltitudeSystem.cpp
    src/Config.cpp
)

target_include_directories(pid_sim PRIVATE include/)

# ─── Ensure logs directory exists at build time ───────────────────────────────
# Define the log directory path and pass it to the compiler
target_compile_definitions(pid_sim PRIVATE
    LOG_DIR="${CMAKE_BINARY_DIR}/logs"
)

add_custom_command(TARGET pid_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
        ${CMAKE_BINARY_DIR}/logs
    COMMENT "Creating logs/ directory"
)

# Copy config.ini to the build directory after each build
add_custom_command(TARGET pid_sim POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/configs/config.ini
        ${CMAKE_BINARY_DIR}/config.ini
    COMMENT "Copying config.ini to build directory"
)

# ─── Tests ────────────────────────────────────────────────────────────────────
enable_testing()

find_package(Catch2 3 QUIET)

if(Catch2_FOUND)
    add_executable(pid_tests
        tests/pid_controller_test.cpp
        src/PidController.cpp
    )

    target_include_directories(pid_tests PRIVATE include/)
    target_link_libraries(pid_tests PRIVATE Catch2::Catch2WithMain)
    add_test(NAME PidTests COMMAND pid_tests)
else()
    message(STATUS "Catch2 not found — tests will not be built")
endif()