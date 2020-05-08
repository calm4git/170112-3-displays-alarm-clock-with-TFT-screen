#ifndef DATASTORE_H_
 #define DATASTORE_H_
 
#include "timecore.h"

typedef struct {
  char ssid[128];
  char pass[128];
} credentials_t;



typedef struct{
  char ntpServerName[129];
  bool NTPEnable;
  int32_t SyncIntervall;
} ntp_config_t;

void datastoresetup();

void write_ntp_config(ntp_config_t c);

ntp_config_t read_ntp_config( void );

void write_timecoreconf(timecoreconf_t c);

timecoreconf_t read_timecoreconf( void );

void write_credentials(credentials_t c);

credentials_t read_credentials( void );

#endif
