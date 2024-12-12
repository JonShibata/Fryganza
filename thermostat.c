#include "thermostat.h"


void thermostat_control(struct thermostat* data) {

    // Lag filter
    if (!isnan(data->T_Meas)){
        data->T_Filt = data->T_Filt + (data->T_Meas - data->T_Filt) * data->k_Filt;
    }
    

    if (data->T_Filt > (float)(data->T_Target + data->T_Hyst)) {
        data->Cnt_Output = Cnt_FlameOff;

    } else if (data->T_Filt < (float)(data->T_Target - data->T_Hyst)) {
        data->Cnt_Output = Cnt_FlameOn;
    }
}
