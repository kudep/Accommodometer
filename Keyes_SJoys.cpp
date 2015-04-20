
#include "Keyes_SJoys.h"
KeyesSjoys::KeyesSjoys()
{}


void KeyesSjoys::init(void)
{

	driver.init(EN_DR, DRCT_DR, STP_DR, H_TR, F_TR); //Eneble, Direct, Step, Home trailer, Foredg trailer
	driver.home();
	driver.return_back();
}

void KeyesSjoys::task(void)
{
	static uint16_t brx_pos_c = BREAK_INIT;
	static uint16_t brx_neg_c = BREAK_INIT;
	static uint16_t br = BREAK_INIT;

	static uint32_t st_time = millis();

	if ((millis() - st_time)>DIF_TIME_CLEAR_BREAK) brx_pos_c = brx_neg_c = BREAK_INIT;



	static uint8_t reg = 0;

	reg = reg << 1;
	reg |= (uint8_t)read_pin_sw();

	if (reg == 1)
	{
		brx_pos_c = brx_neg_c = BREAK_INIT;
		Serial.print("COORD =");
		Serial.println(driver.get_coord(), DEC);
		Serial.print("BIAS =");
		Serial.println(driver.get_bias(), DEC);
		Serial.print("COUNT =");
		Serial.println(driver.get_count(), DEC);
		return;
	}

	if (read_pin_vr(VRX) > 0)
	{
		if (br) br--;
		else
		{
			st_time = millis();
			brx_neg_c = BREAK_INIT;
			if (brx_pos_c) brx_pos_c--;
			br = brx_pos_c;
			driver.forward();
			driver.go_to(STEP_DR);
		}
		return;
	}
	else if (read_pin_vr(VRX) < 0)
	{

		if (br) br--;
		else
		{
			st_time = millis();
			brx_pos_c = BREAK_INIT;
			if (brx_neg_c) brx_neg_c--;
			br = brx_neg_c;
			driver.back();
			driver.go_to(STEP_DR);
		}
		return;

	}

}

int KeyesSjoys::read_pin_vr(int pin)
{
	if (VOLT_VR_MIN > analogRead(pin)) return -1;
	else if (VOLT_VR_MAX < analogRead(pin)) return 1;
	return 0;
}

bool KeyesSjoys::read_pin_sw(void)
{
	if (VOLT_SW_MIN > analogRead(SW)) return true;
	return false;
}