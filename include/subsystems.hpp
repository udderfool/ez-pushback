#pragma once

#include "EZ-Template/api.hpp"
#include "EZ-Template/piston.hpp"
#include "api.h"
#include "pros/adi.h"
#include "subsystems.hpp"
#include "antijam.hpp"

extern Drive chassis;

inline pros::Motor firststage (18);
inline pros::Motor secondstage (19);
inline pros::Motor thirdstage (13);
inline pros::Motor fourthstage (10);
inline ez::Piston aligner('A');
inline ez::Piston scraper('B');
inline ez::Piston redirect('Z');
inline ez::Piston scythe('C');
//intake
void intake();
void outtake();
void scorehigh();
void scoremiddle();
void stopIntake(bool reset);
void opcontrolintake();

void matchload();
void wrongcolor();

//colors
enum Colors { BLUE = 0, NEUTRAL = 1, RED = 2 };
extern Colors allianceColor;

void colorSet(Colors color, lv_obj_t* object);
void setAlliance(Colors alliance);


//antijam
inline const int firststage_wait_time = 40;  // The time in ms that the intake has to have 0 velocity before deciding a jam is happening
inline const int firststage_time = 150;      // The time to outtake for to unjam
inline const int firststage_speed = 20;      // Minimum speed that allows antijam to trigger (out of 127)

inline antijam firststage_antijam(&firststage, firststage_wait_time, firststage_time, firststage_speed);  // Create the antijam

inline const int thirdstage_wait_time = 40;  // The time in ms that the intake has to have 0 velocity before deciding a jam is happening
inline const int thirdstage_time = 150;      // The time to outtake for to unjam
inline const int thirdstage_speed = 20;      // Minimum speed that allows antijam to trigger (out of 127)

inline antijam thirdstage_antijam(&thirdstage, thirdstage_wait_time, thirdstage_time, thirdstage_speed);  // Create the antij