
#include "Keyes_SJoys.h"
KeyesSjoys::KeyesSjoys()
{}


void KeyesSjoys::init(void)
{

	driver.init(EN_DR, DRCT_DR, STP_DR, H_TR, F_TR); //Eneble, Direct, Step, Home trailer, Foredg trailer
	driver.home();
	driver.return_back();
}

void KeyesSjoys::task(bool sclct)
{
	static uint16_t brx_pos_c = BREAK_INIT;
	static uint16_t brx_neg_c = BREAK_INIT;
	static uint16_t br = BREAK_INIT;


	static uint32_t st_time = millis();

	if ((millis() - st_time)>DIF_TIME_CLEAR_BREAK) brx_pos_c = brx_neg_c = BREAK_INIT;




	if (sclct)
	{

		static uint8_t reg = 0;
		reg = reg << 1;
		reg |= (uint8_t)read_pin_sw();

		if (reg == 1)
		{
			brx_pos_c = brx_neg_c = BREAK_INIT;
			Serial.print("COORD =");
			Serial.print(transl(driver.get_coord(), STEP_RANGE, 30), DEC);
			Serial.println(" cm");
			Serial.print("BIAS =");
			Serial.print(transl(driver.get_bias(), STEP_RANGE, 30), DEC);
			Serial.println("cm");
			Serial.print("COUNT =");
			Serial.print(transl(driver.get_count(), 1000, 1), DEC);
			Serial.println(" * 1k step");
			Serial.print("Name file is ");
			Serial.println(glob_str);
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
	else
	{
		decoder_control();
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

int  KeyesSjoys::transl(int var, int max, int unity)
{
	return var / max*unity;
}

int KeyesSjoys::filtrated_encoder(void)
{
	return  ((uint8_t)analogRead(ENCOD)) >> SHIFT_FOR_FILTER;
}

void  KeyesSjoys::decoder_control(void)
{
	static uint8_t inf = 0;
	static uint8_t sup = 0;
	static uint8_t pos = 0;

	inf = filtrated_encoder() - 1;
	sup = inf + 2;
	pos = transl(driver.get_coord(), STEP_RANGE, MAX_VAL_ENC);
	while ((inf > pos) && (sup < pos))
	{
		if (inf > pos)
		{
			driver.forward();
			for (int i; i < (STEP_RANGE / (MAX_VAL_ENC*STEP_DR)); i++)
				driver.go_to(STEP_DR);
		}
		else
		{
			driver.back();
			for (int i; i < (STEP_RANGE / (MAX_VAL_ENC*STEP_DR)); i++)
				driver.go_to(STEP_DR);
		}
		inf = filtrated_encoder() - 1;
		sup = inf + 2;
		pos = transl(driver.get_coord(), STEP_RANGE, MAX_VAL_ENC);

	}

}