/**
 * \file    mote_cfg.h
 *
 * \brief   Header file with the mote parameters.
 *          This file should contain all the relevant parameters, so in case the mote changes,
 *          this is the only file that should be modified.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __MOTE_CFG_H__
#define __MOTE_CFG_H__

#include "../platform/sa_types.h"
#include "../platform/sa_utils.h"

#include "config.h"
#include "sensor_cfg.h"
#include "radio_cfg.h"

/** \addtogroup Configs
 *   @{
 */
/** \addtogroup MoteConfig
 *   @{
 */

/************************************** Defines **************************************************/
#define MOTE_CFG_EXPECTED_BATTERY_LIFE      100 * SA_UTILS_DAYS_TO_HOURS * SA_UTILS_HOURS_TO_MINS * \
                                            SA_UTILS_MINS_TO_S;

/************************************** Typedef **************************************************/

/************************************** Var ******************************************************/
extern CONFIG_POWER_T MoteCfg_BasePower;
extern CONFIG_POWER_T MoteCfg_IdlePower;

/************************************** Function prototypes **************************************/

/** @} (end addtogroup MoteConfig)      */
/** @} (end addtogroup Configs)         */

#endif /* __BATTERY_DATA_H__       */
