/**
 * \file    trigger_cfg.c
 *
 * \brief   Trigger configuration file.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: TriggerCfg_
 *
 */

#include "../platform/sa_types.h"

#include "trigger_cfg.h"


/** \addtogroup Configs
 *   @{
 */

/** \addtogroup TriggerConfig
 *   @{
 */


/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

const uint32_t TriggerCfg_Periods[TRIGGER_CFG_MINIMUM_SAMPLING + 1] = {
    1  * SA_UTILS_S_TO_MILLI_S,         /* One sample per second    */
    5  * SA_UTILS_S_TO_MILLI_S,
    15 * SA_UTILS_S_TO_MILLI_S,
    30 * SA_UTILS_S_TO_MILLI_S,
    1  * SA_UTILS_MINS_TO_MILLI_S,      /* One sample per minute    */
    5  * SA_UTILS_MINS_TO_MILLI_S,
    15 * SA_UTILS_MINS_TO_MILLI_S,
    30 * SA_UTILS_MINS_TO_MILLI_S,
    1  * SA_UTILS_HOURS_TO_MILLI_S,     /* One sample per hour      */
    6  * SA_UTILS_HOURS_TO_MILLI_S,
    12 * SA_UTILS_HOURS_TO_MILLI_S,
    1  * SA_UTILS_DAYS_TO_MILLI_S,      /* One sample per day       */
};

const uint32_t *TriggerCfg_Periods_Ptr = TriggerCfg_Periods;

/************************************** Function implementation **********************************/



/** @} (end addtogroup TriggerConfig)   */
/** @} (end addtogroup Configs)         */
