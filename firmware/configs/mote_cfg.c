/**
 * \file    mote_cfg.c
 *
 * \brief   Mote configuration file.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: MoteCfg_
 *
 */

#include "../platform/sa_types.h"

#include "config.h"
#include "sensor_cfg.h"
#include "radio_cfg.h"
#include "mote_cfg.h"

/** \addtogroup Configs
 *   @{
 */
/** \addtogroup MoteConfig
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/
CONFIG_POWER_T MoteCfg_BasePower = {10.0, 10.0};
CONFIG_POWER_T MoteCfg_IdlePower = {0.0,  0.1};

/************************************** Function implementation **********************************/

/** @} (end addtogroup MoteConfig)      */
/** @} (end addtogroup Configs)         */
