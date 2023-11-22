/**
 * \file    radio_agent_test.c
 *
 * \brief   This file contains the test function for the power agent.
 *          Note that this is not a complete unit test, but a basic functional test to
 *          see:
 *              -) If the predicted power and power increment is computed correctly.
 *              -) If the radio state is updated successfully.
 *              -) If the power index is correctly computed.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: RadioAgentTest_
 *
 */

#include "../platform/sa_types.h"

#include "radio_agent_test.h"
#include "../include/radio_agent.h"
#include "../configs/radio_cfg.h"

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup Tests
 *   @{
 */

/** \addtogroup RadioAgent
 *   @{
 */

/************************************** Defines **************************************************/
#define RADIO_AGENT_TEST_ITERATIONS     10u

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
static uint32_t RadioAgentTest_Iterations;

/************************************** Function implementation **********************************/
/**
 * \brief  Fakes the radio observations
 *
 * \param  p_obs:  Pointer to the radio observation data.
 *
 */
static void RadioAgentTest_FakeObservations(RADIO_AGENT_OBS_T *p_obs) {
    switch (RadioAgentTest_Iterations) {
        case 2:
        case 7:
            p_obs->ConfigChange = DEF_TRUE;
            p_obs->Config = RADIO_CFG_HIGH_POWER_MODE;
            printf("-- ::Radio Agent:: Observation: Changed to high power mode\n");
            break;
        case 5:
            p_obs->ConfigChange = DEF_TRUE;
            p_obs->Config = RADIO_CFG_LOW_POWER_MODE;
            printf("-- ::Radio Agent:: Observation: Changed to low power mode\n");
            break;
        case 6:
            p_obs->ConfigChange = DEF_TRUE;
            p_obs->Config = RADIO_CFG_STANDARD_MODE;
            printf("-- ::Radio Agent:: Observation: Changed to standard mode\n");
            break;
        default:
            p_obs->ConfigChange = DEF_FALSE;
            printf("-- ::Radio Agent:: Observation: Unchanged\n");
            break;
    }
}

/**
 * \brief  Fakes the radio actuation.
 *
 * \param  p_act:  Pointer to the radio actuation data.
 *
 */
static void RadioAgentTest_FakeActuations(RADIO_AGENT_ACTS_T *p_acts) {
    /* Just a placeholder for now       */
    printf("-- ::Radio Agent:: Actuation: Data = %f\n", p_acts->Data);
}

/**
 * \brief  Generates the input data for the radio agent.
 *
 * \param  p_int:  Pointer to the interface data.
 *
 */
static void RadioAgentTest_GenerateInputs(RADIO_AGENT_INTERFACE_T *p_int) {

    float32_t data;
    switch (RadioAgentTest_Iterations) {
        case 3:
            data = 1;
            break;
        case 8:
            data = -3.1;
            break;
        default:
            data = 0;
            break;
    }
    p_int->Inputs.Data = data;
    printf("-- ::Radio Agent:: Data input: %f\n", data);
}

/**
 * \brief  Evaluates the output data for the radio agent.
 *
 * \param  p_int:  Pointer to the interface data.
 *
 */
static void RadioAgentTest_EvaluateOutputs(RADIO_AGENT_INTERFACE_T *p_int) {
    printf("-- ::Radio Agent:: Predicted power, increment, covariance: %f, %f, %f\n",
           p_int->Outputs.PredictedPowerPtr->Power, p_int->Outputs.PredictedPowerIncrement,
           p_int->Outputs.PredictedPowerPtr->Covariance);
    printf("-- ::Radio Agent:: Current Config: %u\n", RadioAgent_GetConfig());
}

/**
 * \brief  Runs the test for the power agent.
 *
 */
void RadioAgentTest_RunTest() {
    printf("//////////////////////////////////\n");
    printf("////    Radio Agent Test    //////\n");
    printf("//////////////////////////////////\n\n");

    RadioAgent_Init(RadioAgentTest_FakeObservations, RadioAgentTest_FakeActuations);

    RadioAgentTest_Iterations = 0;
    RADIO_AGENT_INTERFACE_T data;

    for (RadioAgentTest_Iterations = 0; RadioAgentTest_Iterations < RADIO_AGENT_TEST_ITERATIONS; \
         RadioAgentTest_Iterations++) {
        printf("----------------------------------\nIteration: %u\n", RadioAgentTest_Iterations);
        RadioAgentTest_GenerateInputs(&data);
        RadioAgent_Oda(&data);
        RadioAgentTest_EvaluateOutputs(&data);
    }

}

/** @} (end addtogroup RadioAgent)  */
/** @} (end addtogroup Tests)       */
/** @} (end addtogroup Agents)      */
