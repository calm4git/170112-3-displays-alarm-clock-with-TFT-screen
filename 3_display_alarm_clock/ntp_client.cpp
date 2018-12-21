
#include "datastore.h"
#include "ntp_client.h"



void NTP_Client::begin( Timecore* tc ){

timeptr=tc;
ReadSettings();

NTP.onNTPSyncEvent ([&](NTPSyncEvent_t event) {
       SyncEvent(event);
});

NTP.begin ((char*)Config.ntpServerName, DEFAULT_NTP_TIMEZONE , true, 0);
NTP.setInterval (63);

}

void NTP_Client::SyncEvent(NTPSyncEvent_t event){
          switch(event){
          case noResponse:{
               Serial.println("NTP No Response");
          }

          break;

          case timeSyncd:{
          uint32_t ts = NTP.getLastNTPSync ();
          Serial.print("Sync:");
          Serial.println (NTP.getLastNTPSync ());
          if(ts>100){
            if(timeptr!=NULL){
              timeptr->SetUTC(ts,NTP_CLOCK);
            }           
          }
        } break;

        case invalidAddress :{
          Serial.println("NTP invalid Address");
        } break;

        }
 
     
}

void NTP_Client::SetServerName( char* ServerName ){
  strncpy((char*)Config.ntpServerName,ServerName,sizeof(Config.ntpServerName) );
  NTP.setNtpServerName ( (char*)Config.ntpServerName );
}

void NTP_Client::Sync(){

  /* We send a request and will get the result may in the event */
  if(true == Config.NTPEnable ){
    Serial.println("Send NTP Request");
    NTP.getTime();
  } else {
    Serial.println("NTP not active");  
  }
  
}


void NTP_Client::Tick(){
  if(next_update>0){
    next_update--;
  } else {
   
    Sync();
    next_update = Config.SyncIntervall;
  }
  
}

char* NTP_Client::GetServerName(){
  return (char*)Config.ntpServerName;

}

void NTP_Client::SetNTPSyncEna( bool Ena){
 
  if(true==Ena){
    Config.NTPEnable=true;
  } else {
    Config.NTPEnable=false;
  }
 
 }
 
 bool NTP_Client::GetNTPSyncEna( void ){
  if(true == Config.NTPEnable){
    return true;
  } else {
     return false;
  } 
 }

 int32_t NTP_Client::GetSyncInterval( void ){
  return (  Config.SyncIntervall / 1000 ) ;
 }

 void NTP_Client::SetSyncInterval( int32_t Sync){
  Config.SyncIntervall=( Sync * 1000 );
 }

 uint32_t NTP_Client::ReadTime( bool* delayed_result){
  *delayed_result=true;
  return 0;
 }
 
void NTP_Client::SaveSettings(){
  ntp_config_t cnf;
  memcpy((void*)(&cnf),(void*)(&Config),sizeof(ntp_config_t)); 
  write_ntp_config(cnf);

}

void NTP_Client::ReadSettings(){

 ntp_config_t cnf = read_ntp_config();
 memcpy((void*)(&Config),(void*)(&cnf),sizeof(ntp_config_t));  
  

}

