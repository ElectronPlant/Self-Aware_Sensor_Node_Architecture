/**
 * \file    trigger_agent.h
 *
 * \brief   Header file for the trigger agent.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __TRIGGER_AGENT_H__
#define __TRIGGER_AGENT_H__

#include "../platform/sa_types.h"
#include "../configs/trigger_cfg.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup TriggerAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef enum {
    TRIGGER_AGENT_ALL_OK = 0,
    TRIGGER_AGENT_MAX_ERROR,
} TRIGGER_AGENT_ERROR_T;

typedef struct {
    uint8_t Reserved;
}  TRIGGER_AGENT_OBS_T;

typedef struct {
    uint32_t Periodicity;
}  TRIGGER_AGENT_ACTS_T;

typedef struct {
    int8_t SamplingTarget;
}  TRIGGER_AGENT_INPUTS_T;

typedef struct {
    uint32_t Periodicity;
}  TRIGGER_AGENT_OUTPUTS_T;

typedef struct {
    TRIGGER_AGENT_INPUTS_T Inputs;
    TRIGGER_AGENT_OUTPUTS_T Outputs;
} TRIGGER_AGENT_INTERFACE_T;

/************* External functions ***********/
typedef void (*TRIGGER_AGENT_OBSERVATION_T)(TRIGGER_AGENT_OBS_T*);
typedef void (*TRIGGER_AGENT_ACTUATION_T)(TRIGGER_AGENT_ACTS_T*);
typedef void (*TRIGGER_AGENT_ALARM_T)(uint8_t);

typedef struct {
    TRIGGER_AGENT_OBSERVATION_T Obs;
    TRIGGER_AGENT_ACTUATION_T Act;
    TRIGGER_AGENT_ALARM_T Alarm;
} TRIGGER_AGENT_INIT_T;


/************************************** Function prototypes **************************************/
uint32_t TriggerAgent_GetConfig(void);
bool_t TriggerAgent_Init(TRIGGER_AGENT_OBSERVATION_T observe, TRIGGER_AGENT_ACTUATION_T act, \
                         TRIGGER_AGENT_ALARM_T alarm);
void TriggerAgent_Oda(TRIGGER_AGENT_INTERFACE_T *p_data);
void TriggerAgent_Observe(TRIGGER_AGENT_INTERFACE_T *p_data);
void TriggerAgent_Act(TRIGGER_AGENT_INTERFACE_T *p_data);

/** @} (end addtogroup TriggerAgent)       */
/** @} (end addtogroup Agents)            */

#endif /* __TRIGGER_AGENT_H__       */
