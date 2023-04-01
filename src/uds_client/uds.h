/***********************************************************************
 * @file uds.h
 * @brief  
 * @history
 * Date        Version    Author    description
 * ==========  =========  ========= =======================================
 * 2022-04-12  V1.0       Wcy       Create
 *
 * @Copyright (C)  2022  Jixing. all right reserved
***********************************************************************/
#ifndef __UDS_H__
#define __UDS_H__

#include "uds_ap.h"
#include "virtual_socketcan.h"

#include <stdio.h>
#include <string.h>
#include <linux/can.h>

#define UDS_TP_Cs                (UDS_TP_Cr - UDS_TP_As)
#define UDS_TP_Br                (UDS_TP_Bs - UDS_TP_Ar)


#define TEST_WIN32



/**
 * @brief api of uds
 *  include 
 */


#define UDS_N_WAITCF_IND    0x00u
#define UDS_N_WAITFC_IND    0x01u



#define UDS_TP_WAIT_FC_TIMEOUT          (UDS_TP_As + UDS_TP_Bs)  /* when we are a sender */
#define UDS_TP_WAIT_CF_TIMEOUT          (UDS_TP_Cr)              /* when we are a receiver, and we got a cf already. */




UDS_EXT uds_dl_layer_t uds_dl;
UDS_EXT uds_tp_layer_t uds_tp;
UDS_EXT uds_ap_layer_t uds_ap;


int uds_init(int channel);
void uds_process(void);
void uds_timer_tick(void);

void uds_recv_frame(uds_q_t *q, struct can_frame fr);
void uds_send_frame(struct can_frame *fr);



#endif // __UDS_H__
