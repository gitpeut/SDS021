// SDS021 dust sensor sleep example
// --------------------------------
//
// Jose Baars <peut@peut.org>
// 2018
//

#include <SDS021.h>

float p10,p25;
bool  status;
uint8_t result;

#define NOVARX 13
#define NOVATX 12
SDS021 nova;


void setup() {
  uint8_t fwdate[3], workmode, asleep;
  
  nova.begin(NOVARX,NOVATX);
  nova.setDebug ( false );
  	
  Serial.begin(115200);
  
  nova.sleepWork(&asleep, SDS021_WORKING, SDS021_SET );
  // If module is sleeping, then do not expect an answer. Better ask again after a short delay.
  delay(10);
  
  nova.sleepWork(&asleep, SDS021_WORKING, SDS021_ASK );
  if ( asleep != 0xFF )Serial.printf( "\n\nModule set to %s\n", asleep == SDS021_SLEEPING?"Sleep":"Work" ); 

  nova.firmwareVersion( fwdate );
  if ( fwdate[0] != 0xFF )Serial.printf( "fwdate = 20%02d, month %02d day %d\n", fwdate[0], fwdate[1], fwdate[2] ); 

  nova.workMode( &workmode, SDS021_QUERYMODE, SDS021_SET);
  if ( workmode != 0xFF )Serial.printf( "workmode set to %s\n", workmode == SDS021_REPORTMODE?"Reportmode":"Querymode" ); 

 
  delay(10000);
}

void loop() {
  
	status = nova.queryData(&p10, &p25 );
	if ( status ) {
     Serial.printf("ppm10 : %6.1f µg/m³ ppm2.5 : %6.1f µg/m³\n", p10, p25); 
	}
 
  // Put the module to sleep. Current consumption drops from ~58mA to 1.8 mA.
  // and it gives your module a longer lifespan. The manufacturer states 8000 hours,
  // which is about a year. By turning it off most of the time, this lifespan becomes 
  // multiple years, inthis case about 4 years.
  
  Serial.printf("Sleeping for 90 seconds\n");
  nova.sleepWork(&result, SDS021_SLEEPING, SDS021_SET );
  
  // If module is sleeping, then do not expect an answer. Better ask again after a short delay.
  delay( 90 * 1000 ); // Wait for 90 seconds

  // wake up the module 30 seconds before reading data, as recommended in the datasheet

  Serial.printf("Starting module 30 seconds before a reading is taken\n");
  nova.sleepWork(&result, SDS021_WORKING, SDS021_SET );
  
  delay( 30*1000); // Wait for the module to stabilize
  
}
