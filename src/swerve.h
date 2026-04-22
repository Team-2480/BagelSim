#pragma once
#include <Eigen/QR>
#include <array>
#include <cmath>

#include "Eigen/Core"
#include "config.h"
#include "raylib.h"

struct SwerveState {
  double speed;
  double rot;
};
struct ChassisSpeeds {
  Vector2 lateral;  // meters per second
  double rot;       // rad per second
};

constexpr std::array<Vector2, 4> modules_positions = {
    Vector2{Constants::WHEEL_BASE / 2, Constants::TRACK_WIDTH / 2},
    Vector2{Constants::WHEEL_BASE / 2, -Constants::TRACK_WIDTH / 2},
    Vector2{-Constants::WHEEL_BASE / 2, Constants::TRACK_WIDTH / 2},
    Vector2{-Constants::WHEEL_BASE / 2, -Constants::TRACK_WIDTH / 2}};

std::array<SwerveState, 4> calculate_swerve_states(
    ChassisSpeeds chassis_speeds, std::array<float, 4>& module_headings);
