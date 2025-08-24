/*
This Source Code Form is subject to the terms of the Mozilla Public
License, v. 2.0. If a copy of the MPL was not distributed with this
file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "antijam.hpp"

antijam::antijam(pros::Motor *input_motor, int input_wait_time, int input_outtake_time, int input_min_speed, int input_stuck_retry_timer)
    : antijam_task([this] { this->antijam_function(); }) {
  motor = input_motor;

  wait_time_set(input_wait_time);
  outtake_time_set(input_outtake_time);
  min_activate_speed_set(input_min_speed);
  stuck_retry_time_set(input_stuck_retry_timer);

  antijam_task.resume();
}

antijam::antijam(pros::MotorGroup *input_motors, int input_wait_time, int input_outtake_time, int input_min_speed, int input_stuck_retry_timer)
    : antijam_task([this] { this->antijam_function(); }) {
  motors = input_motors;

  wait_time_set(input_wait_time);
  outtake_time_set(input_outtake_time);
  min_activate_speed_set(input_min_speed);
  stuck_retry_time_set(input_stuck_retry_timer);

  antijam_task.resume();
}

void antijam::set_motors_raw(int input) {
  if (pros::millis() < 1500) return;

  if (motor != NULL)
    motor->move(input);
  else
    motors->move(input);
}

void antijam::set_motors(int input) {
  actual_speed = input;
  if (input != last_input_speed) {
    set_motors_raw(input);
    can_reset = true;
  }
  last_input_speed = input;
}

double antijam::get_velocity() {
  if (motor != NULL)
    return motor->get_actual_velocity();
  else
    return motors->get_actual_velocity();
}

void antijam::enable() { is_enabled = true; }
bool antijam::enabled() { return is_enabled; }
void antijam::disable() {
  is_enabled = false;
  set_motors(real_speed());
}

void antijam::stick_enable() { is_stuck = true; }
void antijam::stick_disable() { is_stuck = false; }
bool antijam::stick_enabled() { return is_stuck; }

void antijam::wait_time_set(int input) { wait_time = input; }
void antijam::outtake_time_set(int input) { outtake_time = input; }
void antijam::min_activate_speed_set(int input) { min_speed = input; }
void antijam::stuck_retry_time_set(int input) { stuck_retry_timer = input; }

int antijam::wait_time_get() { return wait_time; }
int antijam::outtake_time_get() { return outtake_time; }
int antijam::min_activate_speed_get() { return min_speed; }
int antijam::stuck_try_time_get() { return stuck_retry_timer; }

int antijam::real_speed() { return actual_speed; }

void antijam::reset_timers() {
  if (!can_reset) return;
  can_reset = false;

  is_jammed = false;
  jam_counter = 0;
}

void antijam::antijam_function() {
  pros::delay(2000);  // Let IMU and other stuff initialize before starting this task

  while (true) {
    // Only run antijam when enabled
    if (enabled()) {
      // Run intake full power in opposite direction for outtake_time_get()ms when jammed, then set motor back to normal
      if (is_jammed) {
        jam_counter += ez::util::DELAY_TIME;  // Increment timer

        // Set these variables based on if the code is going to "stick" or not
        int motor_speed = stick_enabled() ? 0 : -127 * ez::util::sgn(real_speed());
        int timeout = stick_enabled() ? stuck_try_time_get() : outtake_time_get();

        set_motors_raw(motor_speed);
        if (jam_counter > timeout) {
          is_jammed = false;
          jam_counter = 0;
          set_motors_raw(real_speed());
        }
      }

      // Detect a jam if velocity is 0 for wait_time_get()ms
      else if (abs(real_speed()) >= min_activate_speed_get() && get_velocity() == 0.0) {
        jam_counter += ez::util::DELAY_TIME;  // Increment timer
        if (jam_counter > wait_time_get()) {
          jam_counter = 0;
          is_jammed = true;
        }
      }

      // Reset jam_counter when button is released
      if (abs(real_speed()) <= min_activate_speed_get()) {
        jam_counter = 0;
      }
    } else {
      // Reset jam counter when antijam is disabled
      jam_counter = 0;
    }

    reset_timers();

    // printf("is enabled: %i      is jammed: %i   real speed: %i   min speed: %i   vel: %f      counter: %i\n", (int)enabled(), (int)is_jammed, real_speed(), min_activate_speed_get(), get_velocity(), jam_counter);

    pros::delay(ez::util::DELAY_TIME);
  }
}