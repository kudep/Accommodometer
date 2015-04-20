
#include "Keyes_SJoys.h"
#include <inttypes.h>
int parcel;
KeyesSjoys KeSj;

void setup() {
  Serial.begin(9600);
  parcel=0;
  KeSj.init();
}


void loop() {
  if(Serial.available())
  {
    parcel=Serial.read();
    case_do(parcel);
    Serial.flush();
  }
  KeSj.task();
}



void case_do(int parcel)
{
     switch(parcel)
	 {
	case 49:

	break;
    
    default:
    Serial.println("Invalid");
   }
}



