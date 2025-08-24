#include "subsystems.hpp"

#include "EZ-Template/piston.hpp"
#include "main.h"
#include "pros/misc.h"

bool is_second_jammed = false;
bool is_first_jammed = false;

bool ignorejam = false;
bool scoreflag = false;

bool scoretimerflag = false;

int scoretimer = 0;
int firstTarget = 0;
int secondTarget = 0;

Colors allianceColor = NEUTRAL;
// intake buttons
void intake() {
  firststage_antijam.set_motors(-127);

  secondstage.move(-127);

  thirdstage_antijam.set_motors(127);

  fourthstage.move(0);
}

void outtake() {
  firststage_antijam.set_motors(40);

  secondstage.move(127);

  thirdstage_antijam.set_motors(-80);

  fourthstage.move(0);
}

void scorehigh() {
  firststage_antijam.set_motors(40);

  secondstage.move(127);

  thirdstage_antijam.set_motors(127);

  fourthstage.move(-127);
  aligner.set(true);
  scraper.set(false);
}

void scoremiddle() {
  firststage_antijam.set_motors(40);

  secondstage.move(127);

  thirdstage_antijam.set_motors(127);

  fourthstage.move(35);
}

// matchload things
void matchload() {
  firststage_antijam.set_motors(-127);

  secondstage.move(-127);

  thirdstage_antijam.set_motors(127);

  fourthstage.move(0);
  aligner.set(false);
  scraper.set(true);
}

void wrongcolor() {
  firststage_antijam.set_motors(-127);

  secondstage.move(127);

  thirdstage_antijam.set_motors(-127);

  fourthstage.move(0);
  aligner.set(false);
  scraper.set(true);
}
void stopIntake(bool reset) {
  firststage_antijam.set_motors(0);

  secondstage.move(0);

  thirdstage_antijam.set_motors(0);

  fourthstage.move(0);
  if (reset) redirect.set(false);

  scraper.set(false);
}
// controls
void opcontrolintake() {
  // descore things
  scythe.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_X));
  aligner.button_toggle(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP));

  if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    intake();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    outtake();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    scoremiddle();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    scorehigh();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
    matchload();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y))
    wrongcolor();
  else {
    stopIntake(true);
  }
  // turny off aligner after score
  if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    scoreflag = true;
    scoretimer = 0;
  }

  if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_R2) and scoreflag) {
    scoreflag = false;
    scoretimerflag = true;
  }
  if (scoretimerflag) {
    scoretimer += 10;
    if (scoretimer > 1000) {
      aligner.set(false);
      scoretimerflag = false;
      scoretimer = 0;
    }
  }
}

void colorSet(Colors color, lv_obj_t* object) {
  // Set on screen elements to the corresponding color
  lv_color32_t color_use = theme_accent;
  if (color == RED)
    color_use = red;
  else if (color == BLUE)
    color_use = blue;
  lv_obj_set_style_bg_color(object, color_use, LV_PART_MAIN);
}

void setAlliance(Colors alliance) {
  allianceColor = alliance;
}