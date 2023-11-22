/**
 * \file    sa_utils.h
 *
 * \brief   Miscelaneus resources for the self_awareness module.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __SA_UTILS_H__
#define __SA_UTILS_H__

#include "sa_types.h"

/** \addtogroup Platform
 *   @{
 */

/** \addtogroup Utils
 *   @{
 */

/************************************** Function prototypes **************************************/
/* Special case for functions that will be used by the defines  */


/************************************** Defines **************************************************/

/************* Cons *************************/
/* Time     */
#define SA_UTILS_DAYS_TO_HOURS           24u
#define SA_UTILS_HOURS_TO_MINS           60u
#define SA_UTILS_MINS_TO_S               60u
#define SA_UTILS_MILLI_S_TO_MILLI_S       1u
#define SA_UTILS_S_TO_MILLI_S          1000u
#define SA_UTILS_MINS_TO_MILLI_S       (SA_UTILS_S_TO_MILLI_S * SA_UTILS_MINS_TO_S)
#define SA_UTILS_HOURS_TO_MILLI_S      (SA_UTILS_MINS_TO_MILLI_S * SA_UTILS_HOURS_TO_MINS)
#define SA_UTILS_DAYS_TO_MILLI_S       (SA_UTILS_HOURS_TO_MILLI_S * SA_UTILS_DAYS_TO_HOURS)

/* Values   */
#define SA_UTILS_MAX_UINT32_T          0xFFFFFFFF

/************* Utils ************************/
/**
 * \brief  Maximum of two numbers
 *
 * \param  a:  number 1
 * \param  b:  number 2
 * \return Max(a,b)
 */
#define SA_UTILS_MAX(a, b)  ((a) > (b) ? (a) : (b))

/**
 * \brief   Minimum of two numbers
 *
 * \param  a:  number 1
 * \param  b:  number 2
 * \return Min(a,b)
 */
#define SA_UTILS_MIN(a,b)   ((a) < (b) ? (a) : (b))

/**
 * \brief  Saturate the number within the specified range.
 *
 * \param  min:   Lower range value.
 * \param  max:   Higher range value.
 * \param  value: Value to be saturated.
 *
 * \return Saturated value.
 *
 */
#define SA_UTILS_SATURATE(min, max, value) \
        (SA_UTILS_MAX(SA_UTILS_MIN((value), (max)), (min)))

/**
 * \brief   Absolute value
 *
 * \param  a:  number
 * \return Abs(a)
 */
#define SA_UTILS_ABS(a)   ((a) < (0) ? -(a) : (a))

/**
 * \brief   Manages the conversion from float to int.
 *
 * \param  a:  number
 * \return Round(a)
 */
#define SA_UTILS_ROUND(a)   ((0 <= a) ? (int)(a + 0.5) : (int)(a - 0.5))

/**
 * \brief  Asserts condition to prevent uncontrolled failures.
 *
 * \param  cond: Condition to assert.
 *
 */
#define SA_UTILS_ASSERT(cond) \
            if (DEF_FALSE == (cond)) SaUtils_AssertHandler(__FILE__, __LINE__)


/************************************** Typedef **************************************************/

/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/
void SaUtils_AssertHandler(char *p_file, uint32_t line);
void SaUtils_Average(float32_t *p_avg, float32_t value, float32_t *p_buff, uint16_t size,
                     uint16_t *p_point, bool_t *p_init);
float32_t SaUtils_ChangeRate(float32_t value, float32_t prev, float32_t min);
float32_t SaUtils_UpdateValue(float32_t pred, float32_t obs, float32_t gain);

/** @} (end addtogroup Utils)          */
/** @} (end addtogroup Platform)       */

#endif
