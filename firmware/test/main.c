/**
  * \file    main.c
  *
  * \brief   Main file for the SA node applicaiton.
  *          This will only be used for testing purposes.
  *
  * \version V0.0
  *
  * \author  DavidArnaiz
  *
  * \note    Module Prefix: Main_
  *
  */

#include <stdlib.h>
#include "../platform/sa_types.h"

#include "power_agent_test.h"
#include "radio_agent_test.h"
#include "app_agent_test.h"
#include "decision_engine_test.h"


/** \addtogroup Testing
*   @{
*/

/** \addtogroup Main
*   @{
*/


/************************************** Defines **************************************************/

/************************************** Typedef **************************************************/

/************************************** Function prototypes **************************************/

/************************************** Local Var ************************************************/

/************************************** Function implementation **********************************/

#ifdef TEST_POWER
void Main_Tests(void) {
    PowerAgentTest_RunTest();
    exit(0);
}

#elif defined TEST_RADIO
void Main_Tests(void) {
    RadioAgentTest_RunTest();
    exit(0);
}

#elif defined TEST_APP
void Main_Tests(void) {
    AppAgentTest_RunTest();
    exit(0);
}

#elif defined TEST_DECISION
void Main_Tests(void) {
    DecisionEngTest_RunTest();
    exit(0);
}

#else
void Main_Tests(void) {
    printf("Nothing to test\n");
}
#endif

/**
  * \brief  main function.
  *
  * \note List of notes:
  *       1. This will only be used for testing purposes.
  */
int main(void)  {

    /* Initialization   */

    /* loop             */
    while (DEF_TRUE) {
        Main_Tests();
    }

    return 0;
}

/** @} (end addtogroup Main)   */
/** @} (end addtogroup Testing) */
