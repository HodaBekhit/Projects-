/*
 * STK_cfg.h
 *
 *  Created on: Mar 10, 2024
 *      Author: Dell
 */

#ifndef STK_CFG_H_
#define STK_CFG_H_

#include "STK_priv.h"

/*AHB frequency depends on the chosen clock source, assuming HSE is selected.*/
#define STK_AHB_FREQ 16000000UL

/* Clock source selection
 * Options:
 * - AHB_div_8
 * - Proc_clk_AHB
 */
#define STK_CLKSOURCE AHB_div_8

/* SysTick exception request enable
 * Options:
 * - STK_INT_ENABLE
 * - STK_INT_Disable
 */
#define STK_TICKINT STK_INT_ENABLE



#endif /* STK_CFG_H_ */
