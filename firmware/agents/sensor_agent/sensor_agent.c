/**
 * \file    sensor_agent.c
 *
 * \brief   Main file for the sensor agent.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: SensorAgent_
 *
 */

#include <math.h>
#include "../../platform/sa_types.h"
#include "../../platform/sa_utils.h"

#include "../../include/agents_main.h"
#include "../../configs/config.h"
#include "../../configs/sensor_cfg.h"
#include "../../include/sensor_agent.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup SensorAgent
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
/**
 * \brief  Sensor model.
 *         This struct stores all the relevant informtion to model the sensor.
 *         The model contains the current configuration and the list of possible
 *         configurations with the expected effect in the accuracy and power consumption
 *
 * \note List of notes:
 *       1. For simplicity the sensor will be modeled as having multiple sleep levels,
 *          this will need to be translated into actual configurations of the sensor.
 */
typedef struct {
    SENSOR_CFG_LIST_T CurrentConfig;
    float32_t PowerIncrement;
    float32_t Data;
} SENSOR_AGENT_MODEL_T;


/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
SENSOR_AGENT_OBSERVATION_T SensorAgent_ObserveEnv = NULL;
SENSOR_AGENT_ACTUATION_T SensorAgent_ActuateEnv = NULL;
SENSOR_AGENT_ALARM_T SensorAgent_Alarm = NULL;

SENSOR_AGENT_MODEL_T SensorAgent_Model;


/************************************** Function implementation **********************************/

/************* External  ********************/
/**
 * \brief  Gets the current configuration of the sensor.
 *
 * \return Current sensor configuration.
 *
 */
SENSOR_CFG_LIST_T SensorAgent_GetConfig(void)
{
    return SensorAgent_Model.CurrentConfig;
}

/**
 * \brief  Gets the current power consumption of the sensor.
 *
 * \return Current sensor power consumption.
 *
 */
float32_t SensorAgent_GetPower(void)
{
    return SensorCfg_Configs_Ptr[SensorAgent_Model.CurrentConfig].PowerCost.Power;
}

/**
 * \brief  Gets a pointer to the current power configuration of the sensor.
 *
 * \return Pointer to the current power configuration.
 *
 */
CONFIG_POWER_T *SensorAgent_GetPowerPtr(void)
{
    return &SensorCfg_Configs_Ptr[SensorAgent_Model.CurrentConfig].PowerCost;
}

/************* Utils  ***********************/
/**
 * \brief  Updates the configuration of the sensor.
 *
 * \param  config:  New configuration.
 *
 */
static void SensorAgent_SetConfig(SENSOR_CFG_LIST_T config)
{
    float32_t previous_power = SensorAgent_GetPower();
    SensorAgent_Model.CurrentConfig = config;
    SensorAgent_Model.PowerIncrement = SensorAgent_GetPower() - previous_power;
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
bool_t SensorAgent_Init(SENSOR_AGENT_OBSERVATION_T observe, SENSOR_AGENT_ACTUATION_T act, \
                       SENSOR_AGENT_ALARM_T alarm)
{
    bool_t initialization = DEF_TRUE;

    /* Initialize functions     */
    if (NULL == observe) {
        initialization = DEF_FALSE;
    } else {
        SensorAgent_ObserveEnv = observe;
    }
    if (NULL == act) {
        initialization = DEF_FALSE;
    } else {
        SensorAgent_ActuateEnv = act;
    }
    SensorAgent_Alarm = alarm;

    /* Initilize model      */
    SensorAgent_Model.CurrentConfig = SENSOR_CFG_DEFAULT_CONFIG;
    SensorAgent_Model.PowerIncrement = SensorAgent_GetPower();
    SensorAgent_Model.Data = 0;

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
static void SensorAgent_Learn(SENSOR_AGENT_OBS_T *p_obs, SENSOR_AGENT_INTERFACE_T *p_int)
{
    SensorAgent_Model.Data = p_obs->SensorData;
}

/**
 * \brief  Reflects.
 *         For the moment there is nothing to reflect about.
 *
 * \param  *p_data  Pointer to the interface data.
 *
 */
static void SensorAgent_Reflect(SENSOR_AGENT_INTERFACE_T *p_data)
{
    (void) p_data;
    if (SENSOR_CFG_ERROR_CODE == SensorAgent_Model.Data) {
        if (NULL != SensorAgent_Alarm) SensorAgent_Alarm(SENSOR_AGENT_MEASUREMENT_ERROR);
    }
    //TODO Sensor Agent, implement reflection.

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
static void SensorAgent_Reason(SENSOR_AGENT_INTERFACE_T *p_data)
{
    /* Generate outputs         */
    p_data->Outputs.PredictedPowerPtr = SensorAgent_GetPowerPtr();
    p_data->Outputs.PredictedPowerIncrement = SensorAgent_Model.PowerIncrement;
    p_data->Outputs.MeasuredData = SensorAgent_Model.Data;
    SensorAgent_Model.PowerIncrement = 0;
}

/************* Act **************************/
/**
 * \brief  Manages the actuations of the sensor agent.
 *
 * \param  p_acts: Pointer to the actuation adata.
 *
 */
static void SensorAgent_ManageActuation(SENSOR_AGENT_ACTS_T *p_acts)
{
    p_acts->Config = SensorAgent_Model.CurrentConfig;
    SensorAgent_SetConfig(p_acts->Config);
}

/************* Main ODA *********************/
/**
 * \brief  Performs the ODA loop for the agent.
 *
 * \param  p_data:  Pointer to the interface data of the agent.
 *
 */
void SensorAgent_Oda(SENSOR_AGENT_INTERFACE_T *p_data)
{
    SENSOR_AGENT_OBS_T observations;
    SENSOR_AGENT_ACTS_T actuations;

    /* Observe data     */
    SensorAgent_ObserveEnv(&observations);
    SensorAgent_Learn(&observations, p_data);
    SensorAgent_Reflect(p_data);

    /* Decide           */
    SensorAgent_Reason(p_data);

    /* act              */
    SensorAgent_ManageActuation(&actuations);
    SensorAgent_ActuateEnv(&actuations);
}

/**
 * \brief  Runs the Observation task.
 *         The observation task perform the measurements, updates the model based on the
 *         observe data and compute the requried actuations to achieve its goals.
 *
 * \param  p_data: Pointer to the interface data of the agent.
 *
 */
void SensorAgent_Observe(SENSOR_AGENT_INTERFACE_T *p_data)
{
    SENSOR_AGENT_OBS_T observations;

    /* Observe data     */
    SensorAgent_ObserveEnv(&observations);
    SensorAgent_Learn(&observations, p_data);
    SensorAgent_Reflect(p_data);

    /* Decide           */
    SensorAgent_Reason(p_data);
}

/**
 * \brief  Runs the Actuation task.
 *         The actuation task applies the selected adaptations and updates the model
 *         accordingly.
 *
 * \param  p_data:  Pointer the the interface data of the agent.
 *
 */
void SensorAgent_Act(SENSOR_AGENT_INTERFACE_T *p_data)
{
    SENSOR_AGENT_ACTS_T actuations;

    /* act              */
    SensorAgent_ManageActuation(&actuations);
    SensorAgent_ActuateEnv(&actuations);
}


/** @} (end addtogroup SensorAgent)    */
/** @} (end addtogroup Agents)         */
