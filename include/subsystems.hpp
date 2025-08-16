#pragma once

#include "EZ-Template/api.hpp"
#include "EZ-Template/piston.hpp"
#include "api.h"
#include "pros/adi.h"
#include "subsystems.hpp"

extern Drive chassis;

inline pros::Motor firststage (18);
inline pros::Motor secondstage (19);
inline pros::Motor thirdstage (13);
inline pros::Motor fourthstage (10);
inline ez::Piston aligner('A');
inline ez::Piston scraper('B');
inline ez::Piston redirect('Z');

//intake
void intake();
void outtake();
void scorehigh();
void scoremiddle();
void stopIntake(bool reset);
void opcontrolintake();

void matchload();

void antijamFirstTask();
void antijamSecondTask();

//colors
enum Colors { BLUE = 0, NEUTRAL = 1, RED = 2 };
extern Colors allianceColor;

void colorSet(Colors color, lv_obj_t* object);
void setAlliance(Colors alliance);