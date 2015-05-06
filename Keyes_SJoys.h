
#ifndef KESES_SJOYS_H
#define KESES_SJOYS_H

#define VOLT_VR_MIN 600
#define VOLT_VR_MAX 900
#define VOLT_SW_MIN 10
#define VOLT_SW_MAX 250
#define STEP_DR 2
#define BREAK_INIT 512
#define DIF_TIME_CLEAR_BREAK 200

#define SHIFT_FOR_FILTER 2
#define MAX_VAL_ENC 128
#define STEP_RANGE 19200

#include "Configuration.h"
#include "Arduino.h"
#include "A4988.h"

class KeyesSjoys
{
public:
	KeyesSjoys(void);

	void init(void);
	void task(bool sclct);
private:
	int read_pin_vr(int pin);
	bool read_pin_sw(void);
	int transl(int var, int max, int unity);
	int filtrated_encoder(void);
	void decoder_control(void);
	A4988 driver;

};

#endif