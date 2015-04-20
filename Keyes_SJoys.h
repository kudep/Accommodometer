
#ifndef KESES_SJOYS_H
#define KESES_SJOYS_H

#define VOLT_VR_MIN 700
#define VOLT_VR_MAX 800
#define VOLT_SW_MIN 10
#define VOLT_SW_MAX 250
#define STEP_DR 2
#define BREAK_INIT 512
#define DIF_TIME_CLEAR_BREAK 200

#include "Configuration.h"
#include "Arduino.h"
#include "A4988.h"

class KeyesSjoys
{
public:
	KeyesSjoys(void);

	void init(void);
	void task(void);
private:
	int read_pin_vr(int pin);
	bool read_pin_sw(void);
	A4988 driver;

};

#endif