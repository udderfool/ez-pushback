#include "EZ-Template/drive/drive.hpp"
#include "main.h"
#include "okapi/api/units/QAngle.hpp"
#include "okapi/api/units/QLength.hpp"
#include "pros/rtos.hpp"
#include "subsystems.hpp"

/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////

// These are out of 127
const int drive_speed = 90;
const int turn_speed = 90;
const int swing_speed = 110;

double target_theta = 0;

void wait() {
  chassis.pid_wait();
}

void delay(int millis) {
			pros::delay(millis);
}

void drive(okapi::QLength distance, int speed, bool slew) {
  chassis.pid_drive_set(distance, speed, slew);
}

void drive(double distance, int speed, bool slew) {
  chassis.pid_drive_set(distance, speed, slew);
}

void turn(okapi::QAngle degrees, int speed) {
  chassis.pid_turn_set(degrees, speed);
}

void turn(double degrees, int speed) {
  chassis.pid_turn_set(degrees, speed);
}

///
// Constants
///
void default_constants() {
  // P, I, D, and Start I
  chassis.pid_drive_constants_set(20.0, 0.0, 100.0);         // Fwd/rev constants, used for odom and non odom motions
  chassis.pid_heading_constants_set(11.0, 0.0, 20.0);        // Holds the robot straight while going forward without odom
  chassis.pid_turn_constants_set(3.0, 0.05, 20.0, 15.0);     // Turn in place constants
  chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing constants
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular control for odom motions
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control for boomerang motions

  // Exit conditions
  chassis.pid_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew constants
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // The amount that turns are prioritized over driving in odom motions
  // - if you have tracking wheels, you can run this higher.  1.0 is the max
  chassis.odom_turn_bias_set(0.9);

  chassis.odom_look_ahead_set(7_in);           // This is how far ahead in the path the robot looks at
  chassis.odom_boomerang_distance_set(16_in);  // This sets the maximum distance away from target that the carrot point can be
  chassis.odom_boomerang_dlead_set(0.625);     // This handles how aggressive the end of boomerang motions are

  chassis.pid_angle_behavior_set(ez::shortest);  // Changes the default behavior for turning, this defaults it to the shortest path there
}

void move_forward(int in) {
  drive(in, drive_speed, in > 20 ? true : false);
}

void highschool_right_half_awp() {
//drive to aim for blocks
  drive(23, 80, true);
wait();
//turn to blocks
intake();
  turn(270, turn_speed);
wait();
//score on low goal
  drive(26, 35, true);
delay(4000);
outtake();
delay(1000);
intake();
//go to high goal
  drive(-47.5, 90, true);
wait();
stopIntake(false);
  turn(-45, turn_speed);
wait();
//score on high
  redirect.set(true);
delay(250);
  drive(16, 50, true);
delay(500);
scorehigh();
  wait();

}

void highschool_left_half_awp() {
//drive to aim for blocks
  drive(24.5, 80, true);
wait();
//turn to blocks
intake();
  turn(-270, turn_speed);
wait();
//score on low goal
  drive(23.5, 35, true);
  redirect.set(true);
delay(4000);
scoremiddle();
delay(1000);
intake();
//go to high goal
  drive(-45.5, 90, true);
wait();
stopIntake(false);
  turn(45, turn_speed);
wait();
//score on high
  redirect.set(true);
delay(250);
  drive(16, 50, true);
delay(500);
scorehigh();
  wait();

}

  /* Drive and collect cluster of blocks
  driveSet(28_in, 40, true);
intake();
    chassis.pid_wait();
  
  driveSet(-17_in,80, true);
    chassis.pid_wait();

  turnSet(90_deg, TURN_SPEED);
    chassis.pid_wait();
  
  driveSet(29_in, DRIVE_SPEED, true);
    chassis.pid_wait();
  
  turnSet(345_deg, TURN_SPEED);
    chassis.pid_wait();
stopIntake();
  driveSet(11_in, 60, true);
    chassis.pid_wait();
scorehigh();
*/