/**
 * \file    trigger_agent.c
 *
 * \brief   Main file for the trigger agent.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: TriggerAgent_
 *
 */

#include "../../platform/sa_types.h"
#include "../../platform/sa_utils.h"

#include "../../include/agents_main.h"
#include "../../configs/trigger_cfg.h"
#include "../../include/trigger_agent.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup TriggerAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
/**
 * \brief  Trigger model.
 *         This struct stores all the relevant informtion to model the trigger.
 *
 * \note List of notes:
 *       1. For simplicity the trigger will be only represented by the time between
 *          samples in ms.
 */
typedef struct {
    uint8_t Config;
    uint32_t Periodicity;
    int8_t SamplingTarget;
} TRIGGER_AGENT_MODEL_T;


/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
TRIGGER_AGENT_OBSERVATION_T TriggerAgent_ObserveEnv = NULL;
TRIGGER_AGENT_ACTUATION_T TriggerAgent_ActuateEnv = NULL;
TRIGGER_AGENT_ALARM_T TriggerAgent_Alarm = NULL;

TRIGGER_AGENT_MODEL_T TriggerAgent_Model;


/************************************** Function implementation **********************************/

/************* External  ********************/
/**
 * \brief  Gets the current periodicity configuration.
 *
 * \return Current periodicity configuration.
 *
 */
uint32_t TriggerAgent_GetConfig(void)
{
    return TriggerAgent_Model.Periodicity;
}

/************* Utils  ***********************/
/**
 * \brief  Updates the periodicity given a new configuration.
 *
 * \param  cfg: new configuration.
 *
 */
static void TriggerAgent_UpdatePeriodicity(uint8_t cfg)
{
    TriggerAgent_Model.Config = cfg;
    TriggerAgent_Model.Periodicity = TriggerCfg_Periods_Ptr[cfg];
}

/**
 * \brief  Updates the periodicity.
 *
 */
static void TriggerAgent_SetConfig(void)
{
    if (0 != TriggerAgent_Model.SamplingTarget) {
        int16_t step;
        step = SA_UTILS_ABS(TriggerAgent_Model.SamplingTarget) / TRIGGER_CFG_UPDATE_STEP;
        step++;
        step *= 0 <= TriggerAgent_Model.SamplingTarget ? -1 :  1;
        step += TriggerAgent_Model.Config;
        step = SA_UTILS_SATURATE(TRIGGER_CFG_MAXIMUM_SAMPLING,
                                 TRIGGER_CFG_MINIMUM_SAMPLING,
                                 step);
        TriggerAgent_UpdatePeriodicity((uint8_t)step);
        TriggerAgent_Model.SamplingTarget = 0;
    }
}

/************* Initialization ***************/
/**
 * \brief  Initializes the agent.
 *
 * \param  observe:   Pointer to the observe function.
 * \param  act:       Pointer to the actuation function.
 * \param  alarm:     Pointer to the alarm function.
 *
 * \return DEF_TRUE if the agent could be initialized correctly; otherwise DEF_FALSE.
 *
 */
bool_t TriggerAgent_Init(TRIGGER_AGENT_OBSERVATION_T observe, TRIGGER_AGENT_ACTUATION_T act, \
                       TRIGGER_AGENT_ALARM_T alarm)
{
    bool_t initialization = DEF_TRUE;

    /* Initialize functions     */
    if (NULL == observe) {
        initialization = DEF_FALSE;
    } else {
        TriggerAgent_ObserveEnv = observe;
    }
    if (NULL == act) {
        initialization = DEF_FALSE;
    } else {
        TriggerAgent_ActuateEnv = act;
    }
    TriggerAgent_Alarm = alarm;

    /* Initilize model      */
    TriggerAgent_UpdatePeriodicity(TRIGGER_CFG_DEFAULT_SAMPLING);
    TriggerAgent_Model.SamplingTarget = 0;

    return initialization;
}

/************* Observe **********************/
/**
 * \brief  Learns.
 *
 * \param  *p_obs  Pointer to the obervations data.
 * \param  *p_int  Pointe to the interface data.
 *
 */
static void TriggerAgent_Learn(TRIGGER_AGENT_OBS_T *p_obs, TRIGGER_AGENT_INTERFACE_T *p_int)
{
    (void) p_obs;
    // TODO Trigger Agent, implement learning
    TriggerAgent_Model.SamplingTarget = p_int->Inputs.SamplingTarget;
}

/**
 * \brief  Reflects.
 *         For the moment there is nothing to reflect about.
 *
 * \param  *p_data  Pointer to the interface data.
 *
 */
static void TriggerAgent_Reflect(TRIGGER_AGENT_INTERFACE_T *p_data)
{
    (void) p_data;
    //TODO Trigger Agent, implement reflection.

    /* Plausibility     */

    /* Consistency      */

    /* Cross-validity   */
}

/************* Decide ***********************/
/**
 * \brief  Reason.
 *         In this context reasoning is just generating the outputs.
 *
 * \param  *p_data   Pointer to the interface data.
 *
 */
static void TriggerAgent_Reason(TRIGGER_AGENT_INTERFACE_T *p_data)
{
    /* Compute periodicity      */
    TriggerAgent_SetConfig();

    /* Generate outputs         */
    p_data->Outputs.Periodicity = TriggerAgent_Model.Periodicity;
}

/************* Act **************************/
/**
 * \brief  Manages the actuations of the trigger agent.
 *
 * \param  p_acts: Pointer to the actuation adata.
 *
 */
static void TriggerAgent_ManageActuation(TRIGGER_AGENT_ACTS_T *p_acts)
{
    p_acts->Periodicity = TriggerAgent_Model.Periodicity;
}

/************* Main ODA *********************/
/**
 * \brief  Performs the ODA loop for the radio agent.
 *
 * \param  p_data:  Pointer to the interface data of the agent.
 *
 */
void TriggerAgent_Oda(TRIGGER_AGENT_INTERFACE_T *p_data)
{
    TRIGGER_AGENT_OBS_T observations;
    TRIGGER_AGENT_ACTS_T actuations;

    /* Observe data     */
    TriggerAgent_ObserveEnv(&observations);
    TriggerAgent_Learn(&observations, p_data);
    TriggerAgent_Reflect(p_data);

    /* Decide           */
    TriggerAgent_Reason(p_data);

    /* act              */
    TriggerAgent_ManageActuation(&actuations);
    TriggerAgent_ActuateEnv(&actuations);
}

/**
 * \brief  Runs the Observation task.
 *         The observation task perform the measurements, updates the model based on the
 *         observe data and compute the requried actuations to achieve its goals.
 *
 * \param  p_data: Pointer to the interface data of the agent.
 *
 */
void TriggerAgent_Observe(TRIGGER_AGENT_INTERFACE_T *p_data)
{
    TRIGGER_AGENT_OBS_T observations;

    /* Observe data     */
    TriggerAgent_ObserveEnv(&observations);
    TriggerAgent_Learn(&observations, p_data);
    TriggerAgent_Reflect(p_data);

    /* Decide           */
    TriggerAgent_Reason(p_data);
}

/**
 * \brief  Runs the Actuation task.
 *         The actuation task applies the selected adaptations and updates the model
 *         accordingly.
 *
 * \param  p_data:  Pointer the the interface data of the agent.
 *
 */
void TriggerAgent_Act(TRIGGER_AGENT_INTERFACE_T *p_data)
{
    TRIGGER_AGENT_ACTS_T actuations;

    /* act              */
    TriggerAgent_ManageActuation(&actuations);
    TriggerAgent_ActuateEnv(&actuations);
}


/** @} (end addtogroup TriggerAgent)   */
/** @} (end addtogroup Agents)         */
