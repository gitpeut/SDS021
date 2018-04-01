// SDS021 dust sensor workPeriod example
// --------------------------------
//
// Jose Baars <peut@peut.org>
// 2018
//
// Module consumes about 16 mA when sleeping between periods. This is much more
// than the 1.8 mA it consumes when actually put to sleep with sleepWork.
// Module gives one and the same value after starting to work until the
// next period starts. No need in collecting the same data over and over again.
// Code is way too complicated, but for demo purposes it's fine.

#include <SDS021.h>

#define NOVARX 14
#define NOVATX 12

SDS021 nova;

float p10,p25;
bool  status;
uint8_t result;
int   count=20;



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

  nova.workMode( &workmode, SDS021_REPORTMODE, SDS021_SET);
  if ( workmode != 0xFF )Serial.printf( "workmode set to %s\n", workmode == SDS021_REPORTMODE?"Reportmode":"Querymode" ); 

  // According to the datasheet, the workperiod setting is presered over boots.
  // When playing, make sure to reset it afterwards, like here
  // set workperiod every 2 minutes.
  
  nova.workPeriod( &result, 2, SDS021_SET);
  if ( workmode != 0xFF ){
    Serial.printf( "workperiod set to %d\n", result ); 
  }else{
    Serial.printf( "workperiod not set %d\n", result ); 
  }

}

void loop() {
  count--; 

  if(count > 0){ 

    for ( status = true; status;){
      
      nova.sleepWork(&result, SDS021_SLEEPING, SDS021_ASK );
      if ( result != 0xFF )Serial.printf( "Module is %s\n", result == SDS021_SLEEPING?"asleep":"active" ); 
      if ( result == SDS021_SLEEPING ) break; 

      if ( count < 19 ){
        Serial.printf("Wait for module to stabilize\n");
        delay(30*1000);
      }
          
      status = nova.queryData(&p10, &p25 );
	    if ( status ) {
         Serial.printf("ppm10 : %6.1f µg/m³ ppm2.5 : %6.1f µg/m³\n", p10, p25); 
         delay(4 * 1000);
	    }

      // Module gives one and the same value after starting to work until the
      // next period starts. Check yourself by uncommenting the queryData
      // statements. No need in collecting the same data over and over again.
      
      for ( result = SDS021_WORKING; result == SDS021_WORKING; ){
        Serial.printf("Waiting for module to fall asleep\n"); 
         //status = nova.queryData(&p10, &p25 );
         //if ( status ) {
         //  Serial.printf("ppm10 : %6.1f µg/m³ ppm2.5 : %6.1f µg/m³\n", p10, p25); 
         //}
        delay( 5 * 1000 );
        nova.sleepWork(&result, SDS021_SLEEPING, SDS021_ASK );
      }
      
      Serial.printf("Module is asleep\n"); 
      break;  
    }
    Serial.printf("Waiting for module to wake up (count remaining %d)\n", count); 
    delay(10 * 1000);
}
    
  // after count loops, reset the workperiod to default, and delay forever.
    
  if ( count == 0 ){
        Serial.printf("Set workPeriod to default zero, i.e. continuous\n");
        nova.workPeriod( &result, 0, SDS021_SET);
        Serial.printf("Set module to sleep\n");
        nova.sleepWork( &result, SDS021_SLEEPING, SDS021_SET);          
  }
      
Serial.printf("Doing nothing\n");
delay(1000);

  
}
