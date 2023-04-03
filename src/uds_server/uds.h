/***********************************************************************
 * @file uds.h
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022   all right reserved
***********************************************************************/
#ifndef __UDS_H__
#define __UDS_H__

#include "uds_dl.h"
#include "uds_tp.h"
#include "uds_ap.h"
#include "uds_cfg.def"

#include <stdio.h>
#include <string.h>
#include <linux/can.h>



#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief api of uds
 *  include 
 */

UDS_EXT uds_timer_t uds_timer[UDS_TIEMR_NUM];

UDS_EXT uds_dl_layer_t uds_dl;
UDS_EXT uds_tp_layer_t uds_tp;
UDS_EXT uds_ap_layer_t uds_ap;

/**
 * @brief 
 * 
 * @return int 
 */
int uds_init();

/**
 * @brief 
 * 
 */
void uds_process(void);

/**
 * @brief 
 * 
 */
void uds_timer_tick(void);


#ifdef __cplusplus
}
#endif

#endif // __UDS_H__
