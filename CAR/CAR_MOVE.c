
#include "CAR_MOVE.h"


void Forward()
{
	pwm_config(0, 255, 255);
	pwm_config(1, 255, 0);
	pwm_config(2, 255, 0);
	pwm_config(3, 255, 255);
}

void Backward()
{
	pwm_config(0, 255, 0);
	pwm_config(1, 255, 255);
	pwm_config(2, 255, 255);
	pwm_config(3, 255, 0);

}

void turnleft()
{
	pwm_config(0, 255, 0);
	pwm_config(1, 255, 255);
	pwm_config(2, 255, 0);
	pwm_config(3, 255, 255);
	
}

void turnright()
{
	pwm_config(0, 255, 255);
	pwm_config(1, 255, 0);
	pwm_config(2, 255, 255);
	pwm_config(3, 255, 0);
	
}
