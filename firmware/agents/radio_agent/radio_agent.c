/**
 * \file    radio_agent.c
 *
 * \brief   Main file for the radio agent.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: RadioAgent_
 *
 */

#include "../../platform/sa_types.h"

#include "../../include/radio_agent.h"
#include "../../configs/radio_cfg.h"

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup RadioAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
/**
 * \brief  Radio model
 *         This struct stores all the relevant information to model the radio.
 *
 */
typedef struct {
    RADIO_CFG_LIST_T CurrentConfig;
    float32_t PowerIncrement;
} RADIO_AGENT_MODEL_T;


/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
RADIO_AGENT_OBSERVATION_T RadioAgent_ObserveEnv = NULL;
RADIO_AGENT_ACTUATION_T RadioAgent_ActuateEnv = NULL;

RADIO_AGENT_MODEL_T RadioAgent_Model;

/************************************** Function implementation **********************************/

/************* External  ********************/
/**
 * \brief  Gets the current configuration of the radio
 *
 * \return Current radio configuration.
 *
 */
RADIO_CFG_LIST_T RadioAgent_GetConfig(void)
{
    return RadioAgent_Model.CurrentConfig;
}

/**
 * \brief  Gets the current power consumption
 *
 * \return Current power consumption.
 *
 */
float32_t RadioAgent_GetPower(void)
{
    return RadioCfg_Configs_Ptr[RadioAgent_Model.CurrentConfig].PowerCost.Power;
}

/**
 * \brief  Gets a pointer to the power configuration
 *
 * \return Pointer to the power configuration.
 *
 */
CONFIG_POWER_T *RadioAgent_GetPowerPtr(void)
 {
     return &RadioCfg_Configs_Ptr[RadioAgent_Model.CurrentConfig].PowerCost;
 }

 /************* Tools ************************/
/**
 * \brief  Updates the configuration.
 *
 * \param cfg:  New configuration.
 *
 */
static void RadioAgent_SetCfg(RADIO_CFG_LIST_T cfg)
{
    float32_t previous_power = RadioAgent_GetPower();
    RadioAgent_Model.CurrentConfig = cfg;
    RadioAgent_Model.PowerIncrement =  RadioAgent_GetPower() - previous_power;
}


/************* Initialization ***************/
/**
 * \brief  Initializes the agent.
 *
 * \param  observe:   Pointer to the observe function.
 * \param  act:       Pointer to the actuation function.
 *
 * \return DEF_TRUE if the agent could be initialized correctly; otherwise DEF_FALSE.
 *
 */
bool_t RadioAgent_Init(RADIO_AGENT_OBSERVATION_T observe, RADIO_AGENT_ACTUATION_T act) {

    bool_t initialization = DEF_TRUE;

    /* Initialize functions     */
    if (NULL == observe) {
        initialization = DEF_FALSE;
    } else {
        RadioAgent_ObserveEnv = observe;
    }
    if (NULL == act) {
        initialization = DEF_FALSE;
    } else {
        RadioAgent_ActuateEnv = act;
    }

    /* Initilize model      */
    RadioAgent_Model.CurrentConfig = RADIO_CFG_DEFAULT_CONFIG;
    RadioAgent_Model.PowerIncrement = RadioAgent_GetPower();

    return initialization;
}

/************* Observe **********************/
/**
 * \brief  Learns.
 *         In this context learning is application and sensor models.
 *
 * \param  *p_obs  Pointer to the obervations data.
 * \param  *p_int  Pointe to the interface data.
 *
 */
static void RadioAgent_Learn(RADIO_AGENT_OBS_T *p_obs, RADIO_AGENT_INTERFACE_T *p_int)
{
    /* Update model     */
    if (DEF_TRUE == p_obs->ConfigChange) {
        RadioAgent_SetCfg(p_obs->Config);
    }
}

/**
 * \brief  Reflects.
 *         For the moment there is nothing to reflect about.
 *
 * \param  *p_data  Pointer to the interface data.
 *
 */
static void RadioAgent_Reflect(RADIO_AGENT_INTERFACE_T *p_data)  {

    (void) p_data;
    //TODO Radio Agent, implement reflection.

    /* Plausibility     */

    /* Consistency      */

    /* Cross-validity   */
}

/************* Decide ***********************/
/**
 * \brief  Reason.
 *         In this context reasoning is using the available information:
 *           -) Predict the power consumption and the increment in the power consumption.
 *
 * \param  *p_data   Pointer to the interface data.
 *
 */
static void RadioAgent_Reason(RADIO_AGENT_INTERFACE_T *p_data)  {

    /* Generate outputs         */
    p_data->Outputs.PredictedPowerPtr = RadioAgent_GetPowerPtr();
    p_data->Outputs.PredictedPowerIncrement = RadioAgent_Model.PowerIncrement;
    RadioAgent_Model.PowerIncrement = 0;
}

/************* Act **************************/
// TODO Radio Agent actuation loop.

/************* Main ODA *********************/
/**
 * \brief  Performs the ODA loop for the radio agent.
 *
 * \param  p_data:  Pointer to the interface data of the agent.
 *
 */
void RadioAgent_Oda(RADIO_AGENT_INTERFACE_T *p_data)
{
    RADIO_AGENT_OBS_T observations;
    RADIO_AGENT_ACTS_T actuations;

    /* Observe data     */
    observations.Data = p_data->Inputs.Data;
    RadioAgent_ObserveEnv(&observations);
    RadioAgent_Learn(&observations, p_data);
    RadioAgent_Reflect(p_data);

    /* Decide           */
    RadioAgent_Reason(p_data);

    /* act              */
    actuations.Data = p_data->Inputs.Data;
    RadioAgent_ActuateEnv(&actuations);
}

/**
 * \brief  Runs the Observation task.
 *         The observation task perform the measurements, updates the model based on the
 *         observe data and compute the requried actuations to achieve its goals.
 *
 * \param  p_data: Pointer to the interface data of the agent.
 *
 */
void RadioAgent_Observe(RADIO_AGENT_INTERFACE_T *p_data)
{
    RADIO_AGENT_OBS_T observations;

    /* Observe data     */
    observations.Data = p_data->Inputs.Data;
    RadioAgent_ObserveEnv(&observations);
    RadioAgent_Learn(&observations, p_data);
    RadioAgent_Reflect(p_data);

    /* Decide           */
    RadioAgent_Reason(p_data);
}

/**
 * \brief  Runs the Actuation task.
 *         The actuation task applies the selected adaptations and updates the model
 *         accordingly.
 *
 * \param  p_data:  Pointer the the interface data of the agent.
 *
 */
void RadioAgent_Act(RADIO_AGENT_INTERFACE_T *p_data)
{
    RADIO_AGENT_ACTS_T actuations;

    /* act              */
    actuations.Data = p_data->Inputs.Data;
    RadioAgent_ActuateEnv(&actuations);
}

/** @} (end addtogroup RadioAgent)   */
/** @} (end addtogroup Agents)       */
