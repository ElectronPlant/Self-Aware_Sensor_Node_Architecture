/**
 * \file    power_agent.h
 *
 * \brief   Header file for the power agent.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __POWER_AGENT_H__
#define __POWER_AGENT_H__

#include "../platform/sa_types.h"
#include "../configs/config.h"

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup PowerAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef struct {
    float32_t BatteryVoltage;
    float32_t Temperature;
    float32_t Charge;
}  POWER_AGENT_BATTERY_OBS_T;

typedef struct {
    POWER_AGENT_BATTERY_OBS_T Battery;
}  POWER_AGENT_OBS_T;

typedef struct {
    uint8_t NotDefined;
}  POWER_AGENT_ACTS_T;

typedef struct {
    float32_t PredictedChargeDelta;     /* Prediction of the charge consumption         */
    float32_t PredictedIncrement;       /* Predicted change in the charge consumption   */
    uint32_t  ExpectedLifetime;         /* Expected lifetime in activations             */
}  POWER_AGENT_INPUTS_T;

typedef struct {
    CONFIG_POWER_T *PowerFeedbackPtr;
    int8_t    PowerIndex;
}  POWER_AGENT_OUTPUTS_T;

typedef struct {
    POWER_AGENT_INPUTS_T Inputs;
    POWER_AGENT_OUTPUTS_T Outputs;
} POWER_AGENT_INTERFACE_T;

/************* External functions ***********/
typedef void (*POWER_AGENT_OBSERVATION_T)(POWER_AGENT_OBS_T*);
typedef void (*POWER_AGENT_ACTUATION_T)(POWER_AGENT_ACTS_T*);
typedef void (*POWER_AGENT_BATTERY_DEPLETED_T)(void);

typedef struct {
    POWER_AGENT_OBSERVATION_T Obs;
    POWER_AGENT_ACTUATION_T Act;
    POWER_AGENT_BATTERY_DEPLETED_T Alarm;
} POWER_AGENT_INIT_T;

/************************************** Function prototypes **************************************/
float32_t PowerAgent_GetRemainingBatteryLife();
float32_t PowerAgent_GetRemainingChargePerc();
CONFIG_POWER_T *PowerAgent_GetPowerPtr();
float32_t PowerAgent_GetPowerMeasurement();

void PowerAgent_SetBatteryCharge(float32_t charge);
bool_t PowerAgent_Init(POWER_AGENT_OBSERVATION_T observe, POWER_AGENT_ACTUATION_T act,
                       POWER_AGENT_BATTERY_DEPLETED_T battery_depleted);

void PowerAgent_Oda(POWER_AGENT_INTERFACE_T *p_data);
void PowerAgent_Observe(POWER_AGENT_INTERFACE_T *p_data);
void PowerAgent_Act(POWER_AGENT_INTERFACE_T *p_data);

/** @} (end addtogroup PowerAgent)   */
/** @} (end addtogroup Agents)       */

#endif
