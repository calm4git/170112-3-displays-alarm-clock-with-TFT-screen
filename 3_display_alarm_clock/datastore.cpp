#include <EEPROM.h>
#include <CRC32.h>
#include "datastore.h"







void datastoresetup() {
  /* We emulate 512 byte here */
  EEPROM.begin(4096);

}

void write_ntp_config(ntp_config_t c){
  CRC32 crc;
  uint8_t* data=(uint8_t*)&c;
  for(uint32_t i=1024;i<sizeof(ntp_config_t)+1024;i++){
      EEPROM.write(i,*data);
      crc.update(*data);
      data++;
  }
  /* the last thing to do is to calculate the crc32 for the data and write it to the end */
  uint32_t data_crc = crc.finalize(); 
  uint8_t* bytedata = (uint8_t*)&data_crc;
  for(uint32_t z=sizeof(ntp_config_t)+1024;z<sizeof(ntp_config_t)+1024+sizeof(data_crc);z++){
    EEPROM.write(z,*bytedata);
    bytedata++;
  } 
  EEPROM.commit();
  
}

ntp_config_t read_ntp_config( void ){
  ntp_config_t retval;
  CRC32 crc;
  uint8_t* ret_ptr=(uint8_t*)&retval;
  uint8_t data;
  for(uint32_t i=1024;i<sizeof(ntp_config_t)+1024;i++){
      data = EEPROM.read(i);
      crc.update(data);
      *ret_ptr=data;
      ret_ptr++;   
  }
  
  /* Next is to read the crc32*/
  uint32_t data_crc = crc.finalize(); 
  uint32_t saved_crc=0;
  uint8_t* bytedata = (uint8_t*)&saved_crc;
  for(uint32_t z=sizeof(ntp_config_t)+1024;z<sizeof(ntp_config_t)+1024+sizeof(data_crc);z++){
    *bytedata=EEPROM.read(z);
    bytedata++;
  } 
  if(saved_crc!=data_crc){
    Serial.printf("NTP CONF: CRC SAVED: %u <> CRC DATA %u\n\rDo Failsave",saved_crc,data_crc);
    bzero((void*)&retval,sizeof( credentials_t ));
    write_ntp_config(retval);
  } else {
    
  }
  return retval;
}



void write_timecoreconf(timecoreconf_t c){
  uint8_t* data=(uint8_t*)&c;
  CRC32 crc;
  for(uint32_t i=512;i<sizeof(timecoreconf_t)+512;i++){
      EEPROM.write(i,*data);
      crc.update(*data);
      data++;
  }
 /* the last thing to do is to calculate the crc32 for the data and write it to the end */
  uint32_t data_crc = crc.finalize(); 
  uint8_t* bytedata = (uint8_t*)&data_crc;
  for(uint32_t z=sizeof(timecoreconf_t)+512;z<sizeof(timecoreconf_t)+512+sizeof(data_crc);z++){
    EEPROM.write(z,*bytedata);
    bytedata++;
  } 
  EEPROM.commit();
  
}

timecoreconf_t read_timecoreconf( void ){
  timecoreconf_t retval;
  CRC32 crc;
  uint8_t* ret_ptr=(uint8_t*)&retval;
  uint8_t data;
  for(uint32_t i=512;i<sizeof(timecoreconf_t)+512;i++){
      data = EEPROM.read(i);
      crc.update(data);
      *ret_ptr=data;
      ret_ptr++; 
  }
  /* Next is to read the crc32*/
  uint32_t data_crc = crc.finalize(); 
  uint32_t saved_crc=0;
  uint8_t* bytedata = (uint8_t*)&saved_crc;
  for(uint32_t z=sizeof(timecoreconf_t)+512;z<sizeof(timecoreconf_t)+512+sizeof(data_crc);z++){
    *bytedata=EEPROM.read(z);
    bytedata++;
  } 
  if(saved_crc!=data_crc){
    Serial.printf("TIME CONF: CRC SAVED: %u <> CRC DATA %u\n\rDo Failsave",saved_crc,data_crc);
    retval = Timecore::GetDefaultConfig();
    write_timecoreconf(retval);
  }
  return retval;
}


void write_credentials(credentials_t c){
  uint8_t* data=(uint8_t*)&c;
  CRC32 crc;
  for(uint32_t i=0;i<sizeof(credentials_t);i++){
      EEPROM.write(i,*data);
      crc.update(*data);
      data++;
  }
  uint32_t data_crc = crc.finalize(); 
  uint8_t* bytedata = (uint8_t*)&data_crc;
  for(uint32_t z=sizeof(credentials_t);z<sizeof(credentials_t)+sizeof(data_crc);z++){
    EEPROM.write(z,*bytedata);
    bytedata++;
  } 
  EEPROM.commit();
  
}

credentials_t read_credentials( void ){
  credentials_t retval;
  CRC32 crc;
  uint8_t* ret_ptr=(uint8_t*)&retval;
  uint8_t data;
  for(uint32_t i=0;i<sizeof(credentials_t);i++){
      data = EEPROM.read(i);
      crc.update(data);
      *ret_ptr=data;
      ret_ptr++;   
  }

   /* Next is to read the crc32*/
  uint32_t data_crc = crc.finalize(); 
  uint32_t saved_crc=0;
  uint8_t* bytedata = (uint8_t*)&saved_crc;
  for(uint32_t z=sizeof(credentials_t);z<sizeof(credentials_t)+sizeof(data_crc);z++){
    *bytedata=EEPROM.read(z);
    bytedata++;
  } 
  if(saved_crc!=data_crc){
    Serial.printf("WIFI CONF: CRC SAVED: %u <> CRC DATA %u\n\rDo Failsave",saved_crc,data_crc);
    bzero((void*)&retval,sizeof( credentials_t ));
    write_credentials(retval);
  }
  return retval;
}
