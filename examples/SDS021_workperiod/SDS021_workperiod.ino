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
  if ( asleep != 0xFF ){
    Serial.println("");Serial.print( "Module set to "); Serial.println( asleep == SDS021_SLEEPING?"Sleep":"Work" ); 
  }

  nova.workMode( &workmode, SDS021_REPORTMODE, SDS021_SET);
  if ( workmode != 0xFF ){
    Serial.print( "workmode set to "); Serial.println( workmode == SDS021_REPORTMODE?"Reportmode":"Querymode" ); 
  }

  // According to the datasheet, the workperiod setting is presered over boots.
  // When playing, make sure to reset it afterwards, like here
  // set workperiod every 2 minutes.
  
  nova.workPeriod( &result, 2, SDS021_SET);
  if ( workmode != 0xFF ){
    Serial.print( "workperiod set to "); Serial.println( result ); 
  }else{
    Serial.print( "workperiod not set to "); Serial.println( result ); 
  }

}

void loop() {
  count--; 

  if(count > 0){ 

    for ( status = true; status;){
      
      nova.sleepWork(&result, SDS021_SLEEPING, SDS021_ASK );
      if ( result != 0xFF ){
        Serial.println("");Serial.print( "Module is "); Serial.println( result == SDS021_SLEEPING?"asleep":"active" ); 
      }
      if ( result == SDS021_SLEEPING ) break; 

      if ( count < 19 ){
        Serial.println("Wait for module to stabilize");
        delay(30*1000);
      }
          
      status = nova.queryData(&p10, &p25 );
	    if ( status ) {
         Serial.print("ppm10 : "); Serial.print( p10,1 ); Serial.print(" µg/m³ ppm2.5 : "); Serial.print( p25,1); Serial.println(" µg/m³"); 
         delay(4 * 1000);
	    }

      // Module gives one and the same value after starting to work until the
      // next period starts. Check yourself by uncommenting the queryData
      // statements. No need in collecting the same data over and over again.
      
      for ( result = SDS021_WORKING; result == SDS021_WORKING; ){
        Serial.println("Waiting for module to fall asleep");         
        delay( 5 * 1000 );
        nova.sleepWork(&result, SDS021_SLEEPING, SDS021_ASK );
      }
      
      Serial.println("Module is asleep"); 
      break;  
    }
    Serial.print("Waiting for module to wake up (count remaining "); Serial.println( count ); 
    delay(10 * 1000);
}
    
  // after count loops, reset the workperiod to default, and delay forever.
    
  if ( count == 0 ){
        Serial.println("Set workPeriod to default zero, i.e. continuous");
        nova.workPeriod( &result, 0, SDS021_SET);
        Serial.println("Set module to sleep");
        nova.sleepWork( &result, SDS021_SLEEPING, SDS021_SET);          
  }
      
Serial.println("Doing nothing");
delay(1000);

  
}
