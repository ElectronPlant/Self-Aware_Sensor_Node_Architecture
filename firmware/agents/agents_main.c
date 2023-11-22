/**
 * \file    agents_main.c
 *
 * \brief   Main file for the agents.
 *          This file contains all the commun features used by the agents.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: Agents_
 *
 */

#include <math.h>
#include "../platform/sa_types.h"
#include "../platform/sa_utils.h"

#include "../include/agents_main.h"


/** \addtogroup Agents
 *   @{
 */

/** \addtogroup MainAgents
 *   @{
 */


/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/************************************** Function implementation **********************************/

/**
 * \brief  Computes the plausibility of a given value.
 *         See note 1.
 *
 * \param  value:   Value for which to check the pausibility.
 * \param  hi:      High value of the range.
 * \param  lo:      Low value of the range.
 *
 * \return Plausibility index for the given value.
 *
 * \note List of notes:
 *       1. At the moment this only checks if the value is in range or not. However,
 *          values close to the extremem may also be seen as unlikely.
 */
int8_t Agents_Plausibilty(float32_t value, float32_t hi, float32_t lo) {

    int8_t plausibility;
    if (hi > value || lo < value) {
        plausibility = AGENTS_CONSISTENCY_MAX_VALUE;
    } else {
        plausibility = AGENTS_CONSISTENCY_MIN_VALUE;
    }

    return plausibility;
}

/**
 * \brief  Computes the consistency value.
 *
 * \param  rate:      rate of change.
 * \param  max_rate:  maximum possible rate of change.
 *
 * \return Consistency index.
 *
 * \note List of notes:
 *       1. At the moment the consistency value only check if the rate is over the maximum
 *          rate or not.
 */
int8_t Agents_Consistency(float32_t rate, float32_t max_rate) {

    return rate <= max_rate ? AGENTS_CONSISTENCY_MIN_VALUE : AGENTS_CONSISTENCY_MAX_VALUE;
}

/**
 * \brief  Computes the cross-validiti of a value.
 *
 * \param  value:     Value for which the cross-validity is checked.
 * \param  expected:  Expected value.
 * \param  deviation: Maximum expected deviation from the expected value.
 *
 * \return Cross-validity index.
 *
 * \note List of notes:
 *       1. For now only the deviation is checked, but it may be interesting to do
 *          a more gradual check.
 */
int8_t Agents_CrossValidity(float32_t value, float32_t expected, float32_t deviation) {

    float32_t diff = SA_UTILS_ABS(expected - value);

    return diff <= deviation ? AGENTS_CONSISTENCY_MIN_VALUE : AGENTS_CONSISTENCY_MAX_VALUE;
}

/** @} (end addtogroup MainAgents)   */
/** @} (end addtogroup Agents)  */
