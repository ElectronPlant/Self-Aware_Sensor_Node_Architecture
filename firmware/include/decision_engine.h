/**
 * \file    decision_engine.h
 *
 * \brief   Header file for the decision engine.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __DECISION_ENGINE_H__
#define __DECISION_ENGINE_H__

#include "../platform/sa_types.h"

#include "power_agent.h"
#include "radio_agent.h"
#include "sensor_agent.h"
#include "trigger_agent.h"
#include "app_agent.h"

/** \addtogroup DecicionEngine
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef struct {
    RADIO_AGENT_INTERFACE_T RadioInterface;
    APP_AGENT_INTERFACE_T AppInterface;
    POWER_AGENT_INTERFACE_T PowerInterface;
} DECISION_ENGINE_INTERFACES_T;

typedef struct {
    RADIO_AGENT_INIT_T RadioInit;
    POWER_AGENT_INIT_T PowerInit;
    APP_AGENT_INIT_T AppInit;
} DECISION_ENGINE_INIT_T;


/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/
void DecisionEng_GetIndexValues(int8_t *relevance, int8_t *power);
void DecisionEng_GetFeedbackValues(float32_t *predicted_power, float32_t *predicted_increment,
                                   float32_t *power_feedback);

CONFIG_POWER_T *DecisionEng_GetBasePowerPtr(void);
CONFIG_POWER_T *DecisionEng_GetIdlePowerPtr(void);
float32_t DecisionEng_GetPower(void);

bool_t DecisionEng_Init(DECISION_ENGINE_INIT_T *p_init);
void DecisionEng_SetExpectedLife(uint32_t expected_life);
void DecisionEng_Loop(void);


/** @} (end addtogroup DecisionEngine)  */

#endif /* __DECISION_ENGINE_H__         */
