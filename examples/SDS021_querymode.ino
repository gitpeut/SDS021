// SDS021 dust sensor example
// -----------------------------
//
// Jose Baars <peut@peut.org>
// 2018
//

#include <SDS011.h>

float p10,p25;
bool  status;

#define NOVARX 13
#define NOVATX 12
SDS011 nova;


void setup() {
  uint8_t fwdate[3], workmode, asleep;
  
  nova.begin(NOVARX,NOVATX);
  nova.setDebug ( true );
  	
  Serial.begin(115200);
  
  nova.workMode( &workmode, SDS011_QUERYMODE, SDS011_SET);
  nova.firmwareVersion( fwdate );
  nova.sleepWork(&asleep, SDS011_WORKING, SDS011_SET );

  if ( fwdate[0] != 0xFF )Serial.printf( "\nfwdate = 20%02d, month %02d day %d\n", fwdate[0], fwdate[1], fwdate[2] ); 
  
  Serial.printf( "Workmode = %s, Currently, module is %s\n", 
    workmode==0xFF?"N/A":(workmode==SDS011_REPORTMODE?"Reportmode":"Querymode") ,
    asleep==0xFF?"N/A":(asleep==SDS011_SLEEPING?"sleeping":"working")  ); 
 
  delay(10000);
}

void loop() {
	status = nova.queryData(&p10, &p25 );
	if ( status ) {
		Serial.println("P2.5: "+String(p25));
		Serial.println("P10:  "+String(p10));
	}
	delay(3000);
}
