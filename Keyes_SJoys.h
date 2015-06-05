
#ifndef KESES_SJOYS_H
#define KESES_SJOYS_H

#define VOLT_VR_MIN 700
#define VOLT_VR_MAX 850
#define VOLT_SW_MIN 10
#define VOLT_SW_MAX 250
//STEP_DR = 2  is optimal
#define STEP_DR 2
#define BREAK_INIT 2048
#define DIF_TIME_CLEAR_BREAK 200

#define SHIFT_FOR_FILTER 2
#define MAX_VAL_ENC 128
#define STEP_RANGE 19200

#define STEP_ENCOD 500

#include "Configuration.h"
#include "Arduino.h"
#include "A4988.h"


class KeyesSjoys
{
public:
	KeyesSjoys(void);

	void init(volatile int *encoder_div);
	void task(void);
	void irp_encoder(void);
	void value(void);
	char *glob_str;
private:

	int read_pin_vr(int pin);
	bool read_pin_sw(void);

	int transl(int var, int max, int unity);

	int flag_forw_go(void);
	int flag_back_go(void);
	int reading_signal(int pin);

	A4988 driver;
	volatile int *p_encoder_div;

};

#endif
