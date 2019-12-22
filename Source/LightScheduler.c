/*!
 * @file
 * @brief Light scheduler implementation.
 */

#include "LightScheduler.h"

// static uint8_t schedule_counter = 0;

void LightScheduler_Init(LightScheduler_t *instance, I_DigitalOutputGroup_t *lights, I_TimeSource_t *timeSource)
{
    instance->lights = lights;
    instance->timeSource = timeSource;
}

void LightScheduler_AddSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
    uint8_t schedulesSize = sizeof(instance->schedules)/ sizeof(instance->schedules[0]);
    if(instance->numSchedulesAdded < schedulesSize) {
        instance->schedules[instance->numSchedulesAdded].active = true;
        instance->schedules[instance->numSchedulesAdded].lightId = lightId;
        instance->schedules[instance->numSchedulesAdded].lightState = lightState;
        instance->schedules[instance->numSchedulesAdded].time = time;
        instance->numSchedulesAdded++;
    }
}

void LightScheduler_RemoveSchedule(LightScheduler_t *instance, uint8_t lightId, bool lightState, TimeSourceTickCount_t time)
{
    uint8_t schedulesSize = sizeof(instance->schedules)/ sizeof(instance->schedules[0]);
    for(uint8_t i = 0; i < schedulesSize; i++) {
        if(instance->schedules[i].active == true &&
           instance->schedules[i].lightId == lightId &&
           instance->schedules[i].lightState == lightState &&
           instance->schedules[i].time == time)
           {
               instance->schedules[i].active = false;
           }
    }
}

void LightScheduler_Run(LightScheduler_t *instance)
{
    TimeSourceTickCount_t time = TimeSource_GetTicks(instance->timeSource);
    uint8_t schedulesSize = sizeof(instance->schedules)/ sizeof(instance->schedules[0]);
    for(uint8_t i = 0; i < schedulesSize; i++) {
        if ((time == instance->schedules[i].time) && (instance->schedules[i].active))
        {
            DigitalOutputGroup_Write(instance->lights, instance->schedules[i].lightId, instance->schedules[i].lightState);
        }
    }
}