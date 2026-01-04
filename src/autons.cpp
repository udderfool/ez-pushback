#include "autons.hpp"

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

void match_awp_right() {
  // score prepload in mid goal
  drive(34, 80, true);
  wait();
  // turn to mid goal
  outtake();
  turn(-44.5, turn_speed);
  wait();
  drive(2, 80, true);
  delay(1000);
  // drive back to matchload
  drive(-46.5, 90, true);
  wait();
  turn(0, turn_speed);
  // matchload
  matchload();
  wait();
  drive(-15, 40, true);
  delay(1700);
  intake();
  // score on high
  drive(29, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(3000);
  stopIntake(true);
  chassis.pid_wait_quick();
  // go back to matchload
  drive(-29.5, 40, true);
  wrongcolor();
  delay(4000);
  drive(5,70, true);
  wait();
  matchload();
  drive(-5, 50, true);
  delay(3000);
  // drive to mid goal
  intake();
  drive(13, 80, true);
  wait();
  turn(-45, turn_speed);
  wait();
  drive(44.5, 80, true);
  // score 2 in mid goal
  outtake();
  delay(2000);
  wait();
  stopIntake(true);
  // drive back to score
  drive(-46, 70, true);
  wait();
  turn(0, turn_speed);
  wait();
  // score score score
  scorehigh();
  drive(16, 70, true);
  wait();
  delay(2000);
  // go and park
  intake();
  aligner.set(false);
  drive(-10, 127, true);
  wait();
  turn(-135, turn_speed);
  wait();
  drive(25, 127, true);
  wait();
  turn(-95, turn_speed);
  wait();
  chassis.drive_set(127, 127);
  delay(1000);
  chassis.drive_set(0, 0);
}

void elims_right() {
  // score prepload in mid goal
  drive(34, 80, true);
  wait();
  // turn to mid goal
  outtake();
  turn(-44.5, turn_speed);
  wait();
  drive(2, 80, true);
  delay(1000);
  // drive back to matchload
  drive(-46.5, 90, true);
  wait();
  turn(0, turn_speed);
  // matchload
  matchload();
  wait();
  drive(-15, 40, true);
  delay(1700);
  intake();
  // score on high
  drive(29, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(3000);
  stopIntake(true);
  chassis.pid_wait_quick();
  // go back to matchload
  drive(-29.5, 40, true);
  wrongcolor();
  delay(4000);
  wait();
  matchload();
  delay(3000);
  // drive to mid goal
  intake();
  drive(13, 80, true);
  wait();
  turn(-45, turn_speed);
  wait();
  drive(44.5, 80, true);
  // score 2 in mid goal
  outtake();
  delay(2000);
  wait();
  stopIntake(true);
  // drive back to score
  drive(-46, 70, true);
  wait();
  turn(0, turn_speed);
  wait();
  // score score score
  scorehigh();
  drive(16, 70, true);
  wait();
  delay(2000);
  // push blocks to center
  intake();
  aligner.set(false);
  drive(-10, 127, true);
  wait();
  turn(-90, turn_speed);
  wait();
  drive(-12, 127, true);
  wait();
  turn(-180, turn_speed);
  wait();
  scythe.set(true);
  drive(-31, 50, true);
  wait();
}

void skills_15() {
  // drive to long goal
  drive(32, 70, true);
  aligner.set(true);
  wait();
  turn(-90, turn_speed);
  wait();
  // score preload in long goal
  drive(18, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(2000);
  stopIntake(true);
  // go back to matchload
  drive(-29.5, 40, true);
  // clear matchload tube and score 6 in long goal
  matchload();
  delay(3000);
  drive(32, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(3000);
  drive(-34, 40, true);
  matchload();
  delay(3000);
  drive(28, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(3000);
  stopIntake(true);
  chassis.pid_wait_quick();
  // grab 2 blue blocks
  drive(-17, 70, true);
  intake();
  wait();
  turn(-180, turn_speed);
  wait();
  drive(-15.5, 50, true);
  delay(3000);
  // and score in middle goal
  drive(13, 50, true);
  wait();
  turn(-135, turn_speed);
  wait();
  drive(46.5, 70, true);
  chassis.pid_wait_until(10);
  outtake();
  delay(3000);
  intake();
  // drive to other 2 blue blocks and score middle goal again
  drive(-45, 70, true);
  wait();
  turn(0, turn_speed);
  wait();
  drive(-108, 80, true);
  delay(3000);
  drive(105.5, 80, true);
  wait();
  turn(-135, turn_speed);
  wait();
  drive(45, 70, true);
  chassis.pid_wait_until(10);
  outtake();
  delay(3000);
  intake();
  // go to score high
  drive(-45, 70, true);
  wait();
  turn(0, turn_speed);
  wait();
  drive(-93, 80, true);
  wait();
  turn(-90, turn_speed);
  wait();
  drive(15, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(2000);
  //drive to matchload
  drive(-34, 40, true);
  matchload();
  delay(3000);
  // score 6 in long goal
  drive(33, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(3000);
  drive(-34, 40, true);
  matchload();
  delay(3000);
  drive(34, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  delay(3000);
  // drive to park in red park zone
  intake();
  aligner.set(false);
  drive(-10, 127, true);
  wait();
  turn(45, turn_speed);
  wait();
  drive(26, 127, true);
  wait();
  turn(8, turn_speed);
  wait();
  chassis.drive_set(127, 127);
  delay(1000);
  chassis.drive_set(0, 0);
}

void highschool_right_half_awp() {
  // drive to aim for blocks
  drive(26.5, 80, true);
  wait();
  // turn to blocks
  intake();
  turn(-90, turn_speed);
  wait();
  // score on low goal
  drive(24.5, 35, true);
  wait();
  outtake();
  delay(4000);
  intake();
  // go to high goal
  drive(-46.5, drive_speed, true);
  chassis.pid_wait_until(-30);
  chassis.pid_speed_max_set(40);
  wait();
  matchload();
  turn(-45, turn_speed);
  wait();
  // get matchload blocks
  drive(-19, 127, true);
  delay(1000);
  // score on high
  drive(27, 60, true);
  chassis.pid_wait_until(4);
  scorehigh();
  wait();
}

void highschool_left_half_awp() {
  // drive to aim for blocks
  drive(26.5, 80, true);
  wait();
  // turn to blocks
  intake();
  turn(90, turn_speed);
  wait();
  // score on mid goal
  drive(25, 35, true);
  wait();
  scoremiddle();
  delay(4000);
  intake();
  // go to high goal
  drive(-44, 90, true);
  wait();
  matchload();
  turn(45, turn_speed);
  wait();
  // get matchload blocks
  drive(-17, 65, true);
  delay(2000);
  // score on high
  drive(25, 60, true);
  chassis.pid_wait_until(5);
  scorehigh();
  wait();
}

void move_1_inch() {
  drive(1, 127, true);
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