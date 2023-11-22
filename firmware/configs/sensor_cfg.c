/**
 * \file    sensor_cfg.c
 *
 * \brief   Sensor configuration file.
 *
 * \version V0.0
 *
 * \author  DavidArnaiz
 *
 * \note    Module Prefix: SensorCfg_
 *
 */

#include "../platform/sa_types.h"

#include "sensor_cfg.h"


/** \addtogroup Configs
 *   @{
 */

/** \addtogroup SensorConfig
 *   @{
 */


/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/* TODO store this function in Flash not in RAM     */

/* Sensor config array      */
CONFIG_CONFIGURATION_T SensorCfg_Configs[SENSOR_CFG_CONFIGS_SIZE] = {
    {SENSOR_CFG_LOW_POWER_MODE,  { 5, 0.1},    5},
    {SENSOR_CFG_STANDARD_MODE,   {10, 0.1},   10},
    {SENSOR_CFG_HIGH_POWER_MODE, {15, 0.1},   15},
};
CONFIG_CONFIGURATION_T *SensorCfg_Configs_Ptr = SensorCfg_Configs;

/************************************** Function implementation **********************************/



/** @} (end addtogroup SensorConfig)   */
/** @} (end addtogroup Configs)  */
