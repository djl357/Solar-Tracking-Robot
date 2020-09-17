#ifndef SERVO_H_
#define SERVO_H_

#include "MK64F12.h"

void initialize_flextimer(void);
void move_servo(double angle);

#endif