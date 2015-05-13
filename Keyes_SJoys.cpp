
#include "Keyes_SJoys.h"
KeyesSjoys::KeyesSjoys()
{}


void KeyesSjoys::init(volatile int *p_encoder_div)
{

	driver.init(EN_DR, DRCT_DR, STP_DR, H_TR, F_TR); //Eneble, Direct, Step, Home trailer, Foredg trailer
	driver.home();
	driver.return_back();
	this->p_encoder_div = p_encoder_div;
}

void KeyesSjoys::task(void)
{
	static uint16_t brx_pos_c = BREAK_INIT;
	static uint16_t brx_neg_c = BREAK_INIT;
	static uint16_t br = BREAK_INIT;
	int cirk;


	//value();

	static uint32_t st_time = millis();

	if ((millis() - st_time)>DIF_TIME_CLEAR_BREAK) brx_pos_c = brx_neg_c = BREAK_INIT;


		static uint8_t reg = 0;
		reg = reg << 1;
		reg |= (uint8_t)read_pin_sw();

		if (reg == 1)
		{
			brx_pos_c = brx_neg_c = BREAK_INIT;
			Serial.print("COORD =");
			Serial.print(transl(driver.get_coord(), STEP_RANGE, 230), DEC);
			Serial.println(" mm");
			Serial.print("BIAS =");
			Serial.print(transl(driver.get_bias(), STEP_RANGE, 230), DEC);
			Serial.println("mm");
			Serial.print("COUNT =");
			Serial.print(transl(driver.get_count(), 1000, 1), DEC);
			Serial.println(" * 1k step");
			Serial.print("Name file is ");
			Serial.println(glob_str);
			return;
		}
                cirk=flag_forw_go();
		while((driver.get_count()<cirk))
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
                   if(driver.h_trailer()||driver.f_trailer())
                  {
                    (*p_encoder_div)=0;
                    return;
                  }
		}
                
                cirk=flag_back_go();
                while((driver.get_count()<cirk))
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
                   if(driver.h_trailer()||driver.f_trailer())
                   {
                    (*p_encoder_div)=0;
                    return;
                   }
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
	return (var*unity / max);
}


int KeyesSjoys::flag_forw_go(void)
{
	if ((*p_encoder_div))
	{
		if ((*p_encoder_div) > 0)
		{
			(*p_encoder_div)--;
			return STEP_ENCOD+driver.get_count();
		}
		else return 0;
	}
	else if (read_pin_vr(VRX) > 0) return driver.get_count()+1;
	else return 0;

}
int KeyesSjoys::flag_back_go(void)
{
	if ((*p_encoder_div))
	{
		if ((*p_encoder_div) < 0)
		{
			(*p_encoder_div)++;
			return driver.get_count()+STEP_ENCOD;
		}
		else return 0;
	}
	else if (read_pin_vr(VRX) < 0) return driver.get_count()+1;
	else return 0;

}
void KeyesSjoys::value(void)
{

	static  uint32_t prevMillis = 0;
	if ((millis() - prevMillis)>1000)
	{
		prevMillis = millis();
		Serial.println((*p_encoder_div));
	}
}
