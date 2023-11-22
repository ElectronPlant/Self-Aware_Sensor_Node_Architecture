/**
 * \file    trigger_cfg.h
 *
 * \brief   Header file with the trigger configuration.
 *          This file should contain all the relevant parameters, so in case the sensor
 *          changes, this is the only file that needs to be modified.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __TRIGGER_CFG_H__
#define __TRIGGER_CFG_H__

#include "../platform/sa_types.h"
#include "../platform/sa_utils.h"

#include "config.h"

/** \addtogroup Configs
 *   @{
 */
/** \addtogroup TriggerConfig
 *   @{
 */

/************************************** Defines **************************************************/

/************* Trigger **********************/
/* Sampling rate definitions in ms (milli seconds) between samples */
#define TRIGGER_CFG_DEFAULT_SAMPLING    5u  /* Set as the index of TriggerCfg_Periods    */
#define TRIGGER_CFG_MAXIMUM_SAMPLING    0u
#define TRIGGER_CFG_MINIMUM_SAMPLING    11u
#define TRIGGER_CFG_UPDATE_STEP         25u


/************************************** Typedef **************************************************/

/************************************** Var ******************************************************/
extern const uint32_t *TriggerCfg_Periods_Ptr;


/************************************** Function prototypes **************************************/

/** @} (end addtogroup Configs)       */
/** @} (end addtogroup TriggerConfig)  */

#endif /* __TRIGGER_CFG_H__       */
