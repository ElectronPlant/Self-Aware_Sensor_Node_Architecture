/**
 * \file    radio_agent.h
 *
 * \brief   Header file for the radio agent
 *
 * \author  David Arnaiz
 *
 */

#ifndef __RADIO_AGENT_H__
#define __RADIO_AGENT_H__

#include "../platform/sa_types.h"
#include "../configs/config.h"
#include "../configs/radio_cfg.h"

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup RadioAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef struct {
    bool_t ConfigChange;        /* Flag to state if the config has changed or not   */
    RADIO_CFG_LIST_T Config;
    float32_t Data;
} RADIO_AGENT_OBS_T;

typedef struct {
    float32_t Data;
} RADIO_AGENT_ACTS_T;

typedef struct {
    float32_t Data;
} RADIO_AGENT_INPUTS_T;

typedef struct {
    CONFIG_POWER_T *PredictedPowerPtr;
    float32_t PredictedPowerIncrement;
} RADIO_AGENT_OUTPUTS_T;

typedef struct {
    RADIO_AGENT_INPUTS_T Inputs;
    RADIO_AGENT_OUTPUTS_T Outputs;
} RADIO_AGENT_INTERFACE_T;

/************* External functions ***********/
typedef void (*RADIO_AGENT_OBSERVATION_T)(RADIO_AGENT_OBS_T*);
typedef void (*RADIO_AGENT_ACTUATION_T)(RADIO_AGENT_ACTS_T*);

typedef struct {
    RADIO_AGENT_OBSERVATION_T Obs;
    RADIO_AGENT_ACTUATION_T Act;
} RADIO_AGENT_INIT_T;

/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/
RADIO_CFG_LIST_T RadioAgent_GetConfig(void);
float32_t RadioAgent_GetPower(void);
CONFIG_POWER_T *RadioAgent_GetPowerPtr(void);

bool_t RadioAgent_Init(RADIO_AGENT_OBSERVATION_T observe, RADIO_AGENT_ACTUATION_T act);
void RadioAgent_Oda(RADIO_AGENT_INTERFACE_T *p_data);
void RadioAgent_Observe(RADIO_AGENT_INTERFACE_T *p_data);
void RadioAgent_Act(RADIO_AGENT_INTERFACE_T *p_data);

/** @} (end addtogroup Agents)      */
/** @} (end addtogroup RadioAgent)  */

#endif /* __RADIO_AGENT_H__       */
