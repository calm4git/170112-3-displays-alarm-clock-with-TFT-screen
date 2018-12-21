#ifndef NTP_CLIENT_H
 #define NTP_CLIENT_H

#include "timecore.h"
#include "datastore.h"
#include <NtpClientLib.h>

class NTP_Client {
    public:
       
        void begin(  Timecore* tc );
        void SetServerName( char* ntpServerName );
        char* GetServerName();
        void SetNTPSyncEna( bool );
        bool GetNTPSyncEna( void );
        int32_t GetSyncInterval( void );
        void SetSyncInterval(int32_t);
        void Tick(); /* This needs to be called every second */
        void Sync( void );
        uint32_t ReadTime( bool* delayed_result); 
        void SaveSettings( void );
        void ReadSettings( void );
    private:
    
    void SyncEvent(NTPSyncEvent_t event); 
    
    Timecore* timeptr = NULL;
    volatile ntp_config_t Config;
    int32_t next_update=60; 

};
 #endif
