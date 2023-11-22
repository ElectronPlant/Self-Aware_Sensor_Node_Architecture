/**
 * \file    decision_engine.c
 *
 * \brief   Main file for the decision engine.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: DecisionEng_
 *
 */

#include <string.h>
#include "../platform/sa_types.h"
#include "../platform/sa_utils.h"

#include "../configs/config.h"
#include "../configs/mote_cfg.h"

#include "../include/agents_main.h"
#include "../include/power_agent.h"
#include "../include/radio_agent.h"
#include "../include/sensor_agent.h"
#include "../include/trigger_agent.h"
#include "../include/app_agent.h"

#include "../include/decision_engine.h"

/** \addtogroup DecisionEngine
 *   @{
 */

/************************************** Defines **************************************************/
#define DECISION_ENGINE_POWER_LOOP_GAIN_CORRECTION  0.5f
#define DECISION_ENGINE_POWER_LOOP_NUM_SOURCES      5u      /* Number of sources of noise in the power estimation   */
                                                            /* Sensor Agent, Radio Agent, Base power, Idle power,
                                                               Coulomb counter    */
/************************************** Typedef **************************************************/
/**
 * \brief  Decision engine model.
 *         This contains the contol parameters of the self-awareness module.
 *
 */
typedef struct {
    int8_t RelevanceIndex;
    int8_t PowerIndex;
    uint32_t ExpectedLifetime;          /* Expected lifetime in s               */
    uint32_t ExpectedLifetimeActs;      /* Expected lifetime in activations     */

    float32_t PredictedPower;
    float32_t PredictedIncrement;

    CONFIG_POWER_T *BasePowerPtr;       /* Pointer to the base consumption      */
    CONFIG_POWER_T *IdlePowerPtr;       /* Pointer to the idel consumption      */
} DECISION_ENGINE_MODEL_T;

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
static DECISION_ENGINE_INTERFACES_T DecisionEng_Interfaces;
static DECISION_ENGINE_MODEL_T DecisionEng_Model;


/************************************** Function implementation **********************************/

/************* Monitoring *******************/
/**
 * \brief  Gets the index values.
 *
 * \param  relevance:   pointer to where the relevance index will be saved.
 * \param  power:       pointer to where the power index will be saved.
 *
 */
void DecisionEng_GetIndexValues(int8_t *relevance, int8_t *power)
{
    *relevance = DecisionEng_Model.RelevanceIndex;
    *power = DecisionEng_Model.PowerIndex;
}

/**
 * \brief Get power feedback values.
 *
 * \param  inp:
 *
 * \return DEF_OK if successful, DEF_FAIL otherwise.
 *
 * \note List of notes:
 *       1.
 */
void DecisionEng_GetFeedbackValues(float32_t *predicted_power, float32_t *predicted_increment,
                                   float32_t *power_feedback)
{
    *predicted_power = DecisionEng_Model.PredictedPower;
    *predicted_increment = DecisionEng_Model.PredictedIncrement;
    *power_feedback = DecisionEng_Interfaces.PowerInterface.Outputs.PowerFeedbackPtr->Power;
}

/************* Tools ************************/
/**
 * \brief  Gets a pointer to the base power configuration.
 *
 * \return Pointer to the base power configuration.
 *
 */
CONFIG_POWER_T *DecisionEng_GetBasePowerPtr()
{
    return DecisionEng_Model.BasePowerPtr;
}

/**
 * \brief  Gets a pointer to the idle power configuration.
 *
 * \return Pointer to the idle power configuration.
 *
 */
CONFIG_POWER_T *DecisionEng_GetIdlePowerPtr()
{
    return DecisionEng_Model.IdlePowerPtr;
}

/**
 * \brief  Get total power
 *
 * \return Total power consumption of the mote.
 *
 */
float32_t DecisionEng_GetPower(void)
{
    return DecisionEng_Model.PredictedPower;
}

/**
 * \brief  Computes the expected battery life in number of activations for the current sampling rate.
 *
 * \return Number of expected activations.
 *
 */
static uint32_t DecisionEng_ExpectedActivations()
{
    uint32_t sampling = TriggerAgent_GetConfig();
    float32_t expected;
    expected = DecisionEng_Model.ExpectedLifetime;
    expected /= sampling;
    expected *= SA_UTILS_S_TO_MILLI_S;
    expected = SA_UTILS_SATURATE(0, SA_UTILS_MAX_UINT32_T, expected);
    return (uint32_t) expected;
}

/************* Main *************************/
/**
 * \brief  Initializes the self-awareness module.
 *
 * \param  p_init: Pointer to the initialization struct.
 *
 * \return DEF_TRUE if the initialization is successfull; otherwise DEF_FALSE.
 *
 */
bool_t DecisionEng_Init(DECISION_ENGINE_INIT_T *p_init)
{
    bool_t initialization;
    initialization = PowerAgent_Init(p_init->PowerInit.Obs,
                                     p_init->PowerInit.Act,
                                     p_init->PowerInit.Alarm);
    if (DEF_TRUE == initialization) {
        RadioAgent_Init(p_init->RadioInit.Obs, p_init->RadioInit.Act);
    }
    if (DEF_TRUE == initialization) {
        AppAgent_Init(p_init->AppInit.Sensor.Obs,
                  p_init->AppInit.Sensor.Act,
                  p_init->AppInit.Sensor.Alarm,
                  p_init->AppInit.Trigger.Obs,
                  p_init->AppInit.Trigger.Act,
                  p_init->AppInit.Trigger.Alarm,
                  p_init->AppInit.Alarm);
    }
    memset(&DecisionEng_Interfaces, 0x00, sizeof(DECISION_ENGINE_INTERFACES_T));
    memset(&DecisionEng_Model, 0x00, sizeof(DECISION_ENGINE_MODEL_T));
    DecisionEng_Model.BasePowerPtr = &MoteCfg_BasePower;
    DecisionEng_Model.IdlePowerPtr = &MoteCfg_IdlePower;
    DecisionEng_Model.ExpectedLifetime = MOTE_CFG_EXPECTED_BATTERY_LIFE;

    return initialization;
}

/**
 * \brief  Set expected battery life.
 *
 * \param  expected_life:   New expected battery life in s.
 *
 */
void DecisionEng_SetExpectedLife(uint32_t expected_life)
{
    DecisionEng_Model.ExpectedLifetime = expected_life;
}

/************* Agents ***********************/

/**
 * \brief  Sets the inputs for the Application agent.
 *
 */
void DecisionEng_SetAppInputs(void)
{
    DecisionEng_Interfaces.AppInterface.Inputs.RelevanceTarget = \
        DecisionEng_Model.RelevanceIndex;
}

/**
 * \brief  Sets the inputs for the Radio agent.
 *
 */
void DecisionEng_SetRadioInputs(void)
{
    DecisionEng_Interfaces.RadioInterface.Inputs.Data = DecisionEng_Interfaces.AppInterface.Outputs.Data;
}

/**
 * \brief  Sets the inputs for the Power agent.
 *
 */
void DecisionEng_SetPowerInputs(void)
{
    float32_t charge;
    float32_t increment;

    charge  = DecisionEng_Model.IdlePowerPtr->Power;
    charge += DecisionEng_Model.BasePowerPtr->Power;
    charge += DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Power;
    charge += DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Power;

    increment  = DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerIncrement;
    increment += DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerIncrement;

    DecisionEng_Interfaces.PowerInterface.Inputs.ExpectedLifetime = DecisionEng_ExpectedActivations();
    DecisionEng_Interfaces.PowerInterface.Inputs.PredictedChargeDelta = charge;
    DecisionEng_Interfaces.PowerInterface.Inputs.PredictedIncrement = increment;

    DecisionEng_Model.PredictedPower = charge;
    DecisionEng_Model.PredictedIncrement = increment;
}

/**
 * \brief  Updates the power predictions.
 *
 */
void DecisionEng_UpdatePowerPredictions(void)
{
    float32_t confidence;
    float32_t gain;
    float32_t feedback;

    confidence  = DecisionEng_Model.BasePowerPtr->Covariance * DecisionEng_Model.BasePowerPtr->Power;
    confidence += DecisionEng_Model.IdlePowerPtr->Covariance * DecisionEng_Model.IdlePowerPtr->Power;
    confidence += DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Covariance * \
                  DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Power;
    confidence += DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Covariance * \
                  DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Power;
    confidence += DecisionEng_Interfaces.PowerInterface.Outputs.PowerFeedbackPtr->Covariance;

    feedback = DecisionEng_Interfaces.PowerInterface.Outputs.PowerFeedbackPtr->Power;

    /* Base power       */
    gain  = DecisionEng_Model.BasePowerPtr->Covariance;
    gain *= DecisionEng_Model.BasePowerPtr->Power;
    gain /= confidence;
    DecisionEng_Model.BasePowerPtr->Power += gain * feedback;
    DecisionEng_Model.BasePowerPtr->Covariance -= gain * DecisionEng_Model.BasePowerPtr->Covariance;

    /* Idle power       */
    gain  = DecisionEng_Model.IdlePowerPtr->Covariance;
    gain *= DecisionEng_Model.IdlePowerPtr->Power;
    gain /= confidence;
    DecisionEng_Model.IdlePowerPtr->Power += gain * feedback;
    DecisionEng_Model.IdlePowerPtr->Covariance -= gain * DecisionEng_Model.IdlePowerPtr->Covariance;

    /* Application Agent    */
    gain  = DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Covariance;
    gain *= DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Power;
    gain /= confidence;
    DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Power += gain * feedback;
    DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Covariance -= gain * \
        DecisionEng_Interfaces.AppInterface.Outputs.PredictedPowerPtr->Covariance;

    /* Radio Agent      */
    gain  = DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Covariance;
    gain *= DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Power;
    gain /= confidence;
    DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Power += gain * feedback;
    DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Covariance  -= gain * \
        DecisionEng_Interfaces.RadioInterface.Outputs.PredictedPowerPtr->Covariance;
}

/**
 * \brief  Performs the decision making of the decision agent.
 *
 */
void DecisionEng_Decide(void)
{
    /* Log indexes  */
    DecisionEng_Model.RelevanceIndex = DecisionEng_Interfaces.AppInterface.Outputs.RelevanceIndex;
    DecisionEng_Model.PowerIndex = DecisionEng_Interfaces.PowerInterface.Outputs.PowerIndex;
}

/**
 * \brief  Runs a complete self-aware loop.
 *
 * \param  inp:
 *
 * \return DEF_OK if successful, DEF_FAIL otherwise.
 *
 * \note List of notes:
 *       1.
 */
void DecisionEng_Loop(void)
{
    /* --- Observe ---  */
    /* Agents   */
    DecisionEng_SetAppInputs();
    AppAgent_Observe(&DecisionEng_Interfaces.AppInterface);

    DecisionEng_SetRadioInputs();
    RadioAgent_Observe(&DecisionEng_Interfaces.RadioInterface);

    DecisionEng_SetPowerInputs();
    PowerAgent_Observe(&DecisionEng_Interfaces.PowerInterface);

    /* Engine   */
    DecisionEng_Decide();
    DecisionEng_UpdatePowerPredictions();


    /* --- Decide ---   */
    // TODO: Decision Engine: complete decision module

    /* --- Act ---      */
    /* Agents   */
    AppAgent_Act(&DecisionEng_Interfaces.AppInterface);

    RadioAgent_Act(&DecisionEng_Interfaces.RadioInterface);

    PowerAgent_Act(&DecisionEng_Interfaces.PowerInterface);
}

/** @} (end addtogroup DecisionEngine)   */
