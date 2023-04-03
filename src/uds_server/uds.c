/***********************************************************************
 * @file uds.c
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022   all right reserved
***********************************************************************/

// use to define uds_timer
#define UDS_GLOABL

#include "uds.h"
// #include "uds_phycan.h"

#include <stdio.h>

/**
 * @brief uds init
 * 
 * @return int 
            0:      success 
            other:  failed
 */
int uds_init() 
{

    uds_dl_init(&uds_dl);

    uds_tp_init(&uds_tp); 

    uds_ap_init(&uds_ap);

    return 0;
}



/**
 * @brief 
 * 
 */
void uds_process(void)
{
    uds_dl_process_in(&uds_dl);
    uds_tp_process_in(&uds_tp, &uds_dl);
    uds_ap_process(&uds_ap, &uds_tp);
    uds_tp_process_out(&uds_tp, &uds_dl);
    uds_dl_process_out(&uds_dl);
}



/**
 * @brief 
 * 
 */
void uds_timer_tick(void) 
{   
    uint8_t i;
    uds_timer_t *tmr;

    for (i = 0; i < UDS_TIEMR_NUM; i++) {
        tmr = &uds_timer[i];
        if (tmr->st == true) {
            // when timer is expired, execute the action and stop the timer.
            if (--tmr->cnt == 0) {
                tmr->act(tmr->parg);
                tmr->cnt = tmr->val;
                tmr->st = false;
            }
        }
    }
}




