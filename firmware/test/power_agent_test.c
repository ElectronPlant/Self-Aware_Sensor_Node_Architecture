/**
 * \file    power_agent_test.c
 *
 * \brief   This file contains the test function for the power agent.
 *          Note that this is not a complete unit test, but a basic functional test to
 *          see:
 *              -) If the effective charge is updates correctly.
 *              -) If the charge predictions are managed correctly.
 *              -) If the power index is correctly computed.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: PowerAgentTest_
 *
 */

#include "../platform/sa_types.h"
#include "../Platform/sa_utils.h"

#include "power_agent_test.h"
#include "../include/power_agent.h"

/** \addtogroup Agents
 *   @{
 */
/** \addtogroup Tests
 *   @{
 */
/** \addtogroup PowerAgent
 *   @{
 */

/************************************** Defines **************************************************/
#define POWER_AGENT_TEST_BASE_INCREMENT         100
#define POWER_AGENT_TEST_INCREMENTS              10
#define POWER_AGENT_TEST_NOISE_NUM                3

#define POWER_AGENT_TEST_EXPECTED_LIFE          100

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/************* Battery simulation ***********/
static float32_t PowerAgentTest_PredictedCharge = 0;
static float32_t PowerAgentTest_PredictedIncrement = 0;
static float32_t PowerAgentTest_MeasuredCharge = 0;
static float32_t PowerAgentTest_MeasuredChargeDelta = 0;
static float32_t PowerAgentTest_MeasuredIncrement = 0;
float32_t PowerAgentTest_ChargeIncrements[POWER_AGENT_TEST_INCREMENTS] = {0, 0, 0, 10, -10, 0, 0, 15, -20, 5};
float32_t PowerAgentTest_ChargeNoise[POWER_AGENT_TEST_NOISE_NUM] = {0.05, -0.02, -0.03};
//static float32_t PowerAgentTest_ChargeIncrements[POWER_AGENT_TEST_INCREMENTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//static float32_t PowerAgentTest_ChargeNoise[POWER_AGENT_TEST_NOISE_NUM] = {0, 0, 0.0};
static uint8_t PowerAgentTest_IncrementCounter = 0;
static uint8_t PowerAgentTest_NoiseCounter = 0;
static bool_t PowerAgentTest_First = DEF_FALSE;

/* Battery life */
static uint32_t PowerAgentTest_Remaininglife = POWER_AGENT_TEST_EXPECTED_LIFE;

/************* Simualation state ************/
bool_t PowerAgentTest_SimulationRunning = DEF_FALSE;

/************************************** Function implementation **********************************/

/**
 * \brief  Models battery and generates the charge values.
 *
 */
static void PowerAgentTest_SimulateBattery(void) {
    float32_t temp;
    PowerAgentTest_PredictedIncrement = PowerAgentTest_ChargeIncrements[PowerAgentTest_IncrementCounter++];
    PowerAgentTest_PredictedCharge = PowerAgentTest_PredictedIncrement + POWER_AGENT_TEST_BASE_INCREMENT;
    if (DEF_FALSE == PowerAgentTest_First) {
        PowerAgentTest_PredictedIncrement += POWER_AGENT_TEST_BASE_INCREMENT;
        PowerAgentTest_First = DEF_TRUE;
    }
    temp = PowerAgentTest_PredictedCharge + PowerAgentTest_ChargeNoise[PowerAgentTest_NoiseCounter++];
    PowerAgentTest_MeasuredCharge += temp;
    PowerAgentTest_MeasuredIncrement = temp - PowerAgentTest_MeasuredChargeDelta;
    PowerAgentTest_MeasuredChargeDelta = temp;
    PowerAgentTest_IncrementCounter %= POWER_AGENT_TEST_INCREMENTS;
    PowerAgentTest_NoiseCounter %= POWER_AGENT_TEST_NOISE_NUM;
}

/**
 * \brief  Generates the input data for the agent.
 *
 * \param  p_data:  Pointer to the interface data.
 *
 */
static void PowerAgentTest_Predict(POWER_AGENT_INTERFACE_T *p_data) {
    p_data->Inputs.ExpectedLifetime = PowerAgentTest_Remaininglife--;
    PowerAgentTest_Remaininglife = SA_UTILS_MAX(0, PowerAgentTest_Remaininglife);
    p_data->Inputs.PredictedChargeDelta = PowerAgentTest_PredictedCharge;
    p_data->Inputs.PredictedIncrement = PowerAgentTest_PredictedIncrement;
}

/**
 * \brief  Prints the battery data..
 *
 */
static void PowerAgentTest_PrintBattery(void) {
    float32_t remaining_charge = PowerAgent_GetRemainingBatteryLife();
    float32_t perc = PowerAgent_GetRemainingChargePerc();
    printf("-- Battery charge: %f, %f%%\n", remaining_charge, perc);
}

/**
 * \brief  Prints the interface data from the agent.
 *
 * \param  p_data:  Pointer to the interface data.
 *
 */
static void PowerAgentTest_Results(POWER_AGENT_INTERFACE_T *p_data) {

    printf("-- Charge, prediction, feedback confidence: %f, %f, %f, %f\n",
           PowerAgentTest_MeasuredChargeDelta,
           p_data->Inputs.PredictedChargeDelta,
           p_data->Outputs.PowerFeedbackPtr->Power,
           p_data->Outputs.PowerFeedbackPtr->Covariance);
    printf("-- Increment, prediction: %f, %f\n",
           PowerAgentTest_MeasuredIncrement,
           p_data->Inputs.PredictedIncrement);
    printf("-- Power Index: %d\n", p_data->Outputs.PowerIndex);
    PowerAgentTest_PrintBattery();
}

/**
 * \brief  Fakes the sensors used by the power agent.
 *
 * \param  p_obs:  Pointer to the observations data.
 *
 */
static void PowerAgentTest_FakeObservations(POWER_AGENT_OBS_T *p_obs) {
    p_obs->Battery.Charge = PowerAgentTest_MeasuredCharge;
    printf("-- Coulomb counter charge: %f\n", PowerAgentTest_MeasuredCharge);
}

/**
 * \brief  Fakes the actuators used by the power agent.
 *
 * \param  p_acts:  Pointer to the observations data.
 *
 */
static void PowerAgentTest_FakeActuations(POWER_AGENT_ACTS_T *p_acts) {
    /* Just a placeholder for now       */
    printf("-- Power Actuator: called\n");
}

/**
 * \brief  Fakes the battery depletion function. Stops the simulation.
 *
 */
static void PowerAgentTest_FakeBatteryDepletion(void) {
    PowerAgentTest_SimulationRunning = DEF_FALSE;
    printf("--- Battery depleated! ---\n");
}

/**
 * \brief  Runs the functional test for the power agent.
 *
 */
void PowerAgentTest_RunTest()  {
    printf("//////////////////////////////////\n");
    printf("////    Power Agent Test    //////\n");
    printf("//////////////////////////////////\n\n");

    PowerAgent_Init(PowerAgentTest_FakeObservations, PowerAgentTest_FakeActuations, \
                    PowerAgentTest_FakeBatteryDepletion);

    PowerAgentTest_SimulationRunning = DEF_TRUE;
    uint32_t iterations = 0;
    POWER_AGENT_INTERFACE_T data;

    printf("Initial Conditions\n");
    PowerAgentTest_PrintBattery();

    while (DEF_TRUE == PowerAgentTest_SimulationRunning) {
        printf("Iteration %u\n", iterations++);

        PowerAgentTest_SimulateBattery();
        PowerAgentTest_Predict(&data);

        PowerAgent_Oda(&data);

        PowerAgentTest_Results(&data);
    }
}


/** @} (end addtogroup PowerAgent)  */
/** @} (end addtogroup Tests)       */
/** @} (end addtogroup Agents)      */
