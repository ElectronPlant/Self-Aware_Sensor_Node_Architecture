/**
 * \file    agents.h
 *
 * \brief   Main header file for the agents.
 *
 * \author  David Arnaiz
 *
 */

#ifndef __AGENTS_H__
#define __AGENTS_H__

/** \addtogroup Agents
 *   @{
 */

/** \addtogroup MainAgents
 *   @{
 */

/************************************** Defines **************************************************/

#define AGENTS_INDEX_MAX_VALUE           100
#define AGENTS_INDEX_MIN_VALUE          -100

#define AGENTS_CONSISTENCY_MAX_VALUE    100u
#define AGENTS_CONSISTENCY_MIN_VALUE      0u


/************************************** Typedef **************************************************/

/************************************** Const Var ************************************************/

/************************************** Function prototypes **************************************/
int8_t Agents_Plausibilty(float32_t value, float32_t hi, float32_t lo);
int8_t Agents_Consistency(float32_t rate, float32_t max_rate);
int8_t Agents_CrossValidity(float32_t value, float32_t expected, float32_t deviation);

/** @} (end addtogroup MainAgent)   */
/** @} (end addtogroup Agents)      */

#endif  /* __AGENTS_H__     */
