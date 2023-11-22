/**
 * \file    power_agent.c
 *
 * \brief   Main file for the power agent.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: PowerAgent_
 *
 */

/************* Includes *********************/
#include <string.h>
#include "../../platform/sa_types.h"
#include "../../platform/sa_utils.h"

#include "../../include/agents_main.h"
#include "../../configs/config.h"
#include "../../configs/battery_cfg.h"
#include "../../include/power_agent.h"


/** \addtogroup Agents
 *   @{
 */

/** \addtogroup PowerAgent
 *   @{
 */


/************************************** Defines **************************************************/

/************* Battery Model ****************/
#define POWER_AGENT_NUM_AVERAGES            5u

/************************************** Typedef **************************************************/

/**
 * \brief  Battery model
 *         This struct stores all the relevant information to model de battery.
 *         At the moment only the charge will be modeled.
 *
 * \note List of notes:
 *       1. For simplicity the Total charge will be considered as total effective charge,
 *          which is the battery charge that can actually be used, not the rating.
 *       2. It is also assumed that the Power Agent will update the node everytime the node
 *          wakes up, so there is no need to keep track of time. Also, the charge is read
 *          at the end of the activation process.
 */
typedef struct {
    float32_t BatteryChargeTotal;       /* Total charge of the battery                  */
    float32_t BatteryChargeRemaining;  /* Effective remaining charge of the battery     */

    float32_t PreviousCharge;           /* Previous reading of the charge               */
    float32_t PreviousChargeDelta;      /* Previous charge delta                        */
    float32_t Charge;                   /* Current charge measurement                   */
    float32_t ChargeDelta;              /* Current charge delta                         */

    float32_t ChargeAvg;                /* Average charge value                         */
    float32_t ChargeAvgAccum;           /* Accumulated value for the average            */
    float32_t ChargeBuffer[POWER_AGENT_NUM_AVERAGES];
    uint16_t  ChargeBuffPointer;
    bool_t    Initialized;

    CONFIG_POWER_T PowerFeedback;
} POWER_AGENT_BATTERY_MODEL_T;


/************************************** Function prototypes **************************************/

static void PowerAgent_UpdateAvgCharge(POWER_AGENT_OBS_T *p_obs);
static int32_t PowerAgent_PredictBatteryLife(void);
float32_t PowerAgent_GetRemainingBatteryLife();
float32_t PowerAgent_GetRemainingChargePerc();

bool_t PowerAgent_Init(POWER_AGENT_OBSERVATION_T observe, POWER_AGENT_ACTUATION_T act,
                       POWER_AGENT_BATTERY_DEPLETED_T battery_depleted);
void PowerAgent_Oda(POWER_AGENT_INTERFACE_T *p_data);


/************************************** Local Var ************************************************/

POWER_AGENT_OBSERVATION_T PowerAgent_ObserveEnv = NULL;
POWER_AGENT_ACTUATION_T PowerAgent_ActuateEnv = NULL;
POWER_AGENT_BATTERY_DEPLETED_T PowerAgent_BatteryDepleted = NULL;

bool_t PowerAgent_Initialized = DEF_FALSE;

POWER_AGENT_BATTERY_MODEL_T PowerAgent_BatteryModel = {
    .BatteryChargeTotal = (POWER_AGENT_MAX_BATTERY_MA  * POWER_AGENT_EFFECTIVE_CHARGE),
    .BatteryChargeRemaining = (POWER_AGENT_MAX_BATTERY_MA  * POWER_AGENT_EFFECTIVE_CHARGE),
    .PreviousCharge = 0.0,
    .PreviousChargeDelta = 0,
    .Charge = 0.0,
    .ChargeDelta = 0.0,
    .ChargeAvg = 0.0,
    .ChargeAvgAccum = 0.0,
    .ChargeBuffPointer = 0,
    .Initialized = DEF_FALSE,
    .PowerFeedback = {0, POWER_AGENT_COULOMB_COUNTER_CONF},
};

/************************************** Function implementation **********************************/

/************* Tools ************************/
/**
 * \brief  Computes the moving average of the charge.
 *
 * \param  p_obs:  Pointer to the observation data.
 *
 */
static void PowerAgent_UpdateAvgCharge(POWER_AGENT_OBS_T *p_obs)  {

    float32_t previous_value = PowerAgent_BatteryModel.ChargeBuffer[PowerAgent_BatteryModel.ChargeBuffPointer];
    float32_t current_value = p_obs->Battery.Charge;
    uint8_t num;

    PowerAgent_BatteryModel.ChargeBuffer[PowerAgent_BatteryModel.ChargeBuffPointer] = current_value;
    if (++PowerAgent_BatteryModel.ChargeBuffPointer >= POWER_AGENT_NUM_AVERAGES) {
        PowerAgent_BatteryModel.Initialized = DEF_TRUE;
        PowerAgent_BatteryModel.ChargeBuffPointer = 0;
    }

    PowerAgent_BatteryModel.ChargeAvgAccum += current_value - previous_value;
    if (PowerAgent_BatteryModel.Initialized == DEF_TRUE)  {
        num = POWER_AGENT_NUM_AVERAGES;
    } else {
        num = PowerAgent_BatteryModel.ChargeBuffPointer;
    }
    PowerAgent_BatteryModel.ChargeAvg = PowerAgent_BatteryModel.ChargeAvgAccum / num;
 }

 /**
 * \brief  Predicts the remaining battery life in number of activations.
 *
 * \return number of remaining activations.
 *
 * \note List of notes:
 *       1. The number of remaining activations is rounded down (truncated), since the node
 *          cannot realy turn off.
 */
static int32_t PowerAgent_PredictBatteryLife(void)  {
    float32_t life_raw;
    life_raw = (PowerAgent_BatteryModel.BatteryChargeRemaining) / \
               (PowerAgent_BatteryModel.ChargeDelta);

    return (int32_t) life_raw;
}

/**
 * \brief  gets the remaining effective battery charge.
 *
 * \return remaining battery charge.
 *
 */
float32_t PowerAgent_GetRemainingBatteryLife()  {
    return PowerAgent_BatteryModel.BatteryChargeRemaining;
}

/**
 * \brief  Gets the remaining battery charge as a percentage.
 *
 * \return remaining battery charge as a percentage.
 *
 */
float32_t PowerAgent_GetRemainingChargePerc()  {
    float32_t percent;
    percent = PowerAgent_BatteryModel.BatteryChargeRemaining / \
              PowerAgent_BatteryModel.BatteryChargeTotal;
    percent *= 100;

    return percent;
}

/**
 * \brief  Gets a pointer to the power feedback configuration.
 *
 * \return Pointer to the the power feedback configuration.
 *
 */
CONFIG_POWER_T *PowerAgent_GetPowerPtr()
{
    return &PowerAgent_BatteryModel.PowerFeedback;
}

/**
 * \brief  Gets the last power measurement.
 *
 * \return Last power increment measurement.
 *
 */
float32_t PowerAgent_GetPowerMeasurement()
{
    return PowerAgent_BatteryModel.ChargeDelta;
}

/************* Initialization ***************/
/**
 * \brief  Sets the effective battery charge and remaining battery charge.
 *         This is only for simulation.
 *
 * \param  charge: Total effective charge of the battery.
 *
 */
void PowerAgent_SetBatteryCharge(float32_t charge)
{
    PowerAgent_BatteryModel.BatteryChargeTotal = charge;
    PowerAgent_BatteryModel.BatteryChargeRemaining = charge;
}

/**
 * \brief  Initializes the power agent.
 *
 * \param  observe:   Pointer to the observe function.
 * \param  act:       Pointer to the actuation function.
 * \param  battery_depleted    Pointer to the battery depleted function.
 *
 * \return DEF_TRUE if the agent could be initialized correctly; otherwise DEF_FALSE.
 *
 * \note List of notes:
 *       1. The battery depleted function is optional, so it is not checked for NULL pointer.
 *
 */
bool_t PowerAgent_Init(POWER_AGENT_OBSERVATION_T observe, POWER_AGENT_ACTUATION_T act,
                      POWER_AGENT_BATTERY_DEPLETED_T battery_depleted) {

    bool_t initialization = DEF_TRUE;

    /* Initialize functions     */
    if (NULL == observe) {
        initialization = DEF_FALSE;
    } else {
        PowerAgent_ObserveEnv = observe;
    }
    if (NULL == act) {
        initialization = DEF_FALSE;
    } else {
        PowerAgent_ActuateEnv = act;
    }
    PowerAgent_BatteryDepleted = battery_depleted;

    /* Initilize model      */
    memset(PowerAgent_BatteryModel.ChargeBuffer, 0x00,
           sizeof(float32_t) * POWER_AGENT_NUM_AVERAGES);
    PowerAgent_BatteryModel.Initialized = DEF_FALSE;
    PowerAgent_SetBatteryCharge(POWER_AGENT_MAX_BATTERY_MA);

    PowerAgent_Initialized = DEF_TRUE;

    return initialization;
}


/************* Observe **********************/

/**
 * \brief  Learns.
 *         In this context learning is updating the battery model.
 *
 * \param  *p_obs    Pointer to the obervations data.
 *
 */
static void PowerAgent_Learn(POWER_AGENT_OBS_T *p_obs)  {

    /* Charge update                            */
    PowerAgent_BatteryModel.Charge = p_obs->Battery.Charge;
    PowerAgent_BatteryModel.ChargeDelta = p_obs->Battery.Charge - \
                                          PowerAgent_BatteryModel.PreviousCharge;

    /* Update effective charge  (simplified)    */
    if (PowerAgent_BatteryModel.BatteryChargeRemaining > PowerAgent_BatteryModel.ChargeDelta) {
        PowerAgent_BatteryModel.BatteryChargeRemaining -= PowerAgent_BatteryModel.ChargeDelta;
    } else {
        // TODO: create an alarm to alert earlier (5 activations earlier).
        if (NULL != PowerAgent_BatteryDepleted) {
            PowerAgent_BatteryDepleted();
        }
    }

    /* Average charge   */
    PowerAgent_UpdateAvgCharge(p_obs);
}

/**
 * \brief  Completes the model learning process.
 *
 */
static void PowerAgent_Update(void)  {
    PowerAgent_BatteryModel.PreviousCharge = PowerAgent_BatteryModel.Charge;
    PowerAgent_BatteryModel.PreviousChargeDelta = PowerAgent_BatteryModel.ChargeDelta;
}


/**
 * \brief  Reflects.
 *         In this context reflecting is making sure that the data is correct, by:
 *             -) Computing the plausibility of the charge data, by checking the range.
 *             -) Computing the consistency of the charge data, by checking the rate of change.
 *             -) Cross-validity of the charge date, by comparing the predicted charge with the
 *                actual one.
 *
 * \param  *p_data  Pointer to the interface data.
 *
 */
static void PowerAgent_Reflect(POWER_AGENT_INTERFACE_T *p_data)  {

    float32_t temp;

    /* plausibility     */
    //TODO: Implement plausibility calculation.

    /* Consistency      */
    //TODO: Implement consistency calculation.
    //TODO: Power agent, implement plausibility and consistency to adjust the charge confidence.

    /* Cross-validity   */
    temp = PowerAgent_BatteryModel.ChargeDelta - p_data->Inputs.PredictedChargeDelta;
    PowerAgent_BatteryModel.PowerFeedback.Power = temp;

    /* Power feedback   */
    p_data->Outputs.PowerFeedbackPtr = &PowerAgent_BatteryModel.PowerFeedback;
}

/**
 * \brief  Reason.
 *         In this context reasoning is using the available information:
 *             -) Predict the remaining life of the battery.
 *             -) Compute the "power index".
 *             -) Decide the adaptations.
 *
 * \param  *p_data   Pointer to the interface data.
 *
 */
static void PowerAgent_Reason(POWER_AGENT_INTERFACE_T *p_data)  {

    int32_t life;

    life = PowerAgent_PredictBatteryLife();
    life -= (int32_t)p_data->Inputs.ExpectedLifetime;
    life = SA_UTILS_SATURATE(AGENTS_INDEX_MIN_VALUE, AGENTS_INDEX_MAX_VALUE, life);
    p_data->Outputs.PowerIndex = life;
}


/**
 * \brief  Runs the Observe, Decide, Act loop.
 *
 * \param  p_data:  Pointer the the interface data of the agent.
 *
 */
void PowerAgent_Oda(POWER_AGENT_INTERFACE_T *p_data)
{
    POWER_AGENT_OBS_T observations;
    POWER_AGENT_ACTS_T actuations;

    /* Observe data */
    PowerAgent_ObserveEnv(&observations);  /* Collect extarnal data        */

    /* Decide       */
    PowerAgent_Learn(&observations);    /* Update the model             */
    PowerAgent_Reflect(p_data);         /* Check consistency            */
    PowerAgent_Reason(p_data);          /* Compute index                */
    PowerAgent_Update();                /* Final update to the model    */

    /* act          */
    PowerAgent_ActuateEnv(&actuations);    /* Act                          */
}

/**
 * \brief  Runs the Observation task.
 *         The observation task perform the measurements, updates the model based on the
 *         observe data and compute the requried actuations to achieve its goals.
 *
 * \param  p_data: Pointer to the interface data of the agent.
 *
 */
void PowerAgent_Observe(POWER_AGENT_INTERFACE_T *p_data)
{
    POWER_AGENT_OBS_T observations;
    PowerAgent_ObserveEnv(&observations);   /* Collect extarnal data        */

    PowerAgent_Learn(&observations);        /* Update the model             */
    PowerAgent_Reflect(p_data);             /* Check consistency            */

    PowerAgent_Reason(p_data);              /* Compute index                */
}

/**
 * \brief  Runs the Actuation task.
 *         The actuation task applies the selected adaptations and updates the model
 *         accordingly.
 *
 * \param  p_data:  Pointer the the interface data of the agent.
 *
 */
void PowerAgent_Act(POWER_AGENT_INTERFACE_T *p_data)
{
    POWER_AGENT_ACTS_T actuations;
    PowerAgent_Update();                /* Final update to the model    */
    PowerAgent_ActuateEnv(&actuations);    /* Act                          */
}


/** @} (end addtogroup PowerAgent)  */
/** @} (end addtogroup Agents)      */
