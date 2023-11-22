/**
  * \file    battery_cfg.h
  *
  * \brief   Header file with the battery configuration.
  *          This file should contain all the relevant parameters, so in case the battery
  *          changes, this is the only file that needs to be modified.
  *
  * \author  David Arnaiz
  *
  */

#ifndef __BATTERY_CFG_H__
#define __BATTERY_CFG_H__

#include "../platform/sa_types.h"

/* The battery configuration should only be included from the power agent module        */
#ifdef __POWER_AGENT_C__
#define BATTERY_DATA_EXT
#else
#define BATTERY_DATA_EXT extern
#endif

/** \addtogroup Configs
 *   @{
 */

/** \addtogroup BatteryConfig
 *   @{
 */

/************************************** Defines **************************************************/

/************* Battery data *****************/
#define POWER_AGENT_MAX_BATTERY_MA          5000.0f
#define POWER_AGENT_EFFECTIVE_CHARGE           0.7f

/************* Coulomb Counter **************/
#define POWER_AGENT_COULOMB_COUNTER_CONF      0.01f

/************* Node limits  *****************/
#define POWER_AGENT_CHARGE_UPPER_RANGE      1000.0f      /* Upper range for the charge measurement               */
#define POWER_AGENT_CHARGE_LOWER_RANGE         0.0f      /* Lower range for the charge measurement               */

#define POWER_AGENT_MAX_CONSECUTIVE_CHANGE     0.1f      /* Maximum rate of change for consequtive measurements  */
#define POWER_AGENT_MIN_RATE_VALUE            10.0f      /* Min charge value used when computing the rate        */

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/

/** @} (end addtogroup BatteryConfig)   */
/** @} (end addtogroup Configs)       */

#endif
