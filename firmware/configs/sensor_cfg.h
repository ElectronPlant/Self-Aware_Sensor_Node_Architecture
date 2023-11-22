/**
 * \file    sensor_cfg.h
 *
 * \brief   Header file with the sensor configuration.
 *          This file should contain all the relevant parameters, so in case the sensor
 *          changes, this is the only file that needs to be modified.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __SENSOR_CFG_H__
#define __SENSOR_CFG_H__

#include <math.h>
#include "../platform/sa_types.h"

#include "config.h"

/** \addtogroup Configs
 *   @{
 */
/** \addtogroup SensorConfig
 *   @{
 */

/************************************** Defines **************************************************/
#define SENSOR_CFG_DEFAULT_CONFIG        SENSOR_CFG_STANDARD_MODE
#define SENSOR_CFG_ERROR_CODE            NAN

#define SENSOR_CFG_IDLE_POWER            0.01

/************************************** Typedef **************************************************/

typedef enum {
    SENSOR_CFG_LOW_POWER_MODE = 0,
    SENSOR_CFG_STANDARD_MODE,
    SENSOR_CFG_HIGH_POWER_MODE,
    SENSOR_CFG_CONFIGS_SIZE,
} SENSOR_CFG_LIST_T;


/************************************** Var ******************************************************/

extern CONFIG_CONFIGURATION_T *SensorCfg_Configs_Ptr;

/************************************** Function prototypes **************************************/

/** @} (end addtogroup Configs)       */
/** @} (end addtogroup SensorConfig)  */

#endif /* __SENSOR_CFG_H__       */
