
#include "A4988.h"
#include <inttypes.h>
#define ENEBL 52
#define DIRECT 53
#define STEP 51
#define H_TRAILER 22
#define F_TRAILER 23

A4988 driver(ENEBL,DIRECT,STEP,H_TRAILER,F_TRAILER); //Eneble, Direct, Step, Home trailer, Foredg trailer
int parcel;
int st_speed;
int st_count;
int st_count_p;
bool st_flag;
bool direct_st;
void case_do(int parcel);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  parcel=0;
  st_speed=2;
  st_count=2000;
  st_count_p=0;
  st_flag=false;
  driver.init();
  driver.home();
  driver.return_back();
}


void loop() {
  if(Serial.available())
  {
    parcel=Serial.read();
    case_do(parcel);
    Serial.flush();
  }
  step_do();
}


void step_do(void)
{
  if(st_flag)
  {
    if(st_count_p<st_count)
	{
     driver.go_to(st_speed);
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
    
    
    Serial.print("COORD =");
    Serial.println(driver.get_coord(),DEC);
    Serial.print("BIAS =");
    Serial.println(driver.get_bias(),DEC);
    Serial.print("COUNT =");
    Serial.println(driver.get_count(),DEC);
    driver.home();
    
    break;
    
    case 76:  //"L" -Less
    st_count-=10;
    Serial.print("Number of cycle =");
    Serial.println(st_count,DEC);
    driver.return_back();
    break;
    
    case 82:  //"R" -Reverse
		Serial.println("Reverse");
		driver.reverse();
		Serial.println(driver.get_direct());
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



