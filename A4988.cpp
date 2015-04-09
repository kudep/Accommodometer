/* ver 0.7 
03.04.2015
*/
#include "Arduino.h"
#include "A4988.h"


A4988::A4988(int En_n, int Dir, int Stp):en_n(En_n), dir(Dir), stp(Stp)
{}

void A4988::init()
{
	pinMode(en_n,OUTPUT);
	pinMode(dir,OUTPUT);
	pinMode(stp,OUTPUT);
	digitalWrite(en_n, HIGH);
	digitalWrite(dir, LOW);
	digitalWrite(stp, LOW);
}

void A4988::forward()
{
direct=FORWARD;
comptable(direct);
}

void A4988::back()
{
direct=BACK;
comptable(direct);
}

void A4988::reverse()
{
direct=!direct;
comptable(direct);
}

void A4988::step(int count)
{
digitalWrite(en_n, LOW);

for(int i=0;i<count; i++)
{
 digitalWrite(stp, HIGH);
 delayMicroseconds(DELAY_PULSE_US);
 digitalWrite(stp, LOW);
 delayMicroseconds(DELAY_BETWEEN_US);
 cnt++;
}

digitalWrite(en_n, HIGH);
}

int A4988::get_count(void)
{
  return cnt;
}

bool A4988::get_direct(void)
{
  return direct;
}

void A4988::clr_count(void)
{
 cnt=0; 
}

void A4988::comptable( bool direct)
{
 if(direct) 
 digitalWrite(dir, HIGH);
 else
 digitalWrite(dir, LOW);
 
}

