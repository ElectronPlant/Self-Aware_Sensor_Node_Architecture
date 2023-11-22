/**
 * \file    radio_config.c
 *
 * \brief   Radio configuration file.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: RadioCfg_
 *
 */

#include "../platform/sa_types.h"

#include "radio_cfg.h"


/** \addtogroup Configs
 *   @{
 */

/** \addtogroup RadioConfig
 *   @{
 */


/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/* TODO store this function in Flash not in RAM     */

/* Radio config array      */
/* The power consumption is per message transmitted     */
CONFIG_CONFIGURATION_T RadioCfg_Configs[RADIO_CFG_CONFIGS_SIZE] = {
    {RADIO_CFG_LOW_POWER_MODE,  { 50, 0.1},    0},
    {RADIO_CFG_STANDARD_MODE,   {100, 0.1},    0},
    {RADIO_CFG_HIGH_POWER_MODE, {150, 0.1},    0},
};
CONFIG_CONFIGURATION_T *RadioCfg_Configs_Ptr = RadioCfg_Configs;

/************************************** Function implementation **********************************/



/** @} (end addtogroup RadioConfig)   */
/** @} (end addtogroup Configs)       */
