/**
 * \file    app_agent_test.c
 *
 * \brief   Main file for the application agent.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: AppAgentTest_
 *
 */

#include "../platform/sa_types.h"

#include "app_agent_test.h"
#include "../configs/sensor_cfg.h"
#include "../configs/trigger_cfg.h"
#include "../configs/app_cfg.h"
#include "../include/sensor_agent.h"
#include "../include/trigger_agent.h"
#include "../include/app_agent.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup Tests
 *   @{
 */
/** \addtogroup AppAgent
 *   @{
 */

/************************************** Defines **************************************************/
#define APP_AGENT_TEST_ITERATIONS     10u

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
static uint32_t AppAgentTest_Iterations;

/************************************** Function implementation **********************************/

/************* Sensor ***********************/
/**
 * \brief  Fakes a measurement from the sensor.
 *
 * \param  p_obs:  Pointer to the sensor observation data of the sensor agent.
 *
 */
static void AppAgentTest_SensorObs(SENSOR_AGENT_OBS_T *p_obs)
{
    switch (AppAgentTest_Iterations) {
        case 3:
            p_obs->SensorData = 50.0;
            break;
        case 9:
            p_obs->SensorData = 110.0;
            break;
        default:
            p_obs->SensorData = 10.0;
            break;
    }
    printf("-- ::Sensor :: Observation->Data = %f\n", p_obs->SensorData);
}

/**
 * \brief  Fakes the configuration change for the sensor.
 *
 * \param  p_acts:  Pointer to the actuation data of the sensor agent.
 *
 */
static void AppAgentTest_SensorActs(SENSOR_AGENT_ACTS_T *p_acts)
{
    printf("-- ::Sensor :: Actuation->Config = %u\n", p_acts->Config);
}

/**
 * \brief  Fakes the sensor alarm function.
 *
 * \param  error:  Error code.
 *
 */
static void AppAgentTest_SensorAlarm(SENSOR_AGENT_ERROR_T error)
{
    printf("-- ::Sensor :: Alarm = %u----------------------------\n", error);
}

/************* Trigger **********************/
/**
 * \brief  Fakes the trigger observation.
 *
 * \param  p_obs:  Pointer to the trigger observation.
 *
 */
static void AppAgentTest_TriggerObs(TRIGGER_AGENT_OBS_T *p_obs)
{
    printf("-- ::Trigger:: Observation called\n");
}

/**
 * \brief  Fakes the trigger actuation.
 *
 * \param  p_acts:  Pointer to the trigger actuation.
 *
 */
static void AppAgentTest_TriggerActs(TRIGGER_AGENT_ACTS_T *p_acts)
{
    printf("-- ::Trigger:: Actuation->Periodicity: %u\n", p_acts->Periodicity);
}

/************* App **************************/
/**
 * \brief  Generates the interface data for the application agent.
 *
 * \param  p_data:  Pointer to the interface data.
 *
 */
static void AppAgentTest_GenerateInputs(APP_AGENT_INTERFACE_T *p_data)
{
    int8_t relevance = 0;
    switch (AppAgentTest_Iterations) {
        case 3:
            relevance = 20;
            break;
        case 4:
            relevance = 50;
        case 8:
            relevance = -100;
    }
    p_data->Inputs.RelevanceTarget = relevance;
    printf("-- ::App    :: Inputs: Relevance target: %d\n",
           p_data->Inputs.RelevanceTarget);
}

/**
 * \brief  Evaluates the output data for the application agent.
 *
 * \param  p_data:  Pointer to the interface data.
 *
 */
static void AppAgentTest_EvaluateOutputs(APP_AGENT_INTERFACE_T *p_data)
{
    printf("-- ::App    :: relevance, target: %d, %d\n",
           p_data->Outputs.RelevanceIndex, p_data->Inputs.RelevanceTarget);
    printf("-- ::App    :: predicted power, increment and covariance: %f, %f, %f\n",
           p_data->Outputs.PredictedPowerPtr->Power,
           p_data->Outputs.PredictedPowerIncrement,
           p_data->Outputs.PredictedPowerPtr->Covariance);
    printf("-- ::App    :: period, trigger conf: %u, %u\n", p_data->Outputs.Periodicity,
           TriggerAgent_GetConfig());
}

/**
 * \brief  Runs the test for the power agent.
 *
 */
void AppAgentTest_RunTest() {
    printf("//////////////////////////////////\n");
    printf("////    App Agent Test    ////////\n");
    printf("//////////////////////////////////\n\n");

    AppAgent_Init(AppAgentTest_SensorObs,
                  AppAgentTest_SensorActs,
                  AppAgentTest_SensorAlarm,
                  AppAgentTest_TriggerObs,
                  AppAgentTest_TriggerActs,
                  NULL, NULL);

    AppAgentTest_Iterations = 0;
    APP_AGENT_INTERFACE_T data;

    for (AppAgentTest_Iterations = 0; AppAgentTest_Iterations < APP_AGENT_TEST_ITERATIONS; \
         AppAgentTest_Iterations++) {
        printf("----------------------------------\nIteration: %u\n", AppAgentTest_Iterations);
        AppAgentTest_GenerateInputs(&data);
        AppAgent_Oda(&data);
        AppAgentTest_EvaluateOutputs(&data);
    }
}

/** @} (end addtogroup AppAgent)   */
/** @} (end addtogroup Tests)      */
/** @} (end addtogroup Agents)     */
