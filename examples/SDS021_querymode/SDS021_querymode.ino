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
char  line[80];

void setup() {
  uint8_t fwdate[3], workmode, asleep;
  
  nova.begin(NOVARX,NOVATX);
  nova.setDebug ( false );
  	
  Serial.begin(115200);
  
  nova.sleepWork(&asleep, SDS021_WORKING, SDS021_SET );
  // If module is sleeping, then do not expect an answer. Better ask again after a short delay.
  delay(10);
  
  nova.sleepWork(&asleep, SDS021_WORKING, SDS021_ASK );
  if ( asleep != 0xFF ){
    Serial.println("");Serial.print( "Module set to "); Serial.println( asleep == SDS021_SLEEPING?"Sleep":"Work" ); 
  }

  nova.firmwareVersion( fwdate );
  if ( fwdate[0] != 0xFF ){ 
    Serial.print( "fwdate = 20"); Serial.print( fwdate[0]); Serial.print( " month ");Serial.print( fwdate[1]); Serial.print("day "); Serial.println( fwdate[2] ); 
  }

  nova.workMode( &workmode, SDS021_QUERYMODE, SDS021_SET);
  if ( workmode != 0xFF ){
    Serial.print( "workmode set to "); Serial.println( workmode == SDS021_REPORTMODE?"Reportmode":"Querymode" ); 
  }

 
  delay(10000);
}

void loop() {
	status = nova.queryData(&p10, &p25 );
	if ( status ) {
     Serial.print("ppm10 : "); Serial.print( p10,1 ); Serial.print("µg/m³ ppm2.5 : "); Serial.println( p25,1); 
	}
  // datasheet advises 3 seconds as minimum poll time.
	delay(3000);
}
