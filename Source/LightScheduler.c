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
    // if(schedule_counter < MAX_SCHEDULES) {
    instance->schedules[0].active = true;
    instance->schedules[0].lightId = lightId;
    instance->schedules[0].lightState = lightState;
    instance->schedules[0].time = time;
    // schedule_counter++;
    // }
}

void LightScheduler_Run(LightScheduler_t *instance)
{
    TimeSourceTickCount_t time = TimeSource_GetTicks(instance->timeSource);

    // for(uint8_t i = 0; i < MAX_SCHEDULES; i++) {
    if ((time == instance->schedules[0].time) && (instance->schedules[0].active))
    {
        DigitalOutputGroup_Write(instance->lights, instance->schedules[0].lightId, instance->schedules[0].lightState);
    }
    // }
}