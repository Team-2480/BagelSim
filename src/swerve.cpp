#include "swerve.h"

std::array<SwerveState, 4> calculate_swerve_states(
    ChassisSpeeds chassis_speeds, std::array<float, 4>& module_headings) {
  std::array<SwerveState, 4> module_states;

  if (chassis_speeds.lateral.x == 0 && chassis_speeds.lateral.y == 0 &&
      chassis_speeds.rot == 0) {
    for (std::size_t i = 0; i < 4; i++) {
      module_states[i] = {0, module_headings[i]};
    }

    return module_states;
  }

  // We have a new center of rotation. We need to compute the matrix again.

  Eigen::Matrix<double, 4 * 2, 3> inverse_kinematics;
  if (true) {
    for (size_t i = 0; i < 4; i++) {
      inverse_kinematics.template block<2, 3>(i * 2, 0) =
          Eigen::Matrix<double, 2, 3>{{1, 0, (-modules_positions[i].y)},
                                      {0, 1, (+modules_positions[i].x)}};
    }
  }

  Eigen::Vector3d chassis_speeds_vector{
      chassis_speeds.lateral.x, chassis_speeds.lateral.y, chassis_speeds.rot};

  Eigen::Matrix<double, 4 * 2, 1> moduleStateMatrix =
      inverse_kinematics * chassis_speeds_vector;

  for (size_t i = 0; i < 4; i++) {
    double x = moduleStateMatrix(i * 2, 0);
    double y = moduleStateMatrix(i * 2 + 1, 0);

    auto speed = hypot(x, y);
    auto rotation = speed > 1e-6 ? atan2(x, y) : module_headings[i];

    module_states[i] = SwerveState{speed, rotation};
    module_headings[i] = rotation;
  }

  return module_states;
}
