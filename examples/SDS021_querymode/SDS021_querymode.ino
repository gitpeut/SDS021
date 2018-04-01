// SDS021 dust sensor example
// -----------------------------
//
// Jose Baars <peut@peut.org>
// 2018
//

#include <SDS021.h>

float p10,p25;
bool  status;

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
  // datasheet advises 3 seconds as minimum poll time.
	delay(3000);
}
