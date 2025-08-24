/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

class antijam {
 public:
  antijam(pros::Motor *input_motor, int input_wait_time = 30, int input_outtake_time = 100, int min_speed = 20, int input_stuck_retry_timer = 0);
  antijam(pros::MotorGroup *input_motors, int input_wait_time = 30, int input_outtake_time = 100, int min_speed = 20, int input_stuck_retry_timer = 0);

  pros::Motor *motor;
  pros::MotorGroup *motors;

  void set_motors(int input);

  void wait_time_set(int input);
  int wait_time_get();
  void outtake_time_set(int input);
  int outtake_time_get();
  void min_activate_speed_set(int input);
  int min_activate_speed_get();
  void stuck_retry_time_set(int input);
  int stuck_try_time_get();

  double get_velocity();

  int real_speed();

  void enable();
  void disable();
  bool enabled();

  void stick_enable();
  void stick_disable();
  bool stick_enabled();

  void reset_timers();

  void antijam_function();
  pros::Task antijam_task;

 private:
  int jam_counter = 0;
  int stuck_retry_timer = 0;
  bool is_stuck = false;
  int last_input_speed = 0;
  void set_motors_raw(int input);
  int actual_speed = 0;
  bool is_enabled = true;
  int wait_time = 0;
  int outtake_time = 0;
  int min_speed = 0;
  bool is_jammed = false;
  bool can_reset = false;
};