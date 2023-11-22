/**
 * \file    radio_cfg.h
 *
 * \brief   Header file with the radio configuration.
 *          This file should contain all the relevant parameters, so in case the radio
 *          changes, this is the only file that needs to be modified.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __RADIO_CFG_H__
#define __RADIO_CFG_H__

#include "../platform/sa_types.h"

#include "config.h"

/** \addtogroup Configs
 *   @{
 */

/** \addtogroup RadioConfig
 *   @{
 */

/************************************** Defines **************************************************/
#define RADIO_CFG_IDLE_POWER        0.01
#define RADIO_CFG_DEFAULT_CONFIG    RADIO_CFG_STANDARD_MODE

/************************************** Typedef **************************************************/

typedef enum {
    RADIO_CFG_LOW_POWER_MODE = 0,
    RADIO_CFG_STANDARD_MODE,
    RADIO_CFG_HIGH_POWER_MODE,
    RADIO_CFG_CONFIGS_SIZE,
} RADIO_CFG_LIST_T;


/************************************** Var ******************************************************/

extern CONFIG_CONFIGURATION_T *RadioCfg_Configs_Ptr;

/************************************** Function prototypes **************************************/

/** @} (end addtogroup RadioConfigs)    */
/** @} (end addtogroup Configs)         */

#endif /* __SENSOR_CFG_H__       */