/* ver 0.7 
03.04.2015
*/
#include "A4988.h"
#include <inttypes.h>

A4988 driver(52,53,51); //Eneble, Direct, Step
int parcel;
int st_speed;
int st_count;
int st_count_p;
bool st_flag;
void case_do(int parcel);
void step_do(void);
void trailer(void);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(22,INPUT);
  pinMode(23,INPUT);
  parcel=0;
  st_speed=2;
  st_count=2000;
  st_count_p=0;
  st_flag=false;
  driver.init();
}


void loop() {
  if(Serial.available())
  {
    parcel=Serial.read();
    case_do(parcel);
    Serial.flush();
  }
  step_do();
  trailer();
}


void step_do(void)
{
  if(st_flag)
  {
    if(st_count_p<st_count)
    {
      driver.step(st_speed);
      st_count_p++;
    }
    else
    {
      st_count_p=0;
      st_flag=false;
    }
    
  }
}



void case_do(int parcel)
{
     switch(parcel){
    case 71:  //"G" -Go
    st_flag=true;
    Serial.println("Go");
    break;
    
    case 83:  //"S" -Stop
    st_flag=false;
    Serial.println("Stop");
    break;
    
    case 70:  //"F" - Faster
    st_speed*=2;
    Serial.print("Step of cycle =");
    Serial.println(st_speed,DEC);
    break;
    
    case 68:  //"D" -Delay
    st_speed/=2;
    Serial.print("Step of cycle =");
    Serial.println(st_speed,DEC);
    break;
    
    case 77:  //"M" - More
    st_count+=10;
    Serial.print("Number of cycle =");
    Serial.println(st_count,DEC);
    break;
    
    case 76:  //"L" -Less
    st_count-=10;
    Serial.print("Number of cycle =");
    Serial.println(st_count,DEC);
    break;
    
    case 82:  //"R" -Reverse
    driver.reverse();
    Serial.println("Reverse");
    break;
    
    case 84:  //"T" - Transsmit
    delay(3000);
    st_count=Serial.parseInt();
    Serial.print("Number of cycle =");
    Serial.println(st_count,DEC);
    break;
    
    default:
    Serial.println("Invalid");
   }
}


void trailer(void)
{
  static uint8_t reg1=0;
  reg1=reg1<<1;
  reg1|=(uint8_t)digitalRead(22);
  
  static uint8_t reg2=0;
  reg2=reg2<<1;
  reg2|=(uint8_t)digitalRead(23);
  
  if((reg1==1)||(reg2==1))
  {
    driver.reverse();
    Serial.print("Number of step =");
    Serial.println(driver.get_count(),DEC);
    if(driver.get_direct())
    Serial.println("Forward");
    else
    Serial.println("Back");
    driver.clr_count();
    
  }
}
