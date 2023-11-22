/**
 * \file    app_agent.c
 *
 * \brief   Main file for the application agent.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: AppAgent_
 *
 */

#include <string.h>
#include "../../platform/sa_types.h"
#include "../../platform/sa_utils.h"

#include "../../include/agents_main.h"
#include "../../configs/app_cfg.h"
#include "../../include/sensor_agent.h"
#include "../../include/trigger_agent.h"
#include "../../include/app_agent.h"

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup AppAgent
 *   @{
 */

/************************************** Defines **************************************************/
#define APP_AGENT_AVERAGE_NUM           10u
#define APP_AGENT_CONFIDENCE_DIV        3u


/************************************** Typedef **************************************************/
/**
 * \brief  Application model.
 *         The application model contains the following metrics:
 *              -) Avg value, which is used to predict the future values.
 *              -) Variation, to monitor the rate of change.
 *              -) Accuracy level metric.
 *              -) Relevance index.
 *
 */
typedef struct {
    float32_t Data;
    float32_t Rate;
    float32_t AvgData;
    float32_t AvgDataBuffer[APP_AGENT_AVERAGE_NUM];
    bool_t    AvgDataInitilized;
    uint16_t  AvgDataPointer;
    float32_t AccuracyLevel;
    int8_t    RelevanceIndex;
    int8_t    Confidence;
    bool_t    Initialized;
} APP_AGENT_MODEL_T;


/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
static APP_AGENT_ALARM_T AppAgent_Alarm = NULL;

static APP_AGENT_MODEL_T AppAgent_Model;

static SENSOR_AGENT_INTERFACE_T AppAgent_SensorData;
static TRIGGER_AGENT_INTERFACE_T AppAgent_TriggerData;


/************************************** Function implementation **********************************/

/************* Initialization ***************/
/**
 * \brief  Initializes the agent.
 *
 * \param  sensor_obs   Pointer to the observation function of the sensor agent.
 * \param  sensor_act   Pointer to the actuation funciton of the sensor agent.
 * \param  sensor_alrm  Pointer to the alarm function of the sensor agent.
 * \param  trigger_obs  Pointer to the observation function of the trigger agent.
 * \param  trigger_act  Pointer to the observation function of the trigger agent.
 * \param  trigger_alrm Pointer to the alarm function of the trigger agent.
 * \param  app_alrm:    Pointer to an alarm function of the application agent.
 *
 * \return DEF_TRUE if the agent could be initialized correctly; otherwise DEF_FALSE.
 *
 * \note List of notes:
 *       1. The alarm function is optional, so the NULL check is not performed.
 *
 */
bool_t AppAgent_Init(SENSOR_AGENT_OBSERVATION_T sensor_obs,
                     SENSOR_AGENT_ACTUATION_T sensor_act,
                     SENSOR_AGENT_ALARM_T sensor_alrm,
                     TRIGGER_AGENT_OBSERVATION_T trigger_obs,
                     TRIGGER_AGENT_ACTUATION_T trigger_act,
                     TRIGGER_AGENT_ALARM_T trigger_alrm,
                     APP_AGENT_ALARM_T app_alrm)
{

    bool_t initialization = DEF_TRUE;

    initialization = SensorAgent_Init(sensor_obs, sensor_act, sensor_alrm);
    if (DEF_TRUE == initialization) {
        initialization = TriggerAgent_Init(trigger_obs, trigger_act, trigger_alrm);
    }
    if (DEF_TRUE == initialization) {
        AppAgent_Alarm = app_alrm;
    }

    /* Initilize model      */
    if (DEF_TRUE == initialization) {
        memset(&AppAgent_Model, 0x00, sizeof(APP_AGENT_MODEL_T));
        AppAgent_Model.AvgDataInitilized = DEF_FALSE;
    }

    return initialization;
}

/************* Observe **********************/
/**
 * \brief  Learns.
 *         In this context learning is updating the model.
 *
 * \param  p_sensor   Pointer to the interface data.
 *
 */
static void AppAgent_Learn(SENSOR_AGENT_INTERFACE_T *p_sensor)
{
    float32_t current_data;

    current_data = p_sensor->Outputs.MeasuredData;

    AppAgent_Model.Rate = SaUtils_ChangeRate(current_data,
                                             AppAgent_Model.Data,
                                             APP_CFG_MINIMUM_RATE_REF);
    AppAgent_Model.Data = current_data;

    /* Update application model                 */
    SaUtils_Average(&AppAgent_Model.AvgData,
                    current_data,
                    AppAgent_Model.AvgDataBuffer,
                    APP_AGENT_AVERAGE_NUM,
                    &AppAgent_Model.AvgDataPointer,
                    &AppAgent_Model.AvgDataInitilized);
}

/**
 * \brief  Reflects.
 *         In this context reflecting is making sure that the data is correct, by:
 *           -) Computing the plausibility of the measured data, by checking the range.
 *           -) Computing the consistency of the measured data, by checking the rate of change.
 *           -) Cross-validity of the charge date, by comparing the predicted value with the
 *              actual one.
 *
 * \param  *p_data  Pointer to the interface data.
 *
 */
static void AppAgent_Reflect(APP_AGENT_INTERFACE_T *p_data)
{
    int8_t plausibility, consistency, cross_validity;
    int32_t confidence;

    /* Plausibility     */
    plausibility = Agents_Plausibilty(AppAgent_Model.Data,
                                      APP_CFG_RANGE_LOW,
                                      APP_CFG_RANGE_HIGH);

    if (DEF_TRUE == AppAgent_Model.Initialized) {
        /* Consistency      */
        consistency = Agents_Consistency(AppAgent_Model.Rate,
                                        APP_CFG_MAXIMUM_RATE);

        /* Cross-validity   */
        cross_validity = Agents_CrossValidity(AppAgent_Model.Data,
                                              AppAgent_Model.AvgData,
                                              APP_CFG_DEVIATION);
    } else {
        consistency = 0;
        cross_validity = 0;
        AppAgent_Model.Initialized = DEF_TRUE;
    }

    confidence = (plausibility + consistency + cross_validity) / APP_AGENT_CONFIDENCE_DIV;
    AppAgent_Model.Confidence = (int8_t) confidence;
}

/************* Decide ***********************/
/**
 * \brief  Reason.
 *         In this context reasoning is using the available information:
 *           -) Compute the new required EAR to maintain event detection probability.
 *           -) Compute the "relevance index", and check that relevance is met.
 *
 * \param  p_app   Pointer to the application interface data.
 * \param  p_sens  Pointer to the sensor interface data.
 * \param  p_trig  Pointer to the trigger interface data.
 *
 */
static void AppAgent_Reason(APP_AGENT_INTERFACE_T *p_app,
                            SENSOR_AGENT_INTERFACE_T *p_sens,
                            TRIGGER_AGENT_INTERFACE_T *p_trig)
{
    //TODO app agent relevance index should translate sensor and trigger configs.
    AppAgent_Model.RelevanceIndex = AppAgent_Model.Confidence;

    /* Generate outputs         */
    p_app->Outputs.RelevanceIndex = AppAgent_Model.RelevanceIndex;
    p_app->Outputs.Data = AppAgent_Model.Data;
    p_app->Outputs.PredictedPowerPtr = p_sens->Outputs.PredictedPowerPtr;
    p_app->Outputs.PredictedPowerIncrement = p_sens->Outputs.PredictedPowerIncrement;
    p_app->Outputs.Periodicity = p_trig->Outputs.Periodicity;
}

/************* Main ODA *********************/
/**
 * \brief  Performs the ODA loop for the agent.
 *
 * \param  p_data:  Pointer to the interface data of the agent.
 *
 */
void AppAgent_Oda(APP_AGENT_INTERFACE_T *p_data)
{
    /* Observe data     */
    AppAgent_SensorData.Inputs.AccuracyTarget = 0;  // TODO sensor agent accuracy loop not implemented
    AppAgent_SensorData.Inputs.PowerTarget = 0;     // TODO sensor agent power loop still not implemented
    SensorAgent_Oda(&AppAgent_SensorData);

    AppAgent_TriggerData.Inputs.SamplingTarget = p_data->Inputs.RelevanceTarget;
    TriggerAgent_Oda(&AppAgent_TriggerData);

    AppAgent_Learn(&AppAgent_SensorData);
    AppAgent_Reflect(p_data);

    /* Decide           */
    AppAgent_Reason(p_data, &AppAgent_SensorData, &AppAgent_TriggerData);

    /* act              */
    /* No actuations for this agent     */
}

/**
 * \brief  Runs the Observation task.
 *         The observation task perform the measurements, updates the model based on the
 *         observe data and compute the requried actuations to achieve its goals.
 *
 * \param  p_data: Pointer to the interface data of the agent.
 *
 */
void AppAgent_Observe(APP_AGENT_INTERFACE_T *p_data)
{
    /* Sensor       */
    AppAgent_SensorData.Inputs.AccuracyTarget = 0;  // TODO sensor agent accuracy loop not implemented
    AppAgent_SensorData.Inputs.PowerTarget = 0;     // TODO sensor agent power loop still not implemented
    SensorAgent_Observe(&AppAgent_SensorData);

    /* Trigger      */
    AppAgent_TriggerData.Inputs.SamplingTarget = p_data->Inputs.RelevanceTarget;
    TriggerAgent_Observe(&AppAgent_TriggerData);

    /* App          */
    AppAgent_Learn(&AppAgent_SensorData);
    AppAgent_Reflect(p_data);

    /* Decide           */
    AppAgent_Reason(p_data, &AppAgent_SensorData, &AppAgent_TriggerData);
}

/**
 * \brief  Runs the Actuation task.
 *         The actuation task applies the selected adaptations and updates the model
 *         accordingly.
 *
 * \param  p_data:  Pointer the the interface data of the agent.
 *
 */
void AppAgent_Act(APP_AGENT_INTERFACE_T *p_data)
{
    /* Sensor       */
    SensorAgent_Act(&AppAgent_SensorData);

    /* Trigger      */
    TriggerAgent_Act(&AppAgent_TriggerData);

    /* App          */
    /* No actuations for this agent     */
}

/** @} (end addtogroup AppAgent)    */
/** @} (end addtogroup Agents)      */
