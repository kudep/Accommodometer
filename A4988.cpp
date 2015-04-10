
#include "Arduino.h"
#include "A4988.h"


A4988::A4988(int En_n, int Dir, int Stp, int H_trlr, int F_trlr):en_n(En_n), dir(Dir), stp(Stp), h_trlr(H_trlr), f_trlr(F_trlr),direct(FORWARD),cnt(0),bias(0),coord(0)
{}

void A4988::init()
{
	pinMode(en_n,OUTPUT);
	pinMode(dir,OUTPUT);
	pinMode(stp,OUTPUT);
	pinMode(h_trlr,INPUT);
	pinMode(f_trlr,INPUT);
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
 if(direct){
	 bias--;
	 coord--;
 }
 else {
	bias++;
	coord++;
}

}

digitalWrite(en_n, HIGH);
}

int A4988::get_count(void)
{
  return cnt;
}

long int A4988::get_bias(void)
{
  return bias;
}
long int A4988::get_coord(void)
{
  return coord;
}

bool A4988::get_direct(void)
{
  return direct;
}

void A4988::clr_count(void)
{
 cnt=0; 
}

void A4988::clr_bias(void)
{
 cnt=0; 
}

void A4988::clr_coord(void)
{
 coord=0; 
}

void A4988::comptable( bool direct)
{
 if(direct) 
 digitalWrite(dir, HIGH);
 else
 digitalWrite(dir, LOW);
 
}

bool A4988::h_trailer(void)
{
  static uint8_t reg=0;
  reg=reg<<1;
  reg|=(uint8_t)digitalRead(h_trlr);
  
  if(reg==1)
  {
    reverse();
    clr_coord();
    return true;
  }
    return false;
}

bool A4988::f_trailer(void)
{
  static uint8_t reg=0;
  reg=reg<<1;
  reg|=(uint8_t)digitalRead(f_trlr);
  
  if(reg==1)
  {
    reverse();
    return true;
  }
    return false;
}

void A4988::home(void)
{
  if(!digitalRead(h_trlr))
  {
  back();
 while(!h_trailer()) step(STEPS);
  }
  coord=0;
}

void A4988::return_back(void)
{
  if(bias>0)
  {
  back();
  while(bias) step(STEPS);
  }
  else if(bias<0)
  {
  forward();
  while(bias) step(STEPS);
  }
}

void A4988::go_to(int st, bool _direct)
{
  
  static bool flag=false;
  static bool stop_dir=FORWARD;
  if (!(flag &&(stop_dir==_direct)))
  {
   flag=false;
  if(direct!=_direct)
  reverse();
  for(int i=0;st>i;i++)
 {
   step(STEPS);
   if(h_trailer())
   {
     flag=true;
     stop_dir=BACK;
     break;
   }
   if(f_trailer())
   {
     flag=true;
     stop_dir=FORWARD;
     break;
   }
 }
  }
}
  
  

