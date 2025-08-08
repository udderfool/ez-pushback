#include "subsystems.hpp"

#include "EZ-Template/piston.hpp"
#include "main.h"
#include "pros/misc.h"

bool is_second_jammed = false;
bool is_first_jammed = false;
bool ignorejam = false;
int firstTarget = 0;
int secondTarget = 0;

Colors allianceColor = NEUTRAL;

void intake() {
  if (!is_first_jammed) {
    firststage.move(-127);
    firstTarget = -127;
    ignorejam = false;
  }
  secondstage.move(-127);

  if (!is_second_jammed) {
    thirdstage.move(127);
    secondTarget = 127;
    ignorejam = false;
  }
  fourthstage.move(0);
}

void outtake() {
  if (!is_first_jammed) {
    firststage.move(45);
    firstTarget = 45;
    ignorejam = false;
  }
  secondstage.move(100);

  if (!is_second_jammed) {
    thirdstage.move(-60);
    secondTarget = -60;
    ignorejam = true;
  }
  fourthstage.move(0);
}

void scorehigh() {
  if (!is_first_jammed) {
    firststage.move(30);
    firstTarget = 30;
    ignorejam = false;
  }
  secondstage.move(127);

  if (!is_second_jammed) {
    thirdstage.move(127);
    secondTarget = 127;
    ignorejam = false;
  }
  fourthstage.move(-127);
  redirect.set(true);
}

void scoremiddle() {
  if (!is_first_jammed) {
    firststage.move(30);
    firstTarget = 30;
    ignorejam = false;
  }
  secondstage.move(127);

  if (!is_second_jammed) {
    thirdstage.move(127);
    secondTarget = 127;
    ignorejam = false;
  }

  fourthstage.move(15);
  redirect.set(true);
}

void stopIntake(bool reset) {
  if (!is_first_jammed) {
    firststage.move(0);
    firstTarget = 0;
    ignorejam = false;
  }
  secondstage.move(0);

  if (!is_second_jammed) {
    thirdstage.move(0);
    secondTarget = 0;
    ignorejam = false;
  }

  fourthstage.move(0);
  if (reset) redirect.set(false);
}

void opcontrolintake() {
  if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
    intake();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
    outtake();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
    scoremiddle();
  else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
    scorehigh();
  else {
    stopIntake(true);
  }
}

void antijamFirstTask() {
  int jam_counter = 0;
  const int outtake_time = 150;
  const int wait_time = 200;
  while (true) {
    // Run intake full power in opposite direction for outtake_time ms when jammed, then
    // set intake back to normal
    if (is_first_jammed) {
      firststage.move(127 * util::sgn(firstTarget));
      jam_counter += util::DELAY_TIME;
      if (jam_counter > outtake_time) {
        is_first_jammed = false;
        jam_counter = 0;
        firststage.move(firstTarget);
      }
    }

    // Detect a jam if velocity is 0 for wait_time ms
    else if (!ignorejam && abs(firstTarget) >= 20 && firststage.get_actual_velocity() <= 80) {
      jam_counter += util::DELAY_TIME;
      if (jam_counter > wait_time) {
        jam_counter = 0;
        is_first_jammed = true;
      }
    }

    // Reset jam_counter when button is released
    if (firstTarget <= 20) {
      jam_counter = 0;
    }

    pros::delay(util::DELAY_TIME);
  }
}

void antijamSecondTask() {
  int jam_counter = 0;
  const int outtake_time = 150;
  const int wait_time = 200;
  while (true) {
    // Run intake full power in opposite direction for outtake_time ms when jammed, then
    // set intake back to normal
    if (is_second_jammed) {
      thirdstage.move(-127 * util::sgn(secondTarget));
      jam_counter += util::DELAY_TIME;
      if (jam_counter > outtake_time) {
        is_second_jammed = false;
        jam_counter = 0;
        thirdstage.move(secondTarget);
      }
    }

    // Detect a jam if velocity is 0 for wait_time ms
    else if (!ignorejam && abs(secondTarget) >= 20 && thirdstage.get_actual_velocity() <= 80) {
      jam_counter += util::DELAY_TIME;
      if (jam_counter > wait_time) {
        jam_counter = 0;
        is_second_jammed = true;
      }
    }

    // Reset jam_counter when button is released
    if (secondTarget <= 20) {
      jam_counter = 0;
    }

    pros::delay(util::DELAY_TIME);
  }
}

void colorSet(Colors color, lv_obj_t* object) {
	// Set on screen elements to the corresponding color
	lv_color32_t color_use = theme_accent;
	if(color == RED)
		color_use = red;
	else if(color == BLUE)
		color_use = blue;
	lv_obj_set_style_bg_color(object, color_use, LV_PART_MAIN);
}

void setAlliance(Colors alliance) {
	allianceColor = alliance;
}