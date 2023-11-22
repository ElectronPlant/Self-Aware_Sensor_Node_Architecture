/**
 * \file    app_agent.h
 *
 * \brief   Header file for the Application agent.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __APP_AGENT_H__
#define __APP_AGENT_H__

#include "../platform/sa_types.h"

#include "sensor_agent.h"
#include "trigger_agent.h"

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup AppAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef enum {
    APP_AGENT_ALL_OK = 0,
    APP_AGENT_MAX_ERROR,
} APP_AGENT_ERROR_T;

typedef struct {
    uint8_t Reserved;
}  APP_AGENT_OBS_T;

typedef struct {
    uint8_t Reserved;
}  APP_AGENT_ACTS_T;

typedef struct {
    int8_t RelevanceTarget;
}  APP_AGENT_INPUTS_T;

typedef struct {
    float32_t Data;
    CONFIG_POWER_T *PredictedPowerPtr;
    float32_t PredictedPowerIncrement;
    uint32_t Periodicity;
    int8_t RelevanceIndex;
}  APP_AGENT_OUTPUTS_T;

typedef struct {
    APP_AGENT_INPUTS_T Inputs;
    APP_AGENT_OUTPUTS_T Outputs;
} APP_AGENT_INTERFACE_T;

/************* External functions ***********/
typedef void (*APP_AGENT_ALARM_T)(uint8_t);

typedef struct {
    SENSOR_AGENT_INIT_T Sensor;
    TRIGGER_AGENT_INIT_T Trigger;
    APP_AGENT_ALARM_T Alarm;
} APP_AGENT_INIT_T;

/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/
bool_t AppAgent_Init(SENSOR_AGENT_OBSERVATION_T sensor_obs,
                     SENSOR_AGENT_ACTUATION_T sensor_act,
                     SENSOR_AGENT_ALARM_T sensor_alrm,
                     TRIGGER_AGENT_OBSERVATION_T trigger_obs,
                     TRIGGER_AGENT_ACTUATION_T trigger_act,
                     TRIGGER_AGENT_ALARM_T trigger_alrm,
                     APP_AGENT_ALARM_T app_alrm);
void AppAgent_Oda(APP_AGENT_INTERFACE_T *p_data);
void AppAgent_Observe(APP_AGENT_INTERFACE_T *p_data);
void AppAgent_Act(APP_AGENT_INTERFACE_T *p_data);


/** @} (end addtogroup AppAgent)    */
/** @} (end addtogroup Agents)      */

#endif /* __APP_AGENT_H__       */
