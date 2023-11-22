/**
 * \file    agents.h
 *
 * \brief   Main header file for the agents.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __SA_TYPES_H__
#define __SA_TYPES_H__

#include <stdio.h>
//#include <inttypes.h>
#include <stdint.h>

/** \addtogroup Platform
 *   @{
 */

/** \addtogroup Types
 *   @{
 */

/************************************** Defines **************************************************/

/** Floating point types */
typedef float   float32_t;
typedef double  float64_t;

/** Bool type    */
typedef uint8_t bool_t;

/** Bool defines */
#define DEF_TRUE  1u
#define DEF_FALSE 0u

#define DEF_OK    1u
#define DEF_FAIL  0u

#define DEF_YES   1u
#define DEF_NO    0u


/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/************************************** Function prototypes **************************************/

/** @} (end addtogroup Types)          */
/** @} (end addtogroup Platform)       */

#endif
