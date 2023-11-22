/**
 * \file    app_cfg.h
 *
 * \brief   Header file with application configuration.
 *          This file should contain all the relevant parameters, so in case the
 *          application changes, this is the only file that needs to be modified.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __APP_CFG_H__
#define __APP_CFG_H__

#include "../platform/sa_types.h"

#include "config.h"

/** \addtogroup Configs
 *   @{
 */
/** \addtogroup AppConfig
 *   @{
 */

/************************************** Defines **************************************************/
#define APP_CFG_MAXIMUM_RATE             1.0   /* Relative variation                       */
#define APP_CFG_MINIMUM_RATE_REF         0.1   /* Minimum reference used for change calc   */
#define APP_CFG_DEFAULT_RATE_HI          5.0   /* Upper threshold for the rate             */
#define APP_CFG_DEFAULT_RATE_LO          1.0   /* Lower threshold for the rate             */
#define APP_CFG_RANGE_LOW                  0   /* Minimum value expected by the sensor     */
#define APP_CFG_RANGE_HIGH               100   /* Maximum value expected by the sensor     */
#define APP_CFG_DEVIATION               10.0

/************************************** Typedef **************************************************/

/************************************** Var ******************************************************/

/************************************** Function prototypes **************************************/

/** @} (end addtogroup Configs)       */
/** @} (end addtogroup AppConfig)     */

#endif /* __APP_CFG_H__       */
