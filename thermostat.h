

#ifndef thermostat_h
#define thermostat_h


#include <stdbool.h>

extern int Cnt_FlameOff;
extern int Cnt_FlameOn;

struct thermostat {
    int   Cnt_Output;
    float k_Filt;
    float T_Filt;
    float T_Meas;
    int   T_Hyst;
    int   T_Target;
};


#endif
