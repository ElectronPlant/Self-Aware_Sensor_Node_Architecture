/**
 * \file    battery_data.h
 *
 * \brief   Header file with the battery parameters.
 *          This file should contain all the relevant parameters, so in case the battery changes, this shuold be the
 *          only file that needs to be updated.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

#include "../platform/sa_types.h"

/** \addtogroup Configs
 *   @{
 */

/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/
typedef struct {
    float32_t Power;
    float32_t Covariance;
} CONFIG_POWER_T;

typedef struct {
    uint8_t   ConfigId;
    CONFIG_POWER_T PowerCost;
    float32_t AccuracyCost;
} CONFIG_CONFIGURATION_T;

/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/

/** @} (end addtogroup Configs)  */

#endif /* __CONFIG_H__       */
