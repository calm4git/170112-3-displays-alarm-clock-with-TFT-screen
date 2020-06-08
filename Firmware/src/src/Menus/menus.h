#ifndef __MENUS_H__
 #define __MENUS_H__
//This will handle the menus, rendering and user inputs
#include "TFT_eSPI.h"
#include "../../Free_Fonts.h"
#include "src/AlarmMgr/AlarmMgr.h"

//We need a refference to the alarms
class Menus{

    public:
    
    //This are for Keyactions, we define Key A to B 
    typedef enum {
        NothingPressed =0,
        KeyA_Pressed,
        KeyA_Released,
        KeyB_Pressed,
        KeyB_Released,
        KeyC_Pressed,
        KeyC_Released,
        KeyD_Pressed,
        KeyD_Released,
    }KeyAction_t;

    

    //Requiered element for the 
    Menus( TFT_eSPI& lcd, AlarmMgr& Alarm);
    bool RenderMenu( KeyAction_t key, time_t utc_now ); //Entrypoint for the Menu
    void SetCallBackForGetTime( time_t (*GetTime)( void ) );
    void SetCallBackForSetTime( void (*SetTime)(time_t utc_now) );

    

    
    private:

    typedef enum{
        NoMenu=0,
        SettingsList,
        MenuTimeSettings,
        MenuDateSettings,
        MenuAlarmList,
        MenuAlarmSetting
    } MenuState_t;

    void DrawStatusWeekday(uint16_t offset_x, uint16_t offset_y,uint8_t Dow, bool Enabled, bool selected);
    void DrawAlarmEnabled(uint16_t offset_x,uint16_t offset_y ,bool Ena,bool selected);
    void DrawAlarmTimeHHMM(uint16_t offset_x,uint16_t offset_y, uint8_t hours, uint8_t minutes, bool hour_selected, bool minitues_selected);
    void DrawOneShot(uint16_t offset_x,uint16_t offset_y ,bool Ena,bool selected);
    void ShowAlarmSetup(  Alarm::Alarmtime_t At, uint8_t selected_option, bool refresh);

    void DrawSettingsMenuEntry( uint32_t offset, char* Name,  bool selected );
    void ShowMenuSettingsList( uint8_t selected_idx, bool refresh );
    void DrawTimeSettings( uint8_t Second, uint8_t Minute, uint16_t Hour, uint8_t selected_option,bool refresh );
    void DrawDateSettings( uint8_t Day, uint8_t Month, uint16_t Year, uint8_t selected_option, bool refresh);


    void DrawAlarmMenuEntry( uint32_t offset, bool selected,  Alarm::Alarmtime_t At  );
    void ShowMenuAlarmList( uint8_t selected_idx, bool refresh );

    

    void AlarmSettings( Alarm::Alarmtime_t &At, uint8_t option_idx);


    TFT_eSPI* _lcd;         //Requiered for rendering
    AlarmMgr* _Alarms;       //Interface to the Alarms

    MenuState_t CurretnMenuState;
    uint8_t SelectedAlarmIndex;

    uint8_t SelectedAlarmOptionIndex;
    uint8_t SelectedSettingsOptions;

    uint8_t SelectedTimeIndex;
    uint8_t SelectedDateIndex;
    
    time_t (*_GetTime)( void ); 
    void (*_SetTime)(time_t utc_now);

    uint8_t DaysInMonth( uint8_t Month, uint16_t year);
};

#endif





