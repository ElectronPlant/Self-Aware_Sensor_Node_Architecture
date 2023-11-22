/**
 * \file    sa_utils.c
 *
 * \brief   Miscelaneous resources for the self-awareness module.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: SaUtils_
 *
 */

#include <stdlib.h>
#include "sa_types.h"
#include "sa_utils.h"


/** \addtogroup Platform
 *   @{
 */
/** \addtogroup Utils
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/************************************** Function implementation **********************************/

/**
 * \brief  Handles the assert errors
 *
 * \param  file:  File where the assert failed.
 * \param  line:  Line where the assert failed.
 *
 * \note List of notes:
 *       1. This function prints the error through serial.
 */
void SaUtils_AssertHandler(char *p_file, uint32_t line) {
    // TODO implement debug print and reset.
    printf("Assert in file: \"%s\" at line: %u", p_file, line);
    exit(1);
}

/**
 * \brief  Computes the moving average as used by the agent modules.
 *
 * \param  p_avg:    Pointer to the average value. This will be updated
 * \param  value:    Latest value to add to the average.
 * \param  p_buff:   Pointer to the average buffer. This will be updated.
 * \param  size:     Size of the average buffer.
 * \param  p_point:  Pointer for the average buffer. This will be updated.
 * \param  p_init:   Initialization flag for the average. This will be updated.
 *
 * \note List of notes:
 *       1. The inilialization flag is used to prevent the initial distortion caused in the running
 *          average calculation before the buffer is completely filled.
 */
void SaUtils_Average(float32_t *p_avg, float32_t value, float32_t *p_buff, uint16_t size,
                     uint16_t *p_point, bool_t *p_init) {
    float32_t prev_value;

    prev_value = p_buff[*p_point];
    p_buff[(*p_point)++] = value;

    if (DEF_FALSE == *p_init) {
        *p_avg = (((*p_avg) * (*p_point - 1)) + value)/(*p_point);
        if (*p_point >= size) *p_init = DEF_FALSE;
    } else {
        *p_avg += (value - prev_value) / size;
    }

    *p_point %= size;
}

/**
 * \brief  Computes the rate of change of a value.
 *
 * \param  value:  Reference value for the change calculation.
 * \param  prev:   Previoius value.
 * \param  min:    Minimum value used as reference, see note 1.
 *
 * \return Relative rate of change.
 *
 * \note List of notes:
 *       1. If the reference value is zero or close to zero the rate of change will increase
 *          significantly with very low changes in the data. The minimum reference valy is
 *          used to prevent the excessive rates of change when the measurement is very low.
 *       2. To set the minimum rate of change use the tolerance of the sensor and such.
 */
float32_t SaUtils_ChangeRate(float32_t value, float32_t prev, float32_t min) {

    float32_t ref = SA_UTILS_MIN(SA_UTILS_ABS(value), SA_UTILS_ABS(prev));
    ref = SA_UTILS_MAX(ref, min);

    return SA_UTILS_ABS((value - prev)) / ref;
}

/**
 * \brief  Updates a value in small increments.
 *         The adaptatin in performed in small increments to eliminate noise and
 *
 * \param  pred:  Value to be updated. This value will be updated.
 * \param  obs:   Value that will guide the update.
 * \param  gain:  Gain of the update.
 *
 * \return Updated value.
 *
 */
float32_t SaUtils_UpdateValue(float32_t pred, float32_t obs, float32_t gain) {
    float32_t error = obs - pred;
    return (pred + gain * error);
}

/** @} (end addtogroup Utils)     */
/** @} (end addtogroup Platform)  */
