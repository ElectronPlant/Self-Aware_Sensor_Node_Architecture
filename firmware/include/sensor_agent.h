/**
 * \file    sensor_agent.h
 *
 * \brief   Header file for the sensor agent.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __SENSOR_AGENT_H__
#define __SENSOR_AGENT_H__

#include "../platform/sa_types.h"
#include "../configs/config.h"
#include "../configs/sensor_cfg.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup SensorAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef enum {
    SENSOR_AGENT_ALL_OK = 0,
    SENSOR_AGENT_MEASUREMENT_ERROR,
    SENSOR_AGENT_MAX_ERROR,
} SENSOR_AGENT_ERROR_T;

typedef struct {
    float32_t SensorData;
}  SENSOR_AGENT_OBS_T;

typedef struct {
    SENSOR_CFG_LIST_T Config;
}  SENSOR_AGENT_ACTS_T;

typedef struct {
    float32_t AccuracyTarget;
    float32_t PowerTarget;
}  SENSOR_AGENT_INPUTS_T;

typedef struct {
    float32_t MeasuredData;
    CONFIG_POWER_T *PredictedPowerPtr;
    float32_t PredictedPowerIncrement;
}  SENSOR_AGENT_OUTPUTS_T;

typedef struct {
    SENSOR_AGENT_INPUTS_T Inputs;
    SENSOR_AGENT_OUTPUTS_T Outputs;
} SENSOR_AGENT_INTERFACE_T;

/************* External functions ***********/
typedef void (*SENSOR_AGENT_OBSERVATION_T)(SENSOR_AGENT_OBS_T*);
typedef void (*SENSOR_AGENT_ACTUATION_T)(SENSOR_AGENT_ACTS_T*);
typedef void (*SENSOR_AGENT_ALARM_T)(SENSOR_AGENT_ERROR_T);

typedef struct {
    SENSOR_AGENT_OBSERVATION_T Obs;
    SENSOR_AGENT_ACTUATION_T Act;
    SENSOR_AGENT_ALARM_T Alarm;
} SENSOR_AGENT_INIT_T;


/************************************** Function prototypes **************************************/
SENSOR_CFG_LIST_T SensorAgent_GetConfig(void);
float32_t SensorAgent_GetPower(void);
CONFIG_POWER_T *SensorAgent_GetPowerPtr(void);

bool_t SensorAgent_Init(SENSOR_AGENT_OBSERVATION_T observe, SENSOR_AGENT_ACTUATION_T act, \
                        SENSOR_AGENT_ALARM_T alarm);
void SensorAgent_Oda(SENSOR_AGENT_INTERFACE_T *p_data);
void SensorAgent_Observe(SENSOR_AGENT_INTERFACE_T *p_data);
void SensorAgent_Act(SENSOR_AGENT_INTERFACE_T *p_data);

/** @} (end addtogroup SensorAgent)       */
/** @} (end addtogroup Agents)            */

#endif /* __SENSOR_AGENT_H__       */
