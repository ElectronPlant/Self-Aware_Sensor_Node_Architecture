/**
 * \file    decision_engine_test.c
 *
 * \brief   Main file for the decision engine tests
 *          This are not Unit test, but just functional tests to show functionality.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: DecisionEngTest_
 *
 */

#include <string.h>

#include "../platform/sa_types.h"

#include "../configs/sensor_cfg.h"
#include "../configs/trigger_cfg.h"
#include "../configs/app_cfg.h"
#include "../include/sensor_agent.h"
#include "../include/trigger_agent.h"
#include "../include/app_agent.h"
#include "../configs/radio_cfg.h"
#include "../include/radio_agent.h"
#include "../include/power_agent.h"
#include "../include/decision_engine.h"

#include "decision_engine_test.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup Tests
 *   @{
 */
/** \addtogroup DecisionEngineAgent
 *   @{
 */

/************************************** Defines **************************************************/
#define DECISION_ENGINE_TEST_ITERATIONS         10u

#define DECISION_ENGINE_TEST_SHOW_RADIO         DEF_FALSE
#define DECISION_ENGINE_TEST_SHOW_POWER         DEF_FALSE
#define DECISION_ENGINE_TEST_SHOW_SENSOR        DEF_FALSE
#define DECISION_ENGINE_TEST_SHOW_TRIGGER       DEF_FALSE

#define DECISION_ENGINE_TEST_SHOW_POWER_LOOP    DEF_TRUE

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
static uint32_t DecisionEngTest_Iterations;

/* Battery      */
static float32_t DecisionEngTest_ChargeAccum = 0;

/* Power loop   */
static CONFIG_POWER_T DecisionEngTest_RadioPower    = {0,0};
static CONFIG_POWER_T DecisionEngTest_SensorPower   = {0,0};
static CONFIG_POWER_T DecisionEngTest_FeedbackPower = {0,0};
static CONFIG_POWER_T DecisionEngTest_BasePower     = {0,0};
static CONFIG_POWER_T DecisionEngTest_IdlePower     = {0,0};

/************************************** Function implementation **********************************/
/************* Power Agent ******************/
/**
 * \brief  Fakes a measurement from the coulomb counter.
 *
 * \param  p_obs:  Pointer to the sensor observation data of the sensor agent.
 *
 */
static void DecisionEngTest_PowerObs(POWER_AGENT_OBS_T *p_obs)
{
    switch (DecisionEngTest_Iterations) {
        default:
            DecisionEngTest_ChargeAccum += 200.0;
            break;
    }
    p_obs->Battery.Charge = DecisionEngTest_ChargeAccum;
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_POWER)
        printf("-- ::Power  :: Observation->Data = %f\n", p_obs->Battery.Charge);
}

/**
 * \brief  Fakes the actuatio of the power agent.
 *
 * \param  p_obs:  Pointer to the trigger observation.
 *
 */
static void DecisionEngTest_PowerActs(POWER_AGENT_ACTS_T *p_obs)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_POWER)
        printf("-- ::Power  :: Actiation called\n");
}

/**
 * \brief  Fakes the power agent alarm function.
 *
 * \param  error:  Error code.
 *
 */
static void DecisionEngTest_PowerAlarm(void)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_POWER)
        printf("-- ::Power  :: Alarm ----------------------------\n");
}

/************* Radio Agent ******************/
static void DecisionEngTest_RadioObs(RADIO_AGENT_OBS_T *p_obs)
{
    switch (DecisionEngTest_Iterations) {
        case 255:
            p_obs->ConfigChange = DEF_FALSE;
            if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_RADIO)
                printf("-- ::Radio  :: Observation: Unchanged\n");
            break;
        default:
            p_obs->ConfigChange = DEF_FALSE;
            if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_RADIO)
                printf("-- ::Radio  :: Observation: Unchanged\n");
            break;
    }
}

/**
 * \brief  Fakes the radio actuation.
 *
 * \param  p_act:  Pointer to the radio actuation data.
 *
 */
static void DecisionEngTest_RadioActs(RADIO_AGENT_ACTS_T *p_acts)
{
    /* Just a placeholder for now       */
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_RADIO)
        printf("-- ::Radio  :: Data output: %f\n", p_acts->Data);
}

/************* Application Agent ************/
/**
 * \brief  Fakes a measurement from the sensor.
 *
 * \param  p_obs:  Pointer to the sensor observation data of the sensor agent.
 *
 */
static void DecisionEngTest_SensorObs(SENSOR_AGENT_OBS_T *p_obs)
{
    switch (DecisionEngTest_Iterations) {
        case 0:
        default:
            p_obs->SensorData = 10.0;
            break;
    }
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_SENSOR)
        printf("-- ::Sensor :: Observation->Data = %f\n", p_obs->SensorData);
}

/**
 * \brief  Fakes the configuration change for the sensor.
 *
 * \param  p_acts:  Pointer to the actuation data of the sensor agent.
 *
 */
static void DecisionEngTest_SensorActs(SENSOR_AGENT_ACTS_T *p_acts)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_SENSOR)
        printf("-- ::Sensor :: Actuation->Config = %u\n", p_acts->Config);
}

/**
 * \brief  Fakes the sensor alarm function.
 *
 * \param  error:  Error code.
 *
 */
static void DecisionEngTest_SensorAlarm(SENSOR_AGENT_ERROR_T error)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_SENSOR)
        printf("-- ::Sensor :: Alarm = %u----------------------------\n", error);
}

/**
 * \brief  Fakes the trigger observation.
 *
 * \param  p_obs:  Pointer to the trigger observation.
 *
 */
static void DecisionEngTest_TriggerObs(TRIGGER_AGENT_OBS_T *p_obs)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_TRIGGER)
        printf("-- ::Trigger:: Observation called\n");
}

/**
 * \brief  Fakes the trigger actuation.
 *
 * \param  p_acts:  Pointer to the trigger actuation.
 *
 */
static void DecisionEngTest_TriggerActs(TRIGGER_AGENT_ACTS_T *p_acts)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_TRIGGER)
        printf("-- ::Trigger:: Actuation->Periodicity: %u\n", p_acts->Periodicity);
}

/************* Power Loop *******************/
/**
 * \brief  Gets the relevant data to print the results for the power loop.
 *
 */
void DecisionEngTest_ShowPowerLoopPre(void)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_POWER_LOOP) {
        memcpy(&DecisionEngTest_RadioPower, RadioAgent_GetPowerPtr(), sizeof(CONFIG_POWER_T));
        memcpy(&DecisionEngTest_SensorPower, SensorAgent_GetPowerPtr(), sizeof(CONFIG_POWER_T));
        memcpy(&DecisionEngTest_FeedbackPower, PowerAgent_GetPowerPtr(), sizeof(CONFIG_POWER_T));
        memcpy(&DecisionEngTest_BasePower, DecisionEng_GetBasePowerPtr(), sizeof(CONFIG_POWER_T));
        memcpy(&DecisionEngTest_IdlePower, DecisionEng_GetIdlePowerPtr(), sizeof(CONFIG_POWER_T));
    }
}

/**
 * \brief  Prints the relevant data for the power loop.
 *
 */
void DecisionEngTest_ShowPowerLoopPost(void)
{
    if (DEF_TRUE == DECISION_ENGINE_TEST_SHOW_POWER_LOOP) {
        CONFIG_POWER_T *temp;

        printf(".........\nPowerLoop:\n");
        printf("--- Power prediction: %f, measured %f\n", DecisionEng_GetPower(), PowerAgent_GetPowerMeasurement());
        temp = PowerAgent_GetPowerPtr();
        printf("--- Feedback pre: %f, %f - Post %f, %f\n",
               DecisionEngTest_FeedbackPower.Power,DecisionEngTest_FeedbackPower.Covariance,
               temp->Power, temp->Covariance);
        temp = RadioAgent_GetPowerPtr();
        printf("--- Radio    pre: %f, %f - Post %f, %f\n",
               DecisionEngTest_RadioPower.Power,DecisionEngTest_RadioPower.Covariance,
               temp->Power, temp->Covariance);
        temp = SensorAgent_GetPowerPtr();
        printf("--- Sensor   pre: %f, %f - Post %f, %f\n",
               DecisionEngTest_SensorPower.Power,DecisionEngTest_SensorPower.Covariance,
               temp->Power, temp->Covariance);
        temp = DecisionEng_GetBasePowerPtr();
        printf("--- Base     pre: %f, %f - Post %f, %f\n",
               DecisionEngTest_BasePower.Power,DecisionEngTest_BasePower.Covariance,
               temp->Power, temp->Covariance);
        temp = DecisionEng_GetIdlePowerPtr();
        printf("--- Idle     pre: %f, %f - Post %f, %f\n",
               DecisionEngTest_IdlePower.Power,DecisionEngTest_IdlePower.Covariance,
               temp->Power, temp->Covariance);
        printf(".........\n");
    }
}

/************* Main *************************/
/**
 * \brief  Runs the simulation for the decision engine module.
 *
 */
void DecisionEngTest_RunTest()
{
    DECISION_ENGINE_INIT_T init = {
        .PowerInit.Obs = DecisionEngTest_PowerObs,
        .PowerInit.Act = DecisionEngTest_PowerActs,
        .PowerInit.Alarm = DecisionEngTest_PowerAlarm,
        .RadioInit.Obs = DecisionEngTest_RadioObs,
        .RadioInit.Act = DecisionEngTest_RadioActs,
        .AppInit.Sensor.Obs = DecisionEngTest_SensorObs,
        .AppInit.Sensor.Act = DecisionEngTest_SensorActs,
        .AppInit.Sensor.Alarm = DecisionEngTest_SensorAlarm,
        .AppInit.Trigger.Obs = DecisionEngTest_TriggerObs,
        .AppInit.Trigger.Act = DecisionEngTest_TriggerActs,
        .AppInit.Trigger.Alarm = NULL,
        .AppInit.Alarm = NULL
    };
    DecisionEng_Init(&init);

    for (DecisionEngTest_Iterations = 0; DecisionEngTest_Iterations < DECISION_ENGINE_TEST_ITERATIONS;
         DecisionEngTest_Iterations ++) {

        /* 0 - Generate header  */
        printf("----------------------------------\nIteration: %u\n",
               DecisionEngTest_Iterations);

        /* 1 - Generate inputs  */
        // N/A

        /* 2 - Run loop         */
        DecisionEngTest_ShowPowerLoopPre();
        DecisionEng_Loop();
        DecisionEngTest_ShowPowerLoopPost();

    }
}


/** @} (end addtogroup PowerAgent)  */
/** @} (end addtogroup Tests)       */
/** @} (end addtogroup Agents)      */
